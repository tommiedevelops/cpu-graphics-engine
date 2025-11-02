#include "vector.h"

float lerp_float(float u, float v, float t){
	if(t > 1.0f) t = 1.0f;
	if(t < 0.0f) t = 0.0f;	
	return u*(1-t) + v*t;
}

Vec2f lerp_vec2f(Vec2f u, Vec2f v, float t){
	if(t > 1.0f) t = 1.0f;
	if(t < 0.0f) t = 0.0f;	
	return vec2f_add(vec2f_scale(u, 1-t), vec2f_scale(v, t));
}

Vec3f lerp_vec3f(Vec3f u, Vec3f v, float t){
	if(t > 1.0f) t = 1.0f;
	if(t < 0.0f) t = 0.0f;	
	return vec3f_add(vec3f_scale(u, 1-t), vec3f_scale(v, t));
}

Vec4f lerp_vec4f(Vec4f u, Vec4f v, float t){
	if(t > 1.0f) t = 1.0f;
	if(t < 0.0f) t = 0.0f;	
	return vec4f_add(vec4f_scale(u, 1-t), vec4f_scale(v, t));
}
