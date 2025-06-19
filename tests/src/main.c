#include <stdio.h>
#include "obj_parser_tests.h"
#include "vertex_tests.h"

int main(){
	printf("Running unit tests...\n");
	//obj_parser_tests
	test_get_bounds();
	test_shift_to_origin();
	test_normalize_lengths();
//	test_scale_lengths();
}
