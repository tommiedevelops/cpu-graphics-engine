#include <stdio.h>
#include "bounds_tests.h"

void bounds_tests(){
	test_get_bounds_from_tri();	
}

int main(){
	printf("Running unit tests...\n");
	bounds_tests();
}

