#ifndef VERT_SHADER_H
#define VERT_SHADER_H

#include <stdlib.h>
#include "vector.h"
#include "matrix.h"

typedef struct VSin {
	// model space
	Vec3f pos;
	Vec3f n;
	Vec2f uv;
} VSin;

typedef struct VSout {
	Vec4f pos;
	Vec3f view_pos;
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

void vs_default(const VSin* in, VSout* out, const VSUniforms* uniforms);

#endif
