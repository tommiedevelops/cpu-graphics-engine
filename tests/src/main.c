#include <stdio.h>
#include "compute_line_tests.h"
#include "transform_line_tests.h"

int main(){
	printf("Running unit tests...\n");
        test_y_reflection_works_correctly();
        test_x_reflection_works_correctly();
        test_identify_octant_simple_positive();
	// compute_line_tests
        test_can_draw_simple_horizontal_line();
        test_can_draw_simple_vertical_line();
        test_can_draw_yx();
        test_can_draw_minus_yx();
        test_can_draw_y2x();
}
