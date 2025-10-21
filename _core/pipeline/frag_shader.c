#include "triangle.h"
#include "vector.h"

Vec3f compute_reflection_vector(Vec3f l, Vec3f n){
	float dot = vec3f_dot(l,n);
	Vec3f r = vec3f_add(vec3f_scale(n, 2*dot),vec3f_scale(l, -1.0f));
	return vec3f_normalize(r);
}

Vec3f compute_eyesight_vector(Vec3f cam_pos, Vec3f origin){
	return vec3f_normalize(vec3f_add(cam_pos, vec3f_scale(origin, -1.0f)));	
}

Vec4f compute_specular(float exponent, Vec4f light_col, Vec3f norm, Vec3f cam_pos, Vec3f light_dir){	
	// current heuristic: select random vertex as origin for eye vec	
	Vec3f frag_pos = VEC3F_0;
	Vec3f e = compute_eyesight_vector(cam_pos, frag_pos);
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

