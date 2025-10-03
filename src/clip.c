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

int clip_against_plane(struct Vec3f* verts, int num_verts, struct Plane P){

	// assuming verts represents a convex polygon that is in clockwise order
	
	if(num_verts == 0) return 0;
	struct Vec3f in[12] = {0};

	copy_vals(verts,in,num_verts);					
	
	int n = 0;
	for(int v = 0; v < num_verts; v++){

		struct Vec3f s = in[v];
		struct Vec3f e = in[(v+1)%num_verts];

		float epsilon = 0.01f; // some allowance
		if(inside(P,s,epsilon) && inside(P,e,epsilon)) {
			verts[n++] = e;	
			continue;
		}

		if(inside(P,s,epsilon) && !inside(P,e,epsilon)){
			verts[n++] = intersect(P,s,e);
			continue;
		}

		if(!inside(P,s,epsilon) && inside(P,e,epsilon)){
			verts[n++] = intersect(P,s,e);
			verts[n++] = e;
			continue;
		}

		if(!inside(P,s,epsilon) && !inside(P,e,epsilon)){
			continue;	
		}

	}
	
	return n;
}

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes){
	struct Vec3f a[9], b[9];
	int in_n = 3, out_n;

	a[0] = vec4f_to_vec3f(tri.v0);
	a[1] = vec4f_to_vec3f(tri.v1);
	a[2] = vec4f_to_vec3f(tri.v2);
	
	for(int i = 0; i < num_planes && in_n > 0; i++){
		out_n = clip_against_plane(/*in*/a, in_n, planes[i], /*out*/ b);
		memcpy(a,b,out_n*sizeof(struct Vec3f));	
		in_n = out_n;
	}
}


