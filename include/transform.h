#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "vector3.h"
#include "mesh.h"
struct Transform {
	struct Vector3 position = {0};
	struct Vector3 forward = VECTOR3_X;
	struct Mesh mesh;
}
#endif
