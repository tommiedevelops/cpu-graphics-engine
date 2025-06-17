#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "transform_line.h"
#include "compute_line.h"

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

void test_y_reflection_works_correctly(){
	printf("testing_y_reflection_works_correctly\n");
	int num_points = 10;
	struct Point* points = malloc(num_points*sizeof(struct Point));
	struct Point* expected_points = malloc(num_points*sizeof(struct Point));

	memset(points,0x0,sizeof(struct Point)*num_points);
	memset(expected_points,0x0,sizeof(struct Point)*num_points);

	// produce expected array
	for(int i = 0; i < num_points; i++){
		expected_points[i].x = i;
		expected_points[i].y = -i;
	}


	// start off with y = x line.
	for(int i = 0; i < num_points; i++){
		points[i].x = i;
		points[i].y = i;
	}

	// apply y-axis reflection
	reflect_on_y(points, num_points);

	// assert equal
	for(int i = 0; i < num_points; i++){
		assert(PointsAreEqual(points[i], expected_points[i]));
	}

	free(expected_points);
	free(points);
	printf("success\n");
}


/* THIS FUNCTION RELIES ON BEING ABLE TO ACCURATELY DRAW LINES IN 1ST QUADRANT (WHICH I HAVENT TESTED) */

void test_yx_reflection_works_correctly(){
	printf("testing_yx_reflection_works_correctly\n");
	int num_points = 10;
	struct Point* points = malloc(num_points*sizeof(struct Point));
	struct Point* expected_points = malloc(num_points*sizeof(struct Point));

	memset(points,0x0,sizeof(struct Point)*num_points);
	memset(expected_points,0x0,sizeof(struct Point)*num_points);

	// TODO

	free(expected_points);
	free(points);
	printf("NOT IMPLEMENTED\n");
}


void test_x_reflection_works_correctly(){
	printf("testing_x_reflection_works_correctly\n");
	int num_points = 10;
	struct Point* points = malloc(num_points*sizeof(struct Point));
	struct Point* expected_points = malloc(num_points*sizeof(struct Point));

	memset(points,0x0,sizeof(struct Point)*num_points);
	memset(expected_points,0x0,sizeof(struct Point)*num_points);

	// produce expected array
	for(int i = 0; i < num_points; i++){
		expected_points[i].x = -i;
		expected_points[i].y = i;
	}

	// start off with y = x line.
	for(int i = 0; i < num_points; i++){
		points[i].x = i;
		points[i].y = i;
	}

	// apply y-axis reflection
	reflect_on_x(points, num_points);

	// assert equal
	for(int i = 0; i < num_points; i++){
		assert(PointsAreEqual(points[i], expected_points[i]));
	}

	free(expected_points);
	free(points);
	printf("success\n");
}
