#include "clip.h"

float sdf(struct Plane P, struct Vec3f x){
	return vec3f_dot(P.n, vec3f_add(P.p, vec3f_scale(x, -1.0f) ));
}	

bool inside(struct Plane P, struct Vec3f x, float eps){ return sdf(P,x) - eps <= 0.0f; }

struct Vec3f lerp(struct Vec3f u, struct Vec3f v, float t){
	if(t > 1){
		//LOG_ERROR("invalid t supplied");
		t = 1;
	}

	if( t < 0){
		//LOG_ERROR("invalid t supplied");
		t = 0;	
	}

	return vec3f_add(vec3f_scale(u, 1-t), vec3f_scale(v, t));
}

struct Vec3f intersect(struct Plane P, struct Vec3f u, struct Vec3f v){
	float t = vec3f_dot(P.n, vec3f_add(u, vec3f_scale(P.p, -1.0f) ))
		/ vec3f_dot(P.n, vec3f_add(u, vec3f_scale(v, -1.0f) ));
	return lerp(u,v,t);
}

static inline void copy_vals(struct Vec3f* from, struct Vec3f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

static inline struct Vec3f vec4f_to_vec3f(struct Vec4f v) {
    return (struct Vec3f){ v.x, v.y, v.z };
}

int clip_against_plane(struct Vec3f* in, int in_n, struct Plane P, struct Vec3f* out){

	// assuming verts represents a convex polygon that is in clockwise order
	
	if(in_n == 0) return 0;
	copy_vals(in,out,in_n);					
	
	int n = 0;
	for(int v = 0; v < in_n; v++){

		struct Vec3f s = in[v];
		struct Vec3f e = in[(v+1)%in_n];

		float epsilon = 0.01f; // some allowance
		if(inside(P,s,epsilon) && inside(P,e,epsilon)) {
			out[n++] = e;	
			continue;
		}

		if(inside(P,s,epsilon) && !inside(P,e,epsilon)){
			out[n++] = intersect(P,s,e);
			continue;
		}

		if(!inside(P,s,epsilon) && inside(P,e,epsilon)){
			struct Vec3f i  = intersect(P,s,e);
			out[n++] = i;
			
			if(!vec3f_are_about_equal(i,e,0.01f)) {
				out[n++] = e;
			}

			continue;
		}

		if(!inside(P,s,epsilon) && !inside(P,e,epsilon)){
			continue;	
		}

	}
	
	return n;
}

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes){
	struct Vec3f in[9] = {0}, out[9] = {0};
	int in_n = 3, out_n = 0;

	in[0] = vec4f_to_vec3f(tri.v0);
	in[1] = vec4f_to_vec3f(tri.v1);
	in[2] = vec4f_to_vec3f(tri.v2);
	
	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(in, in_n, planes[i], out);
		memcpy(in,out,out_n*sizeof(struct Vec3f));	
		in_n = out_n;
	}

	for(int i = 0; i < out_n; i++){
		print_vec3f(out[i]);
	}

	struct ClipResult r = {0};

	if(out_n == 3){
		r.num_tris = 1;
		r.tris[0] = tri;
		tri.v0 = vec3f_to_vec4f(out[0], 1.0f);
		tri.v1 = vec3f_to_vec4f(out[1], 1.0f);
		tri.v2 = vec3f_to_vec4f(out[2], 1.0f);
	} else if(out_n == 4){
		r.num_tris = 2;

		r.tris[0] = tri;
		tri.v0 = vec3f_to_vec4f(out[0], 1.0f);
		tri.v1 = vec3f_to_vec4f(out[1], 1.0f);
		tri.v2 = vec3f_to_vec4f(out[2], 1.0f);

		r.tris[1] = tri;
		tri.v0 = vec3f_to_vec4f(out[0], 1.0f);
		tri.v1 = vec3f_to_vec4f(out[2], 1.0f);
		tri.v2 = vec3f_to_vec4f(out[3], 1.0f);

	} else {
		r.num_tris = 0;
	}

	return r;

}


