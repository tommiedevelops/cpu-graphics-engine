#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdint.h>
#include "vec3f.h"
#include "color.h"

struct Triangle {
	// no order is assumed here
	struct Vec3f* v0;
	struct Vec3f* v1;
	struct Vec3f* v2;
};

struct Triangle create_triangle(
	struct Vec3f* a,
	struct Vec3f* b,
	struct Vec3f* c
);

// array size 3 of struct Vec3f ptrs
void sort_vertices_by_y_asc(struct Triangle tri);
void rasterize_triangle(struct Triangle tri, uint32_t* framebuffer, float* zbuffer, uint32_t color);
#endif
