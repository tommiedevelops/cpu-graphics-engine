#ifndef SHADING_H
#define SHADING_H

#include <stdlib.h>
#include "material.h"
#include "vector.h"
#include "constants.h"

typedef struct Light {
	Vec3f direction;
	Vec4f color;
} Light;

typedef struct FragShaderInput {
} FragShaderInput;

typedef struct FragShader {
	FragShaderInput input;
	Vec4f (*shade)(FragShaderInput input);
} FragShader;

Material* material_create(Vec4f color, struct Texture* tex);
Vec4f material_get_albedo(struct Material* mat, Vec2f uv);

#endif 
