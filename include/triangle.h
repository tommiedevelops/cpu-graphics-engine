#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include "vec3f.h"
#include "pixel.h"

struct Triangle {
	// no order is assumed here
	struct Vec3f* a;
	struct Vec3f* b;
	struct Vec3f* c;
};

struct Triangle create_triangle(
	struct Vec3f* a,
	struct Vec3f* b,
	struct Vec3f* c
);

// array size 3 of struct Vec3f ptrs
struct Vec3f** sort_vertices_by_y_asc(struct Triangle tri);
struct Vec3f** sort_vertices_by_x_asc(struct Triangle tri);
struct PixelArray rasterize_triangle(struct Triangle tri);

#endif
