#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdint.h>

#pragma once
#include "vector.h"
#include "matrix.h"
#include "shading.h"

struct Vec3f;

struct Triangle {

	// Vertex 0
	struct Vec4f v0;
	struct Vec2f uv0;
	struct Vec3f n0;
	struct Vec2f uv0_over_w;
	float w0_inv;

	// Vertex 1
	struct Vec4f v1;
	struct Vec2f uv1;
	struct Vec3f n1;
	struct Vec2f uv1_over_w;
	float w1_inv;

	// Vertex 2
	struct Vec4f v2;
	struct Vec2f uv2;
	struct Vec3f n2;
	struct Vec2f uv2_over_w;	
	float w2_inv;
};

struct Bounds get_bounds_from_tri(struct Triangle tri);

struct Vec3f calculate_normal(struct Triangle tri);
struct Triangle tri_perspective_divide(struct Triangle tri); 

// array size 3 of struct Vec3f ptrs
void rasterize_triangle(struct Triangle tri, struct Material* mat, uint32_t* framebuffer, float* zbuffer);
#endif
