#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "vector3.h"
struct Transform {
	struct Vector3 position = {0};
	struct Vector3 forward = {0};
	struct Vector3 left = {0};
	struct Mesh mesh;
}
#endif
