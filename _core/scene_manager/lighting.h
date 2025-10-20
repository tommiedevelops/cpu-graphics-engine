#ifndef LIGHTING_H
#define LIGHTING_H

#include "vector.h"

typedef enum LightType {
	DIRECTIONAL,
	GLOBAL
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

Light* light_create(LightType type, Vec3f direction, Vec4f color);
int lighting_add_light(Lighting* lighting, Light* l);
Light* light_default();

#endif
