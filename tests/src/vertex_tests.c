#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vertex.h"

void test_get_bounds(){
	printf("test_get_bounds\n");
	struct Vertex vertices[] = {
		{.x = -5.0f, .y = 3.0f, .z = 10.0f},
		{.x = 50.0f, .y = -10.0f, .z = 4.0f},
		{.x = 13.0f, .y = 27.0f, .z = 0.0f}
	};

	int num_vertices = 3;

	struct Bounds expected = {
		.xmin = -5.0f, .xmax = 50.0f,
		.ymin = -10.0f,.ymax = 27.0f,
		.zmin = 0.0f,  .zmax = 10.0f
	};

	struct Bounds actual = get_bounds(vertices, num_vertices);

	for(int i = 0; i < 6; i++){
		assert(bounds_are_equal(actual, expected));
	}

	printf("success\n");
}

void test_normalize_lengths(){
	printf("test_normalize_lengths\n");

	struct Vertex vertices[] = {
			{.x=0.0f,.y=0.0f,.z=0.0f},
			{.x=0.6f,.y=0.6f,.z=0.6f},
			{.x=0.3f,.y=0.3f,.z=0.3f}
	};

	int num_vertices = 3;

	struct Bounds bounds = get_bounds(vertices,num_vertices);

	struct Vertex expected[] = {
			{.x=-1.0f, .y=-1.0f, .z=-1.0f},
			{.x=1.0f, .y=1.0f, .z=1.0f},
			{.x=0.0f, .y=0.0f, .z=0.0f}
	};

	normalize_lengths(bounds, vertices, num_vertices);

	for(int i = 0; i < num_vertices;i++){
		printf("testing expected:{%f,%f,%f} == actual:{%f,%f,%f}\n",
			expected[i].x, expected[i].y, expected[i].z,
			vertices[i].x,vertices[i].y,vertices[i].z
		);
		assert(vertices_are_equal(expected[i], vertices[i]));
	}
	printf("success\n");
}

void test_shift_to_origin() {
	printf("test_shift_to_origin\n");
	struct Vertex vertices[] = {
		{.x=-5.0f, .y=10.0f, .z=3.0f},
		{.x=10.0f, .y=5.0f, .z=-3.0f}
	};

	int num_vertices = 2;
	struct Bounds bounds = get_bounds(vertices,num_vertices);

	shift_to_origin(bounds, vertices, num_vertices);

	struct Vertex expected[] = {
		{.x=0.0f, .y=5.0f, .z=6.0f},
		{.x=15.0f, .y=0.0f, .z=0.0f}
	};
	for(int i = 0; i < num_vertices; i++){
		assert(vertices_are_equal(vertices[i],expected[i]));
	}
	printf("success\n");
}

void test_scale_lengths() {
	printf("test_scale_lengths\n");
	struct Vertex vertices[] = {
			{.x=-1.0f, .y=-1.0f, .z=-1.0f},
			{.x=1.0f, .y=1.0f, .z=1.0f},
			{.x=0.0f, .y=0.0f, .z=0.0f}
	};

	int num_vertices = 3;

	struct Bounds bounds = get_bounds(vertices,num_vertices);
	const float scale = 200.0f;

	scale_lengths(scale, bounds, vertices, num_vertices);

	struct Vertex expected[] = {
			{.x=-100.0f, .y=-100.0f, .z=-100.0f},
			{.x=100.0f, .y=100.0f, .z=100.0f},
			{.x=0.0f, .y=0.0f, .z=0.0f}
	};

	for(int i = 0; i < 3*num_vertices; i++){
		assert(vertices_are_equal(expected[i], vertices[i]));
	}

	printf("success\n");
}

