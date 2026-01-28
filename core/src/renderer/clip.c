/// -------------------------------------------------------------
/// Implements Sutherland-Hodgman clipping algorithm to clip
/// vertices against the canonical view volume (Vulkan)
/// 
/// See: https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
/// for pseudocode
///
/// Assumes vertices are in homogeneous clip space (see render.c)
/// -------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include "game_math/lerp.h"
#include "game_math/plane.h"
#include "renderer/vert_shader.h"
#include "error_log.h"

/// Initializes the six clipping planes of the canonical view volume given by
/// y ∈ [-w,w] && x ∈ [-w,w] && z ∈ [0,1]
/// 
/// Normals are pointing "inside"
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

static void compute_intersection(VSout s, VSout e, VSout* i, float t) {
	// assumes memory at &i already allocated
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

static inline void write_vert_to_out(VSout* v, VSout* out, int* out_n_ptr)
{
	out[(*out_n_ptr)++] = *v;
}

static inline void write_itx_to_out(Plane4* P, VSout* s, VSout* e, VSout* out,
					int* out_n_ptr)
{
	float t = plane4_compute_intersect_t(*P,s->pos,e->pos);
	VSout* i = &out[(*out_n_ptr)++];
	compute_intersection(*s,*e,i,t);
}

/// Implements the conditional checks in Sutherland Hodgman for a single
/// pair of vertices.
/// See: https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
static void clip_edge(VSout s, VSout e, Plane4 P, VSout* out, int* out_n) {

	bool s_inside = plane4_inside(P,s.pos);
	bool e_inside = plane4_inside(P,e.pos); 

	if(s_inside && e_inside) 
	{
		write_vert_to_out(&e, out, out_n);
	}

	if(s_inside && !e_inside) 
	{
		write_itx_to_out(&P,&s,&e,out,out_n);
	}

	if(!s_inside && e_inside) 
	{
		write_itx_to_out(&P,&s,&e,out,out_n);	
		write_vert_to_out(&e, out,out_n);
	}

}

static void clip_against_plane(const Plane4 P, 
		               const VSout* in, const int in_n, 
			       VSout* out, int* out_n_ptr) 
{
	*out_n_ptr = 0;

	for(int v = 0; v < in_n; v++){
		VSout s = in[v];
		VSout e = in[(v+1)%in_n];
		clip_edge(s,e,P,out,out_n_ptr);
	}
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

	// Ping-pong buffers: clip a triangle against NUM_PLANES without heap allocations.
	// After each clip, we swap buffers so the next plane uses the newly clipped polygon.
	
	const int MAX_CLIP_VERTS = 16;
	VSout bufA[MAX_CLIP_VERTS];
	VSout bufB[MAX_CLIP_VERTS];

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


