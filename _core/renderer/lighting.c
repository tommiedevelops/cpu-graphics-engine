#include <stdio.h>
#include "lighting.h"

typedef struct Material Material;

static inline Vec3f compute_tri_normal(Triangle* tri) {
	Vec3f v0 = vec4f_to_vec3f(tri->v[0].pos);
	Vec3f v1 = vec4f_to_vec3f(tri->v[1].pos);
	Vec3f v2 = vec4f_to_vec3f(tri->v[2].pos);
	Vec3f x = vec3f_add(v0, vec3f_scale(v1, -1.0f));
	Vec3f y = vec3f_add(v0, vec3f_scale(v2, -1.0f));
	Vec3f n = vec3f_cross(x,y);
	return vec3f_normalize(n);
}

Vec3f compute_reflection_vector(Vec3f l, Vec3f n){
	float dot = vec3f_dot(l,n);
	Vec3f r = vec3f_add(vec3f_scale(n, 2*dot),vec3f_scale(l, -1.0f));
	return vec3f_normalize(r);
}

Vec3f compute_eyesight_vector(Vec3f cam_pos, Vec3f origin){
	return vec3f_normalize(vec3f_add(cam_pos, vec3f_scale(origin, -1.0f)));	
}

Vec4f compute_specular(float exponent, Vec4f light_col, Vec3f norm, Vec3f cam_pos, Vec3f light_dir, Triangle* tri){	
	// current heuristic: select random vertex as origin for eye vec
	
	Vec3f e = compute_eyesight_vector(cam_pos, vec4f_to_vec3f(tri->v[0].pos));
	Vec3f r = compute_reflection_vector(light_dir, norm);
	float r_dot_e = fmaxf(vec3f_dot(r,e), 0.0f);
	float specular = pow(r_dot_e,exponent); 

	Vec4f spec_vec = vec4f_create(light_col.x * specular, light_col.y * specular, light_col.z * specular, 1.0f);
	return spec_vec;
}

Vec4f compute_diffuse(Vec4f albedo, Vec3f light_dir, Vec4f light_col, Vec3f norm){
	float n_dot_l = fmaxf(vec3f_dot(norm, light_dir), 0.0f);
	Vec4f diffuse = vec4f_scale(light_col, n_dot_l);
	diffuse.x = diffuse.x * albedo.x;
	diffuse.y = diffuse.y * albedo.y;
	diffuse.z = diffuse.z * albedo.z;
	return diffuse;
}

