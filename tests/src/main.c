#include <stdio.h>
#include "obj_parser_tests.h"
#include "vertex_tests.h"
#include "triangle_tests.h"

void triangle_tests(){
	test_create_triangle();
	test_sort_vertices_by_y();
	test_sort_vertices_by_x();
	test_sort_vertices_by_y_degenerate();
}

void vertex_tests(){
	test_get_bounds();
	test_shift_to_origin();
	test_scale_vertex();
	test_translate_vertex();
	test_normalize_lengths();
	test_scale_lengths();
}

int main(){
	printf("Running unit tests...\n");
	// triangle.c tests
	triangle_tests();
}

