#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vector.h"

typedef struct Bounds Bounds;

typedef struct Vertex {
	Vec4f pos;
	Vec2f uv;
	Vec3f n;
	Vec2f uv_over_w;
	float w_inv;
} Vertex;

typedef struct Triangle {
	Vertex v[3];
} Triangle;

Bounds triangle_get_bounds(const Triangle* tri);

#endif
