#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "vector.h"
#include "matrix.h"

typedef struct Bounds Bounds;

typedef struct {
	Vec4f clip_pos;
	Vec3f view_pos; // view space for ligthing
	Vec3f normal;
	Vec2f uv_over_w;
	float w_inv;
} Vertex;

typedef struct Triangle {
	Vertex v[3];
} Triangle;

Bounds tri_get_bounds(const Triangle* tri);
void tri_apply_perspective_divide(Triangle* tri);
void tri_apply_transformation(Mat4 tr, Triangle* tri);
void tri_precompute_interpolated_values(Triangle* tri);
#endif
