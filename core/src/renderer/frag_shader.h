#ifndef FRAG_SHADER_H
#define FRAG_SHADER_H

#include "game_math/vector.h"

typedef struct Light Light;
typedef struct Texture Texture;

typedef struct {
	Vec3f world_pos; 
	Vec3f normal; 
	Vec2f uv;
	float depth;
} FSin;

typedef struct {
	Vec4f color;
	float depth;
} FSout;

typedef struct {
	Vec3f cam_world_pos;
	Vec4f base_color;
	Texture* tex;
	Light* light;
} FSUniforms;

typedef void (*FragShaderF)(const FSin* in, FSout* out, const FSUniforms* u);	

void fs_unlit(const FSin* in, FSout* out, const FSUniforms* u);
void fs_lit(const FSin* in, FSout* out, const FSUniforms* u);
void fs_phong(const FSin* in, FSout* out, const FSUniforms* u);
void fs_toon(const FSin* in, FSout* out, const FSUniforms* u);

#endif 

