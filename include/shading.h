#ifndef SHADING_H
#define SHADING_H

#include "vector.h"
#include "constants.h"

struct LightSource {
	struct Vec3f direction;
	struct Vec4f color;
};

struct Texture {
	int width;
	int height;
	struct Vec4f* map;
};

typedef struct FragShaderInput {
} FragShaderInput;

typedef struct FragShader {
	FragShaderInput input;
	struct Vec4f (*shade)(FragShaderInput input);
} FragShader;

struct Material {
	struct Vec4f color; // fallback color if no texture
	struct Texture *tex; // for base
	struct Vec4f diffuse;
	struct Vec4f specular;
	FragShader frag_shader;
};

struct Texture* texture_load(char* filename);
void texture_free(struct Texture tex);
struct Vec4f texture_sample(struct Texture* tex, float u, float v);
struct Material* material_create(struct Vec4f color, struct Texture* tex);
struct Material material_default(); 
struct Vec4f material_get_albedo(struct Material* mat, struct Vec2f uv);

#endif 
