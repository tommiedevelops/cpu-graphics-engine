#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "game_math/vector.h"
#include "game_math/quaternion.h"

typedef struct Transform {
	Vec3f position;
	Quat rotation;  
	Vec3f scale;
} Transform;

Transform* transform_create(Vec3f pos, Quat rot, Vec3f scale);
Transform* transform_default();

void transform_destroy(Transform* tr);

#endif
