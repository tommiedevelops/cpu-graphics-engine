#include "game_math/vector.h"
#include "scene_manager/scene.h"
#include "asset_manager/texture.h"
#include "renderer/frag_shader.h"
 
static Vec3f compute_reflection_vector(Vec3f l, Vec3f n){
	float dot = vec3f_dot(l,n);
	Vec3f r = vec3f_add(vec3f_scale(n, 2*dot),vec3f_scale(l, -1.0f));
	return vec3f_normalize(r);
}

static Vec3f compute_eyesight_vector(Vec3f cam_pos, Vec3f world_pos){
	return vec3f_normalize(vec3f_add(cam_pos, vec3f_scale(world_pos, -1.0f)));	
}

static Vec4f compute_specular(float exponent, Vec4f light_col, Vec3f world_pos, Vec3f norm, Vec3f cam_pos, Vec3f light_dir){	
	Vec3f e = compute_eyesight_vector(cam_pos, world_pos);
	Vec3f r = compute_reflection_vector(light_dir, norm);
	float r_dot_e = fmaxf(vec3f_dot(r,e), 0.0f);
	float specular = powf(r_dot_e,exponent); 
	Vec4f spec_vec = vec4f_create(light_col.x * specular, light_col.y * specular, light_col.z * specular, 1.0f);
	return spec_vec;
}

static Vec4f compute_diffuse(Vec4f albedo, Vec3f light_dir, Vec4f light_col, Vec3f norm){
	float n_dot_l = fmaxf(vec3f_dot(norm, light_dir), 0.0f);
	Vec4f diffuse = vec4f_scale(light_col, n_dot_l);
	diffuse.x = diffuse.x * albedo.x;
	diffuse.y = diffuse.y * albedo.y;
	diffuse.z = diffuse.z * albedo.z;
	diffuse.w = diffuse.w * albedo.w;
	return diffuse;
}

void fs_unlit(const FSin* in, FSout* out, const FSUniforms* u){
	out->color = u->tex ? texture_sample(u->tex, in->uv.x, in->uv.y) : u->base_color;
	out->depth = in->depth;
}

void fs_lit(const FSin* in, FSout* out, const FSUniforms* u) {
	Vec4f albedo = u->tex ? texture_sample(u->tex, in->uv.x, in->uv.y) : u->base_color;
	Light* l = u->light;
	Vec3f light_dir = vec3f_normalize(l->direction);
	Vec4f light_col = l->color;
	Vec3f norm = in->normal;

	out->color = compute_diffuse(albedo, light_dir, light_col, norm);
	out->depth = in->depth;
}

void fs_phong(const FSin* in, FSout* out, const FSUniforms* u) {
	Vec4f albedo = u->tex ? texture_sample(u->tex, in->uv.x, in->uv.y) : u->base_color;

	Light* l = u->light;
	Vec3f light_dir = vec3f_normalize(l->direction);

	Vec4f light_col = l->color;
	Vec3f norm = vec3f_normalize(in->normal);
	Vec3f cam_pos = u->cam_world_pos;
	Vec3f world_pos = in->world_pos; 
	float exponent = 16.0f;

	Vec4f ambient = vec4f_scale(albedo, 0.3f);
	Vec4f specular = compute_specular(exponent, light_col, world_pos, norm, cam_pos, vec3f_scale(light_dir, -1.0f));
	Vec4f diffuse = compute_diffuse(albedo, light_dir, light_col, norm);

	out->color = vec4f_add(ambient, vec4f_add(specular, diffuse));
	out->depth = in->depth;
}
