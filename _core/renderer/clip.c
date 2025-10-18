#include "clip.h"
#include "lerp.h"
#include "plane.h"

static inline void copy_uvs(Vec2f* from, Vec2f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

static inline void copy_vals(Vec4f* from, Vec4f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

int clip_against_plane(Vec4f* in, Vec2f* in_uv, int in_n, Plane4 P, Vec4f* out, Vec2f* out_uv){

	// assuming verts represents a convex polygon that is in clockwise order
	if(in_n == 0) return 0;
	copy_vals(in,out,in_n);					
	copy_uvs(in_uv,out_uv,in_n);
	
	int n = 0;
	for(int v = 0; v < in_n; v++){

		Vec4f s = in[v];
		Vec2f s_uv = in_uv[v];
		Vec4f e = in[(v+1)%in_n];
		Vec2f e_uv = in_uv[(v+1)%in_n];

		bool sIn = plane4_inside(P,s);
		bool eIn = plane4_inside(P,e);

		if(sIn && eIn) {
			out[n] = e;	
			out_uv[n++] = e_uv;
			continue;
		}

		if(sIn && !eIn){

			float t = plane4_compute_intersect_t(P,s,e);
			Vec4f i = lerp_vec4f(s,e,t);
			Vec2f uv = lerp_vec2f(s_uv,e_uv,t);

			if(!vec4f_are_equal(i,s)) {
				out[n] = i;
				out_uv[n++] = uv;
			}

			continue;
		}

		if(!sIn && eIn){
			float t = plane4_compute_intersect_t(P,s,e);
			Vec4f i  = lerp_vec4f(s,e,t);
			Vec2f uv = lerp_vec2f(s_uv,e_uv,t);

			out[n] = i;
			out_uv[n++] = uv;
			
			if(!vec4f_are_equal(i,e)) {
				out[n] = e;
				out_uv[n++] = e_uv;
			}

			continue;
		}

		if(!sIn && !eIn){
			continue;	
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
	Vec2f in_uv[9] = {0}, out_uv[9] = {0};

	int in_n = 3, out_n = 0;

	in[0] = tri->v[0].pos;
	in[1] = tri->v[1].pos;
	in[2] = tri->v[2].pos;
	
	in_uv[0] = tri->v[0].uv;
	in_uv[1] = tri->v[1].uv;
	in_uv[2] = tri->v[2].uv;

	Plane4 planes[6];
	int num_planes = 6;
	get_clipping_planes(planes);

	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(in,in_uv, in_n, planes[i], out, out_uv);
		memcpy(in,out,out_n*sizeof(Vec4f));	
		memcpy(in_uv, out_uv, out_n*sizeof(Vec2f));
		in_n = out_n;
	}

	if(out_n < 2) return 0;

	int num_tris = out_n - 2;
	
	for(int k = 0; k < num_tris; k++){
		tris_out[k] = *tri;
		tris_out[k].v[0].pos = out[0];
		tris_out[k].v[1].pos = out[k+1];
		tris_out[k].v[2].pos = out[k+2];	
		tris_out[k].v[0].uv = out_uv[0];
		tris_out[k].v[1].uv = out_uv[k+1];
		tris_out[k].v[2].uv = out_uv[k+2];
	}

	return num_tris;

}


