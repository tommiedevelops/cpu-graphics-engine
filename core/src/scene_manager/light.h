#ifndef LIGHT_H
#define LIGHT_H

#include "game_math/vector.h"

typedef struct Light {
	Vec4f color;
	Vec3f direction;
} Light;

Light* light_create(Vec3f direction, Vec4f color);
void   light_destroy(Light* light);

#endif
