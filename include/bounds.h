#ifndef BOUNDS_H
#define BOUNDS_H

#include <float.h>
#include <stdbool.h>

#include "vec3f.h"
struct Vec3f;

struct Bounds {
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
};


#define BOUNDS_DEFAULT (struct Bounds) { 	\
	.xmin = FLT_MAX, .xmax = -FLT_MAX, 	\
	.ymin = FLT_MAX, .ymax = -FLT_MAX, 	\
	.zmin = FLT_MAX, .zmax = -FLT_MAX 	\
	}
void print_bounds(struct Bounds bounds);
bool bounds_are_equal(struct Bounds a, struct Bounds b);
struct Bounds get_bounds(struct Vec3f* vertices, int num_vertices);
void update_bounds(struct Bounds* bounds, struct Vec3f* vertices, int num_vertices);

#endif
