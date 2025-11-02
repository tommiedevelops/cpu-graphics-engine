#ifndef VERT_SHADER_H
#define VERT_SHADER_H

#include "game_math/vector.h"
#include "game_math/matrix.h"

typedef struct VSin {
	Vec3f pos;
	Vec3f n;
	Vec2f uv;
} VSin;

typedef struct VSout {
	Vec4f pos;
	Vec3f world_pos;
	Vec3f normal;
	Vec2f uv_over_w;	
	float w_inv;
} VSout;

typedef struct {
    Mat4 model;
    Mat4 view;
    Mat4 proj;
    Mat4 viewport;
} VSUniforms;

typedef void (*VertShaderF)(const VSin* in, VSout* out, const VSUniforms* u);

void vs_default(const VSin* in, VSout* out, const VSUniforms* uniforms);

#endif
