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
	Light* lights;
	int num_lights;
} Lighting;

#endif
