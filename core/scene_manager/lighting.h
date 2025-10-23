#ifndef LIGHTING_H
#define LIGHTING_H

#include "vector.h"

typedef enum LightType {
	LIGHT_DIRECTIONAL,
	LIGHT_GLOBAL
} LightType;

typedef struct Light {
	LightType type;
	Vec3f direction;
	Vec4f color;
} Light;

typedef struct Lighting {
	Light** lights;
	int len, cap;
} Lighting;

Lighting* lighting_init();
void      lighting_uninit(Lighting* lighting);
Light*    light_create(LightType type, Vec3f direction, Vec4f color);
Light*    lighting_get_light(Lighting* lighting, int idx);
int       lighting_add_light(Lighting* lighting, Light* l);
Light*    light_default();

#endif
