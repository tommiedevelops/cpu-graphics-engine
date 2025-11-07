#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asset_manager/texture.h"
#include "game_math/vector.h"
#include "renderer/frag_shader.h"
#include "renderer/vert_shader.h"
#include "renderer/render.h"
#include "material.h"

typedef struct Texture Texture;

Material* material_default(){
	Material* mat = malloc(sizeof(Material));
	memset(mat,0x0,sizeof(Material));
	mat->color = VEC4F_1;
	mat->pipeline = NULL;
	return mat;
}

Material* material_create(Vec4f color, Texture* tex, Pipeline* p){
	Material* mat = malloc(sizeof(Material));
	mat->color = color;
	mat->texture = tex;
	mat->pipeline = p;
	return mat;
}

void material_destroy(Material* mat) {
	pipeline_destroy(mat->pipeline);
	texture_destroy(mat->texture);
	free(mat);
}

Vec4f material_get_base_color(Material* mat) {
	return mat->color;
}

Texture* material_get_texture(Material* mat) {
	return mat->texture;
}

Vec4f material_get_albedo(struct Material* mat, Vec2f uv) {
	if(NULL == mat){
		//LOG_ERROR("mat is NULL ptr");
		return VEC4F_0;
	}
	
	if(NULL == mat->texture){
		return mat->color;
	}

	return texture_sample(mat->texture, uv.x, uv.y);
}

Pipeline* material_get_pipeline(Material* mat) {
	return mat->pipeline;
}
