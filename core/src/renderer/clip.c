#include <string.h>
#include <stdlib.h>
#include "game_math/lerp.h"
#include "game_math/plane.h"
#include "renderer/vert_shader.h"
#include "renderer/triangle.h"
#include "error_log.h"

static inline void get_clipping_planes(struct Plane4* planes){
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

static void prepare_clip_inputs(VSout** in, int* in_n, const Triangle* tri){
	for(int i = 0; i <= 2; i++) in[i] = tri->v[i];
}

static void ensure_ccw(Triangle tri) {
	Vec4f v0 = tri.v[0]->pos;
	Vec4f v1 = tri.v[1]->pos;
	Vec4f v2 = tri.v[2]->pos;

	Vec2f a = (Vec2f) {v0.x/v0.w, v0.y/v0.w};
	Vec2f b = (Vec2f) {v1.x/v1.w, v1.y/v1.w};
	Vec2f c = (Vec2f) {v2.x/v2.w, v2.y/v2.w};

	// define vec2f_cross?
	float area2 = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

	if(area2 < 0.0f) {
		// swap two elements
		VSout* tmp = tri.v[0];
		tri.v[0] = tri.v[1];
		tri.v[1] = tmp;
	}
}

static int prep_clip_output(Triangle* tris_out, VSout** clip_out, int out_n)  {
	// clamp to 0 if less than 2
	int num_tris = (out_n > 2) ? out_n - 2 : 0;

	// fanning out triangle from 0th index
	for(int k = 0; k < num_tris; k++){
		tris_out[k].v[0] = clip_out[0];
		tris_out[k].v[1] = clip_out[k+1];
		tris_out[k].v[2] = clip_out[k+2];	
		ensure_ccw(tris_out[k]);
	}

	return num_tris;
}

static VSout* compute_intersection(VSout* s, VSout* e, float t) {
	VSout* i = malloc(sizeof(VSout));	

	i->pos = lerp_vec4f(s->pos, e->pos, t);
	i->world_pos = lerp_vec3f(s->world_pos, e->world_pos, t);
	i->normal = lerp_vec3f(s->normal, e->normal, t);
	i->uv_over_w = lerp_vec2f(s->uv_over_w, e->uv_over_w, t);
	i->w_inv = lerp_float(s->w_inv, e->w_inv, t);

	return i;
}

static void clip_edge(VSout* s, VSout* e, Plane4 P, VSout** out, int* out_n) {

	bool sIn = plane4_inside(P,s->pos);
	bool eIn = plane4_inside(P,e->pos);

	if(sIn && eIn) out[(*out_n)++] = e;	

	if(sIn && !eIn) {

		float t = plane4_compute_intersect_t(P,s->pos,e->pos);
		VSout* i = compute_intersection(s,e,t);

		out[(*out_n)++] = i;
	}

	if(!sIn && eIn){

		float t = plane4_compute_intersect_t(P,s->pos,e->pos);
		VSout* i = compute_intersection(s,e,t);
		out[(*out_n)++] = i;
		out[(*out_n)++] = e;
	}

}

static void clip_against_plane(VSout** in, int in_n, Plane4 P, VSout** out, int* out_n){

	for(int v = 0; v < in_n; v++){
		VSout* s = in[v];
		VSout* e = in[(v+1)%in_n];
		clip_edge(s,e,P,out,out_n);
	}
	
}

static int clip_poly(VSout** in, int in_n, const Plane4* p, int p_n, VSout** out){
	// assuming in represents a convex polygon that is in clockwise order
	int out_n = 0;
	for(size_t i = 0; i < p_n && in_n > 0; i++){
		out_n = 0;
		clip_against_plane(in,in_n, p[i], out, &out_n);
		memcpy(in,out,out_n*sizeof(VSout*));	
		in_n = out_n;
	}
	return out_n;
}

int clip_tri(const Triangle* tri, Triangle* tris_out){

	VSout* in[9]  = {0};
	VSout* out[9] = {0};
	int in_n  = 3;

	prepare_clip_inputs(in, &in_n, tri);

	const int num_planes = 6;
	Plane4 planes[num_planes];
	get_clipping_planes(planes);

	int out_n = clip_poly(in, in_n, planes, num_planes, out);
	int num_tris = prep_clip_output(tris_out, out, out_n);

	return num_tris;
}

int clip(VSout** in, VSout** out, int* out_n) {

	if(!in || !out || !out_n) {
		LOG_ERROR("inputs were null");
		return -1;
	}

	int in_n = 3; // input is a single triangle 
	
	const int num_planes = 6;
	Plane4 planes[num_planes];
	get_clipping_planes(planes);

	return clip_poly(in, in_n, planes, num_planes, out);

}


