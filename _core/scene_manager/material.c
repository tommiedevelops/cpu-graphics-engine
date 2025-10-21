#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture.h"
#include "vector.h"

typedef struct Texture Texture;
typedef struct Pipeline Pipeline;

typedef struct Material {
	Vec4f color; // fallback color if no texture
	Texture *tex; // for base
	Vec4f diffuse;
	Vec4f specular;
	Pipeline* pipeline;
} Material;

Material* material_default(){
	Material* mat = malloc(sizeof(Material));
	memset(mat,0x0,sizeof(Material));
	mat->color = VEC4F_1;
	mat->pipeline = NULL;
	return mat;
}

struct Material* material_create(Vec4f color, struct Texture* tex){
	struct Material* mat = malloc(sizeof(struct Material));
	mat->color = color;
	mat->tex = tex;
	mat->pipeline = NULL;
	return mat;
}

Vec4f material_get_base_color(Material* mat) {
	return mat->color;
}

Texture* material_get_texture(Material* mat) {
	return mat->tex;
}

Vec4f material_get_albedo(struct Material* mat, Vec2f uv) {
	if(NULL == mat){
		//LOG_ERROR("mat is NULL ptr");
		return VEC4F_0;
	}
	
	if(NULL == mat->tex){
		return mat->color;
	}

	return texture_sample(mat->tex, uv.x, uv.y);
}

Pipeline* material_get_pipeline(Material* mat) {
	return mat->pipeline;
}
