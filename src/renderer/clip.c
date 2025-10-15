#include "clip.h"

float sdf(struct Plane P, struct Vec4f x){
	return vec4f_dot(P.n, vec4f_add(P.p, vec4f_scale(x, -1.0f) ));
}	

bool inside(struct Plane P, struct Vec4f x, float eps){ return sdf(P,x) <= 0.0f; }

struct Vec2f lerp2(struct Vec2f u, struct Vec2f v, float t){
	if(t > 1.0f){
		//LOG_ERROR("invalid t supplied");
		t = 0.99f;
	}

	if( t < 0.0f){
		//LOG_ERROR("invalid t supplied");
		t = 0.01f;	
	}

	return vec2f_add(vec2f_scale(u, 1-t), vec2f_scale(v, t));
}


struct Vec4f lerp4(struct Vec4f u, struct Vec4f v, float t){
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

float compute_t(struct Plane P, struct Vec4f u, struct Vec4f v){
	float t = (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(P.p, -1.0f) ))
		/ (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(v, -1.0f) ));
	return t;	
}

static inline void copy_uvs(struct Vec2f* from, struct Vec2f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

static inline void copy_vals(struct Vec4f* from, struct Vec4f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

int clip_against_plane(struct Vec4f* in, struct Vec2f* in_uv, int in_n, struct Plane P, struct Vec4f* out, struct Vec2f* out_uv){

	// assuming verts represents a convex polygon that is in clockwise order
	if(in_n == 0) return 0;
	copy_vals(in,out,in_n);					
	copy_uvs(in_uv,out_uv,in_n);
	
	int n = 0;
	for(int v = 0; v < in_n; v++){

		struct Vec4f s = in[v];
		struct Vec2f s_uv = in_uv[v];
		struct Vec4f e = in[(v+1)%in_n];
		struct Vec2f e_uv = in_uv[(v+1)%in_n];

		float eps = 0.001f; // some allowance
		bool sIn = inside(P,s,eps);
		bool eIn = inside(P,e,eps);

		if(sIn && eIn) {
			out[n] = e;	
			out_uv[n++] = e_uv;
			continue;
		}

		if(sIn && !eIn){

			float t = compute_t(P,s,e);
			struct Vec4f i = lerp4(s,e,t);
			struct Vec2f uv = lerp2(s_uv,e_uv,t);

			if(!vec4f_are_equal(i,s)) {
				out[n] = i;
				out_uv[n++] = uv;
			}

			continue;
		}

		if(!sIn && eIn){
			float t = compute_t(P,s,e);
			struct Vec4f i  = lerp4(s,e,t);
			struct Vec2f uv = lerp2(s_uv,e_uv,t);

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

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes){
	struct Vec4f in[9] = {0}, out[9] = {0};
	struct Vec2f in_uv[9] = {0}, out_uv[9] = {0};

	int in_n = 3, out_n = 0;

	in[0] = tri.v0;
	in[1] = tri.v1;
	in[2] = tri.v2;
	
	in_uv[0] = tri.uv0;
	in_uv[1] = tri.uv1;
	in_uv[2] = tri.uv2;

	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(in,in_uv, in_n, planes[i], out, out_uv);
		memcpy(in,out,out_n*sizeof(struct Vec4f));	
		memcpy(in_uv, out_uv, out_n*sizeof(struct Vec2f));
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
		r.tris[k].uv0 = out_uv[0];
		r.tris[k].uv1 = out_uv[k+1];
		r.tris[k].uv2 = out_uv[k+2];
	}

	return r;

}


