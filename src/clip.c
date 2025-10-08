#include "clip.h"

float sdf(struct Plane P, struct Vec4f x){
	return vec4f_dot(P.n, vec4f_add(P.p, vec4f_scale(x, -1.0f) ));
}	

bool inside(struct Plane P, struct Vec4f x, float eps){ return sdf(P,x) <= 0.0f; }

struct Vec4f lerp(struct Vec4f u, struct Vec4f v, float t){
	if(t > 1.0f){
		//LOG_ERROR("invalid t supplied");
		t = 0.99f;
	}

	if( t < 0.0f){
		//LOG_ERROR("invalid t supplied");
		t = 0.01f;	
	}

	return vec4f_add(vec4f_scale(u, 1-t), vec4f_scale(v, t));
}

struct Vec4f intersect(struct Plane P, struct Vec4f u, struct Vec4f v){
	float t = (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(P.p, -1.0f) ))
		/ (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(v, -1.0f) ));
	return lerp(u,v,t);
}

static inline void copy_vals(struct Vec4f* from, struct Vec4f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

int clip_against_plane(struct Vec4f* in, int in_n, struct Plane P, struct Vec4f* out){

	// assuming verts represents a convex polygon that is in clockwise order
	if(in_n == 0) return 0;
	copy_vals(in,out,in_n);					
	
	int n = 0;
	for(int v = 0; v < in_n; v++){

		struct Vec4f s = in[v];
		struct Vec4f e = in[(v+1)%in_n];

		float eps = 0.001f; // some allowance
		bool sIn = inside(P,s,eps);
		bool eIn = inside(P,e,eps);

		if(sIn && eIn) {
			out[n++] = e;	
			continue;
		}

		if(sIn && !eIn){
			struct Vec4f i = intersect(P,s,e);
			if(!vec4f_are_equal(i,s)) out[n++] = intersect(P,s,e);
			continue;
		}

		if(!sIn && eIn){
			struct Vec4f i  = intersect(P,s,e);
			out[n++] = i;
			
			if(!vec4f_are_equal(i,e)) {
				out[n++] = e;
			}

			continue;
		}

		if(!sIn && !eIn){
			continue;	
		}

	}
	
	return n;
}

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes){
	struct Vec4f in[9] = {0}, out[9] = {0};
	int in_n = 3, out_n = 0;

	in[0] = tri.v0;
	in[1] = tri.v1;
	in[2] = tri.v2;
	
	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(in, in_n, planes[i], out);
		memcpy(in,out,out_n*sizeof(struct Vec4f));	
		in_n = out_n;
	}

	struct ClipResult r = {0};
	if(out_n < 2) return r;

	r.num_tris = out_n - 2;
	
	for(int k = 0; k < r.num_tris; k++){
		r.tris[k] = tri;
		r.tris[k].v0 = out[0];
		r.tris[k].v1 = out[k+1];
		r.tris[k].v2 = out[k+2];	
	}

	return r;

}


