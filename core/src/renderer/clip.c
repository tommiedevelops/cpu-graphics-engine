/// -------------------------------------------------------------
/// Implements Sutherland-Hodgman clipping algorithm to clip
/// vertices against the canonical view volume (Vulkan)
///
/// Assumes vertices are in homogeneous clip space (see render.c)
/// -------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include "game_math/lerp.h"
#include "game_math/plane.h"
#include "renderer/vert_shader.h"
#include "renderer/triangle.h"
#include "error_log.h"

#define CLIP_IN_SIZE (3)
#define CLIP_OUT_SIZE (16)

/// Initializes the six clipping planes of the canonical view volume
/// using the Vulkan clip-space convention.
///
/// The planes are defined in homogeneous clip space such that a point
/// is considered inside the frustum if:
///
///     -w ≤ x ≤ w
///     -w ≤ y ≤ w
///      0 ≤ z ≤ w
///
/// All plane normals are oriented so that plane4_inside(plane, v) returns
/// true for vertices inside the view volume.
///
/// @param planes Output array of 6 Plane4 structures receiving the
///               clip-space frustum planes. Must be preallocated.

static void calculate_clipping_planes(Plane4* planes){
	//top (y = w)
	planes[0].n = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);
	planes[0].p = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);

	//bottom (y = -w)
	planes[1].n = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);
	planes[1].p = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);

	// left (x = -w)
	planes[2].n = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);
	planes[2].p = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);

	// right (x = w)
	planes[3].n = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);
	planes[3].p = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);

	// near (z = 0)
	planes[4].n = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	planes[4].p = VEC4F_0;

	// far (z = w)
	planes[5].n = vec4f_create(0.0f, 0.0f, -1.0f, 1.0f);
	planes[5].p = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
}

/// Computes the interpolated vertex attributes at a line-plane intersection
///
/// Given start and end vertices of an edge in clip space, this function
/// linearly interpolates all per-vertex attributes using parameter t,
/// where t ∈ [0, 1] represents the intersection point along the segment:
///
///  P(t) = S + t (E - S)
///
/// This is used during homogeneous clipping to generate new vertices that 
/// lie exactly on the clipping plane
///
/// @param s Start vertex of the edge
/// @param e End vertex of the edge
/// @param i Output vertex receiving interpolated attributes (must not be NULL)
/// @param t Interpolation parameter along the edge, computed from plane-line
/// 	     intersection in homogeneous coordinates
static void compute_intersection(VSout s, VSout e, VSout* i, float t) {

	if(!i){
		LOG_ERROR("param was null");
		return;
	}

	i->pos = lerp_vec4f(s.pos, e.pos, t);
	i->world_pos = lerp_vec3f(s.world_pos, e.world_pos, t);
	i->normal = lerp_vec3f(s.normal, e.normal, t);
	i->uv_over_w = lerp_vec2f(s.uv_over_w, e.uv_over_w, t);
	i->w_inv = lerp_float(s.w_inv, e.w_inv, t);
}


static void clip_edge(VSout s, VSout e, Plane4 P, VSout* out, int* out_n) {

	bool sIn = plane4_inside(P,s.pos);
	bool eIn = plane4_inside(P,e.pos);

	if(sIn && eIn) out[(*out_n)++] = e;	

	if(sIn && !eIn) {
		float t = plane4_compute_intersect_t(P,s.pos,e.pos);
		VSout* i = &out[(*out_n)++];
		compute_intersection(s,e,i,t);
	}

	if(!sIn && eIn) {

		float t = plane4_compute_intersect_t(P,s.pos,e.pos);

		VSout* i = &out[(*out_n)++];
		compute_intersection(s,e,i,t);

		out[(*out_n)++] = e;
	}

}

/// Performs Sutherland-Hodgman clipping of a polygon against a single plane P
static void clip_against_plane(const Plane4 P, 
		               const VSout* in, const int in_n, 
			       VSout* out, int* out_n_ptr) 
{
	int out_n = 0;

	for(int v = 0; v < in_n; v++){
		VSout s = in[v];
		VSout e = in[(v+1)%in_n];
		clip_edge(s,e,P,out,&out_n);
	}

	*out_n_ptr = out_n;
}

static inline void swap_ptrs(void** a, void** b) {
	void* temp = *a;
	*a = *b;
	*b = temp;
}

/// Performs Sutherland-Hodgman clipping of a polygon (a triangle in this case)
/// against an array of Planes representing the Vulkan Canonical View Volume.
///
/// Uses a 'Ping-Pong Buffer' solution to avoid having to allocate new memory
///
/// @param  in  Input triangle, assumed CCW winding
/// @param  out Pre-allocated memory for the clip output. Minimum size of 16.
/// @return     The number of vertices the clip output has. 

int clip(const VSout in[3], VSout* out) {

	if(!in || !out) {
		LOG_ERROR("inputs were null");
		return -1;
	}
	
	const int NUM_PLANES = 6;
	Plane4 planes[NUM_PLANES];
	calculate_clipping_planes(planes);

	// Ping-Pong buffers
	VSout bufA[CLIP_OUT_SIZE] = {0};
	VSout bufB[CLIP_OUT_SIZE] = {0};
	int sizeA = 3, sizeB = 0;

	// Pointers to current input and output buffers & respective sizes
	VSout* in_ptr = bufA;
	VSout* out_ptr = bufB;
	int* in_size_ptr = &sizeA;
	int* out_size_ptr = &sizeB;

	VSout* final_out = bufB;
	int* final_size = &sizeB;

	for(int i = 0; i < 3; i++) bufA[i] = in[i];
	
	for(int i = 0; i < NUM_PLANES; i++){
		clip_against_plane(planes[i], 
				   in_ptr, *in_size_ptr,
			       	   out_ptr, out_size_ptr);

		swap_ptrs((void*)&in_ptr,(void*)&out_ptr);
		swap_ptrs((void*)&in_size_ptr, (void*)&out_size_ptr);

		// The final output is given by the buffer and size we just 
		// outputted to 

		final_out = (final_out == bufB) ? bufA : bufB;
		final_size = (final_size == &sizeB) 
			      ?  &sizeA : &sizeB;
	}

	for(int i = 0; i < *final_size; i++) out[i] = final_out[i];

	return *final_size;
}


