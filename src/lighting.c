#include <stdio.h>
#include "lighting.h"

static inline struct Vec3f compute_tri_normal(struct Triangle tri) {

	struct Vec3f v0 = vec4f_to_vec3f(tri.v0);
	struct Vec3f v1 = vec4f_to_vec3f(tri.v1);
	struct Vec3f v2 = vec4f_to_vec3f(tri.v2);
	struct Vec3f x = vec3f_add(v0, vec3f_scale(v1, -1.0f));
	struct Vec3f y = vec3f_add(v0, vec3f_scale(v2, -1.0f));
	struct Vec3f n = vec3f_cross(x,y);
	return vec3f_normalize(n);
}

static inline struct Vec3f compute_reflection_vector(struct Vec3f l, struct Vec3f n){
	float dot = vec3f_dot(l,n);
	struct Vec3f r = vec3f_add(vec3f_scale(n, 2*dot),vec3f_scale(l, -1.0f));
	return vec3f_normalize(r);
}

static inline struct Vec3f compute_eyesight_vector(struct Vec3f cam_pos, struct Vec3f origin){
	return vec3f_normalize(vec3f_add(cam_pos, vec3f_scale(origin, -1.0f)));	
}

static inline struct Vec4f compute_specular(float exponent, struct Vec4f light_col, struct Vec3f norm, struct Vec3f cam_pos, struct Vec3f light_dir, struct Triangle tri){	
	// current heuristic: select random vertex as origin for eye vec
	
	struct Vec3f e = compute_eyesight_vector(cam_pos, vec4f_to_vec3f(tri.v0));
	struct Vec3f r = compute_reflection_vector(light_dir, norm);
	float r_dot_e = fmaxf(vec3f_dot(r,e), 0.0f);
	float specular = pow(r_dot_e,exponent); 

	struct Vec4f spec_vec = vec4f_create(light_col.x * specular, light_col.y * specular, light_col.z * specular, 1.0f);
	return spec_vec;
}

static inline struct Vec4f compute_diffuse(struct Vec3f light_dir, struct Vec4f light_col, struct Vec3f norm){
	float n_dot_l = fmaxf(vec3f_dot(norm, light_dir), 0.0f);
	struct Vec4f diffuse = vec4f_scale(light_col, n_dot_l);
	return diffuse;
}

void precompute_lighting(struct Material* mat, struct Triangle tri, struct LightSource ls, struct Camera cam){
	struct Vec3f norm = compute_tri_normal(tri);
	struct Vec3f light_dir = vec3f_normalize(ls.direction);

	struct Vec4f diffuse = compute_diffuse(ls.direction, ls.color, norm);
	float specular_intensity = 32.0f;
	struct Vec4f specular = compute_specular(specular_intensity, ls.color, norm, cam.transform.position, ls.direction, tri);
	mat->specular = specular;
	mat->diffuse = diffuse;
}


