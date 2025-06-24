#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include "vertex.h"
#include "point.h"

struct Triangle {
	// no order is assumed here
	struct Vertex* a;
	struct Vertex* b;
	struct Vertex* c;
};

struct Triangle create_triangle(
	struct Vertex* a,
	struct Vertex* b,
	struct Vertex* c
);

// array size 3 of struct Vertex ptrs
struct Vertex** sort_vertices_by_y_asc(struct Triangle tri);
struct Vertex** sort_vertices_by_x_asc(struct Triangle tri);
struct PointArray rasterize_triangle(struct Triangle tri);

#endif
