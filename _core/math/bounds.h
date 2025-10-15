#ifndef BOUNDS_H
#define BOUNDS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "vector.h"

#define BOUNDS_DEFAULT (Bounds3) { 	\
	.xmin = FLT_MAX, .xmax = -FLT_MAX, 	\
	.ymin = FLT_MAX, .ymax = -FLT_MAX, 	\
	.zmin = FLT_MAX, .zmax = -FLT_MAX 	\
	}


typedef struct Bounds {
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
} Bounds3;

void print_bounds(Bounds3 bounds);
bool bounds_are_equal(Bounds3 a, Bounds3 b);
Bounds3 get_bounds(Vec3f* vertices, int num_vertices);
void update_bounds(Bounds3* bounds, Vec3f* vertices, int num_vertices);

#endif
