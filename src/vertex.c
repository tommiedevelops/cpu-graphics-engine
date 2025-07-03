#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vertex.h"
#include "bounds.h"

// single vertex methods
void scale_vertex(struct Vertex* vertex, float scalar){
	// accepts struct Vertex ptr, not array

	if(vertex == NULL) {
		perror("src/vertex.c/scale_vertex: vertex was null");
		exit(EXIT_FAILURE);
	}

	vertex->x *= scalar;
	vertex->y *= scalar;
	vertex->z *= scalar;
}

void translate_vertex(struct Vertex* vertex, float dx, float dy, float dz){
	// accepts struct Vertex ptr, not array

	if(vertex == NULL) {
		perror("src/vertex.c/translate_vertex: vertex was null");
		exit(EXIT_FAILURE);
	}

	vertex->x += dx;
	vertex->y += dy;
	vertex->z += dz;
}

bool vertices_are_equal(struct Vertex a, struct Vertex b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	return true;
}

void convert_vertex_to_int_values(struct Vertex* vertex){
	if(vertex == NULL) {
		perror("src/vertex.c/convert_vertex_to_int_values: vertex was null");
		exit(EXIT_FAILURE);
	}
	vertex->x = round(vertex->x);
	vertex->y = round(vertex->y);
	vertex->z = round(vertex->z);
}

/* vertex array methods */

/* Shifts coordinates on all three axes to [0, imax - imin] */
static void shift_to_origin(struct Bounds bounds, struct Vertex* vertices, int num_vertices) {
        for(int i = 0; i < num_vertices; i++){
                vertices[i].x -= bounds.xmin;
                vertices[i].y -= bounds.ymin;
                vertices[i].z -= bounds.zmin;
        }
}

/* Normalizes values to between [-1,1] - assumes min = 0 for all axes */
static void normalize_lengths(struct Bounds bounds, struct Vertex* vertices, int num_vertices) {

        // normalizes between [0,1]
        for(int i = 0; i < num_vertices; i++){
                vertices[i].x = (float)vertices[i].x / bounds.xmax;
                vertices[i].y = (float)vertices[i].y / bounds.ymax;
                vertices[i].z = (float)vertices[i].z / bounds.zmax;
        }

        // scale and translate to [-1,1]
        for(int i = 0; i < num_vertices; i++){
                scale_vertex(vertices + i, 2);
                translate_vertex(vertices + i, -1.0f,-1.0f,-1.0f);
        }
}

/* Takes normalized vertices between [-1,1] and scales them to [-target_length/2, target_length/2] */
static void scale_lengths(float target_length, struct Bounds bounds, struct Vertex* vertices, int num_vertices){
        for(int i = 0; i < num_vertices; i++){
                scale_vertex(vertices + i, (float)target_length/2.0f);
        }
}

/*
   normalize vertices to fit in box, side length s that is centered
   on the origin all axes scaled equally.
*/
void normalize_vertices(
			float sidelength,
			struct Vertex* vertices,
			int num_vertices
){
        struct Bounds bounds = get_bounds(vertices, num_vertices);

        shift_to_origin(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        normalize_lengths(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        scale_lengths(sidelength, bounds, vertices, num_vertices);
}
