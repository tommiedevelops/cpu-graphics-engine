#ifndef VERT_SHADER_H
#define VERT_SHADER_H

#include <stdlib.h>
#include "vector.h"
#include "matrix.h"

typedef struct Vertex Vertex;

typedef struct {
	// model space
	Vec4f pos;
	Vec3f n;
	Vec2f uv;
} VSin;

typedef struct {
    Mat4 model;
    Mat4 view;
    Mat4 proj;
} VSUniforms;

void vs_default(const VSin* in, Vertex* out, VSUniforms* uniforms);

#endif
