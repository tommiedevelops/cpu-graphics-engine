#include "vector.h"
#include "barycentric.h"

BaryCoords cartesian_to_bary(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {

	float alpha_num = A.x*(C.y-A.y) + (P.y-A.y)*(C.x-A.x) - P.x*(C.y-A.y);
	float alpha_den = (B.y-A.y)*(C.x-A.x) - (B.x-A.x)*(C.y-A.y);
	float alpha = alpha_num / alpha_den;

	float beta_num = P.y - A.y - alpha*(B.y-A.y);
	float beta_den = C.y - A.y;

	float beta = beta_num / beta_den;
	
	float gamma = 1 - alpha - beta;

	return (BaryCoords){alpha, beta, gamma};
}

float bary_interpolate_float(BaryCoords b, float v0, float v1, float v2) {
	return v0*b.alpha + v1*b.beta + v2*b.gamma;	
}

Vec2f bary_interpolate_vec2f(BaryCoords b, Vec2f v0, Vec2f v1, Vec2f v2) {
	float rx = bary_interpolate_float(b, v0.x, v1.x, v2.x); 
	float ry = bary_interpolate_float(b, v0.y, v1.y, v2.y);
	return (Vec2f){rx,ry};
}

Vec3f bary_interpolate_vec3f(BaryCoords b, Vec3f v0, Vec3f v1, Vec3f v2) {
	float rx = bary_interpolate_float(b, v0.x, v1.x, v2.x); 
	float ry = bary_interpolate_float(b, v0.y, v1.y, v2.y);
	float rz = bary_interpolate_float(b, v0.z, v1.z, v2.z);
	return (Vec3f){rx,ry,rz};
}


