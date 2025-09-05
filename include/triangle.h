#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdint.h>

#pragma once
#include "vec3f.h"
#include "color.h"
#include "matrix.h"

struct Vec3f;

struct Triangle {
	// no order is assumed here
	struct Vec3f v0;
	struct Vec3f v1;
	struct Vec3f v2;
};

struct Triangle create_triangle(
	struct Vec3f a,
	struct Vec3f b,
	struct Vec3f c
);

struct Vec3f calculate_normal(struct Triangle tri);
struct Triangle apply_transformation(struct Mat4 tr, struct Triangle tri);
struct Triangle tri_perspective_divide(struct Triangle tri); 
struct Triangle apply_perspective_projection(struct Mat4 m, struct Triangle tri);
// array size 3 of struct Vec3f ptrs
void rasterize_triangle(struct Triangle tri, uint32_t* framebuffer, float* zbuffer, uint32_t color);
#endif
