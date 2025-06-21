#include <stdio.h>
#include "obj_parser_tests.h"
#include "vertex_tests.h"

int main(){
	printf("Running unit tests...\n");
	//vertex.c tests
	test_get_bounds();
	test_shift_to_origin();
	test_scale_vertex();
	test_translate_vertex();
	test_normalize_lengths();
	test_scale_lengths();
}
