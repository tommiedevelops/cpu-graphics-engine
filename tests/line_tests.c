#include <stdio.h>
#include "../src/headerfiles/line.h"
#include <assert.h>


void test_identify_octant_simple_positive() {
	int dx, dy;

	printf("test_identify_octant_simple_positive\n");
	// 1st octant
	dx = 2; dy = 1;
	assert(identify_octant(dx,dy) == 1);
	// 2nd octant
	dx = 1; dy = 2;
	assert(identify_octant(dx,dy) == 2);
	// 3rd octant
	dx = -1; dy = 2;
	assert(identify_octant(dx,dy) == 3);
	// 4th octant
	dx = -2; dy = 1;
	assert(identify_octant(dx,dy) == 4);
	// 5th octant
	dx = -2; dy = -1;
	assert(identify_octant(dx,dy) == 5);
	//6th octant
	dx = -1; dy = -2;
	assert(identify_octant(dx,dy) == 6);
	//7th octant
	dx = 1; dy = -2;
	assert(identify_octant(dx,dy) == 7);
	//8th octant
	dx = 2; dy = -1;
	assert(identify_octant(dx,dy) == 8);
	printf("success\n");
}

int main(){
	printf("Running unit tests...\n");
	test_identify_octant_simple_positive();
}
