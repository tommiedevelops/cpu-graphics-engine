#include "clip.h"
#include "lerp.h"
#include "plane.h"
#include "vert_shader.h"

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

static void prepare_clip_inputs(Vec4f* in, int* in_n, const Triangle* tri){
	for(int i = 0; i <= 2; i++) in[i] = tri->v[i]->pos;
	*in_n = 3;
}

static int prep_clip_output(Triangle* tris_out, const Vec4f* clip_out, int out_n, const Triangle* tri)  {
	// clamp to 0 if less than 2
	int num_tris = (out_n > 2) ? out_n - 2 : 0;
	
	// fanning out triangle from 0th index
	for(int k = 0; k < num_tris; k++){
		tris_out[k] = *tri;
		tris_out[k].v[0]->pos = clip_out[0];
		tris_out[k].v[1]->pos = clip_out[k+1];
		tris_out[k].v[2]->pos = clip_out[k+2];	
	}
	return num_tris;
}

static void clip_edge(Vec4f s, Vec4f e, Plane4 P, Vec4f*out, int* out_n) {

	bool sIn = plane4_inside(P,s);
	bool eIn = plane4_inside(P,e);

	if(sIn && eIn) {
		out[(*out_n)++] = e;	
	}

	if(sIn && !eIn){

		float t = plane4_compute_intersect_t(P,s,e);
		Vec4f i = lerp_vec4f(s,e,t);

		if(!vec4f_are_equal(i,s)) {
			out[(*out_n)++] = i;
		}

	}

	if(!sIn && eIn){
		float t = plane4_compute_intersect_t(P,s,e);
		Vec4f i  = lerp_vec4f(s,e,t);

		out[(*out_n)++] = i;

		if(!vec4f_are_equal(i,e)) {
			out[(*out_n)++] = e;
		}

	}

}

static void clip_against_plane(Vec4f* in, int in_n, Plane4 P, Vec4f* out, int* out_n){

	for(int v = 0; v < in_n; v++){
		Vec4f s = in[v];
		Vec4f e = in[(v+1)%in_n];
		clip_edge(s,e,P,out,out_n);
	}
	
}

static int clip_poly(Vec4f* in, int in_n, const Plane4* p, int p_n, Vec4f* out){
	// assuming in represents a convex polygon that is in clockwise order
	int out_n = 0;
	for(size_t i = 0; i < p_n && in_n > 0; i++){
		out_n = 0;
		clip_against_plane(in,in_n, p[i], out, &out_n);
		memcpy(in,out,out_n*sizeof(Vec4f));	
		in_n = out_n;
	}
	return out_n;
}

int clip_tri(const Triangle* tri, Triangle* tris_out){
	Vec4f in[9]  = {0};
	Vec4f out[9] = {0};
	int in_n  = 3;

	prepare_clip_inputs(in, &in_n, tri);

	const int num_planes = 6;
	Plane4 planes[num_planes];
	get_clipping_planes(planes);

	int out_n = clip_poly(in, in_n, planes, num_planes, out);
	int num_tris = prep_clip_output(tris_out, out, out_n, tri);

	return num_tris;
}


