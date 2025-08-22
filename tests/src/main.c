#include <stdio.h>
#include "bounds_tests.h"
#include "vec3f_tests.h"
void bounds_tests(){
	test_get_bounds_from_tri();	
}

int main(){
	printf("Running unit tests...\n");
	//bounds_tests();
	test_vec3f_cross();
}

