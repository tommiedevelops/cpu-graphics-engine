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

void test_normalize_vertices(){
	//TODO
}

void test_shift_to_origin() {
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

void tests_scale_lengths() {
	//TODO
}
