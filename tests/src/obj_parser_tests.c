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
