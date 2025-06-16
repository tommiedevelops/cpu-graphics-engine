#include <stdio.h>
#include "line_tests.h"
#include "line_transform_tests.h"

int main(){
	printf("Running unit tests...\n");
        test_y_reflection_works_correctly();
        test_x_reflection_works_correctly();
        test_identify_octant_simple_positive();
}
