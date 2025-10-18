#include "vector.h"
#include "plane.h"

bool plane4_inside(Plane4 P, Vec4f x) {return plane4_sdf(P,x) <= 0.0f;}

float plane4_sdf(Plane4 P, Vec4f x){
	return vec4f_dot(P.n, vec4f_add(P.p, vec4f_scale(x, -1.0f) ));
}	

float plane4_compute_intersect_t(Plane4 P, Vec4f u, Vec4f v){
	float t = (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(P.p, -1.0f) ))
		/ (float)vec4f_dot(P.n, vec4f_add(u, vec4f_scale(v, -1.0f) ));
	return t;	
}
