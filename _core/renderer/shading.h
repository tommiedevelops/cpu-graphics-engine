#ifndef SHADING_H
#define SHADING_H

#include "vector.h"
#include "constants.h"

struct LightSource {
	Vec3f direction;
	Vec4f color;
};

struct Texture {
	int width;
	int height;
	Vec4f* map;
};

typedef struct FragShaderInput {
} FragShaderInput;

typedef struct FragShader {
	FragShaderInput input;
	Vec4f (*shade)(FragShaderInput input);
} FragShader;

struct Material {
	Vec4f color; // fallback color if no texture
	struct Texture *tex; // for base
	Vec4f diffuse;
	Vec4f specular;
	FragShader frag_shader;
};

struct Texture* texture_load(char* filename);
void texture_free(struct Texture tex);
Vec4f texture_sample(struct Texture* tex, float u, float v);
struct Material* material_create(Vec4f color, struct Texture* tex);
struct Material material_default(); 
Vec4f material_get_albedo(struct Material* mat, Vec2f uv);

#endif 
