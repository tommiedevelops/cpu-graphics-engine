#include <assert.h>
#include <stdio.h>
#include "obj_parser.h"
#include <stdlib.h>

void test_get_bounds(){
	float test_vertices[] = {
		-5.0f, 3.0f, 10.0f,
		50.0f, -10.0f, 4.0f,
		13.0f, 27.0f, 0.0f
	};

	int num_vertices = 3;

	float expected_result[] = {-5.0f, 50.0f, -10.0f, 27.0f, 0.0f, 10.0f };

	float* result = get_bounds(test_vertices, num_vertices);

	for(int i = 0; i < 6; i++){
		printf("test_get_bounds: testing {%f} == {%f}\n", expected_result[i], result[i]);
		assert(expected_result[i] == result[i]);
	}
	printf("success\n");
	free(result);
}

void test_normalize_lengths(){
	printf("test_normalize_lengths\n");

	float test_vertices[] = {
			0.0f,0.0f,0.0f,
			0.6f,0.6f,0.6f,
			0.3f,0.3f,0.3f,
	};

	int num_vertices = 3;

	float* bounds = get_bounds(test_vertices,num_vertices);

	float expected_vertices[] = {
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f
	};

	normalize_lengths(bounds, test_vertices, num_vertices);

	for(int i = 0; i < 3*num_vertices;i++){
		printf("testing {%f}=={%f}\n", test_vertices[i], expected_vertices[i]);
		assert(test_vertices[i] == expected_vertices[i]);
	}
	printf("success\n");
	free(bounds);
}

void test_shift_to_origin() {
	printf("test_shift_to_origin\n");
	float test_vertices[] = {
		-5.0f, 10.0f, 3.0f,
		10.0f, 5.0f, -3.0f
	};

	int num_vertices = 2;
	float* bounds = get_bounds(test_vertices,num_vertices);

	shift_to_origin(bounds, test_vertices, num_vertices);

	float expected[] = {
		0.0f, 5.0f, 6.0f,
		15.0f, 0.0f, 0.0f
	};
	for(int i = 0; i < 6; i++){
		printf("testing actual={%f} == expected={%f}\n", test_vertices[i], expected[i]);
		assert(test_vertices[i] == expected[i]);
	}
	printf("success\n");
	free(bounds);
}

void test_scale_lengths() {
	printf("test_scale_lengths\n");
	float test_vertices[] = {
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f
	};

	int num_vertices = 3;

	float* bounds = get_bounds(test_vertices,num_vertices);
	const float scale = 200.0f;

	scale_lengths(scale,bounds, test_vertices, num_vertices);

	float expected[] = {
			-100.0f, -100.0f, -100.0f,
			100.0f, 100.0f, 100.0f,
			0.0f, 0.0f, 0.0f
	};

	for(int i = 0; i < 3*num_vertices; i++){
		printf("testing actual={%f} == expected={%f}\n", test_vertices[i], expected[i]);
		assert(test_vertices[i] == expected[i]);
	}

	printf("success\n");
	free(bounds);
}

