#include "clip.h"
#include "lerp.h"
#include "plane.h"
#include "vert_shader.h"

static inline void copy_vals(Vec4f* from, Vec4f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

int clip_against_plane(Vec4f* in, int in_n, Plane4 P, Vec4f* out){
	// assuming verts represents a convex polygon that is in clockwise order
	if(in_n == 0) return 0;
	copy_vals(in,out,in_n);					
	
	int n = 0;
	for(int v = 0; v < in_n; v++){

		Vec4f s = in[v];
		Vec4f e = in[(v+1)%in_n];

		bool sIn = plane4_inside(P,s);
		bool eIn = plane4_inside(P,e);

		if(sIn && eIn) {
			out[n++] = e;	
		}

		if(sIn && !eIn){

			float t = plane4_compute_intersect_t(P,s,e);
			Vec4f i = lerp_vec4f(s,e,t);

			if(!vec4f_are_equal(i,s)) {
				out[n++] = i;
			}

		}

		if(!sIn && eIn){
			float t = plane4_compute_intersect_t(P,s,e);
			Vec4f i  = lerp_vec4f(s,e,t);

			out[n++] = i;
			
			if(!vec4f_are_equal(i,e)) {
				out[n++] = e;
			}

		}

	}
	
	return n;
}

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

int clip_tri(const Triangle* tri, Triangle* tris_out){
	Vec4f in[9] = {0}, out[9] = {0};

	int in_n = 3, out_n = 0;

	for(int i = 0; i <= 2; i++) in[i] = tri->v[i]->pos;

	Plane4 planes[6];
	int num_planes = 6;
	get_clipping_planes(planes);

	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(in,in_n, planes[i], out);
		memcpy(in,out,out_n*sizeof(Vec4f));	
		in_n = out_n;
	}

	if(out_n < 2) return 0;

	int num_tris = out_n - 2;
	
	for(int k = 0; k < num_tris; k++){
		tris_out[k] = *tri;
		tris_out[k].v[0]->pos = out[0];
		tris_out[k].v[1]->pos = out[k+1];
		tris_out[k].v[2]->pos = out[k+2];	
	}

	return num_tris;

}


