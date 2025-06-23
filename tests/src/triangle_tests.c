#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "triangle.h"
#include "vertex.h"

void test_create_triangle(){
	printf("test_create_triangle\n");
	struct Vertex a = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
	struct Vertex b = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	struct Vertex c = {.x = 0.0f, .y = 0.0f, .z = 1.0f};

	struct Triangle actual = create_triangle(&a,&b,&c);

	assert( (actual.a != NULL) && (actual.b != NULL) && (actual.c != NULL) );

	// check if you modify original vertex, that it is updated correctly in the triangle
	b.y = 10.0f;
	assert(actual.b->y == 10.0f);

	printf("success\n");
}

void test_sort_vertices_by_y(){
	printf("test_sort_vertices_by_y\n");
	struct Vertex a = {.x = 1.0f, .y = 1.0f, .z = 0.0f};
	struct Vertex b = {.x = 0.0f, .y = 2.0f, .z = 0.0f};
	struct Vertex c = {.x = 0.0f, .y = -1.0f, .z = 1.0f};

	struct Triangle tri = create_triangle(&a,&b,&c);

	struct Vertex** sorted_by_y = sort_vertices_by_y_asc(tri);
	printf("sorted_by_y = [%f, %f, %f]\n", sorted_by_y[0]->y, sorted_by_y[1]->y, sorted_by_y[2]->y);
	assert(sorted_by_y != NULL);
	assert(sorted_by_y[0]->y == -1.0);
	assert(sorted_by_y[1]->y == 1.0f);
	assert(sorted_by_y[2]->y == 2.0f);
	free(sorted_by_y);
	printf("success\n");
}

void test_sort_vertices_by_x(){
	printf("test_sort_vertices_by_x\n");
	struct Vertex a = {.x = 3.0f, .y = 1.0f, .z = 0.0f};
	struct Vertex b = {.x = -6.0f, .y = 2.0f, .z = 0.0f};
	struct Vertex c = {.x = 5.0f, .y = -1.0f, .z = 1.0f};

	struct Triangle tri = create_triangle(&a,&b,&c);

	struct Vertex** sorted_by_x = sort_vertices_by_x_asc(tri);
	printf("sorted_by_x = [%f, %f, %f]\n", sorted_by_x[0]->x, sorted_by_x[1]->x, sorted_by_x[2]->x);
	assert(sorted_by_x != NULL);
	assert(sorted_by_x[0]->x == -6.0);
	assert(sorted_by_x[1]->x == 3.0f);
	assert(sorted_by_x[2]->x == 5.0f);
	free(sorted_by_x);
	printf("success\n");
}
