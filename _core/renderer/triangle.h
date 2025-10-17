#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"
#include "shading.h"
#include "vector.h" 
#include "lighting.h"
#include "scene_manager.h"

typedef struct Vertex {
	Vec4f pos;
	Vec2f uv;
	Vec3f n;
	Vec2f uv_over_w;
	float w_inv;
} Vertex;

struct Triangle {
	Vertex v[3];
};

struct Bounds get_bounds_from_tri(struct Triangle tri);
Vec3f calculate_normal(struct Triangle tri);
#endif
