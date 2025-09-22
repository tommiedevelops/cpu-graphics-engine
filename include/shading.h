#ifndef SHADING_H
#define SHADING_H

#include "vec3f.h"
#include "vec4f.h"

struct LightSource {
	struct Vec3f direction;
};

struct Material {
	struct Vec3f color; // fallback color if no texture
	struct Texture *albedo; // for base
};

struct Material material_default(); 

struct Texture {
	int width;
	int height;
	struct Vec4f* map;
};

struct Texture load_texture(char* filename);

void free_texture(struct Texture tex);
#endif 
