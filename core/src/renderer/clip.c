#include <string.h>
#include <stdlib.h>
#include "game_math/lerp.h"
#include "game_math/plane.h"
#include "renderer/vert_shader.h"
#include "renderer/triangle.h"
#include "error_log.h"

#define NUM_PLANES (6)
#define CLIP_IN_SIZE (3)
#define CLIP_OUT_SIZE (16)

static void calculate_clipping_planes(struct Plane4* planes){
	/* all normals facing 'plane4_inside'*/
	/* plane4_inside => -w<=x<=w, -w<=y<=w, 0<=z<=w*/

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

	if(!sIn && eIn){

		float t = plane4_compute_intersect_t(P,s.pos,e.pos);

		VSout* i = &out[(*out_n)++];
		compute_intersection(s,e,i,t);

		out[(*out_n)++] = e;
	}

}

static int clip_against_plane(const Plane4 P, 
		              const VSout* in, 
			      const int in_n, 
			      VSout* out){
	int out_n = 0;

	for(int v = 0; v < in_n; v++){
		VSout s = in[v];
		VSout e = in[(v+1)%in_n];
		clip_edge(s,e,P,out,&out_n);
	}

	return out_n;
}

void swap_ptrs(void* a, void* b) {
	void* temp = a;
	a = b;
	b = a;
}

int clip(const VSout in[3], VSout* out) {

	if(!in || !out) {
		LOG_ERROR("inputs were null");
		return -1;
	}
	
	Plane4 planes[NUM_PLANES];
	calculate_clipping_planes(planes);

	// run clipping algorithm using ping pong buffers
	VSout bufA[CLIP_OUT_SIZE] = {0};
	VSout bufB[CLIP_OUT_SIZE] = {0};
	int sizeA = 3, sizeB = 0;

	VSout* in_ptr = bufA;
	VSout* out_ptr = bufB;
	int* in_size_ptr = &sizeA;
	int* out_size_ptr = &sizeB;

	// write initial input into one of the buffers
	for(int i = 0; i < 3; i++) bufA[i] = in[i];

	for(int i = 0; i < NUM_PLANES; i++){
		*out_size_ptr = clip_against_plane(
					planes[i], 
					in_ptr, *in_size_ptr,
			       		out_ptr);
		swap_ptrs((void*)in_ptr,(void*)out_ptr);
		swap_ptrs((void*)in_size_ptr, (void*)out_size_ptr);
		*out_size_ptr = 0;
	}
	
	// write the results into output.
	for(int i = 0; i < *in_size_ptr; i++){
		out[i] = in_ptr[i];
	}

	return *in_size_ptr;
}


