#ifndef VERTEX_H
#define VERTEX_H

#include <stdbool.h>
#include "bounds.h"

struct Bounds;

struct Vertex {
	float x;
	float y;
	float z;
};

void convert_vertex_to_int_values(struct Vertex* vertex);
void scale_vertex(struct Vertex* vertex, float scalar);
void translate_vertex(struct Vertex* vertex, float dx, float dy, float dz);
bool vertices_are_equal(struct Vertex a, struct Vertex b);

void normalize_vertices(float sidelength, struct Vertex* vertices, int num_vertices);


#endif

