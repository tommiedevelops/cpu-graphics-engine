#include <stdio.h>
#include "line.h"
#include "line_transform.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void test_can_draw_simple_horizontal_line(){

	/* In this test, we draw a line from (0,10)->(9,10) */

	// TODO
	int num_pixels = 10;
	struct Point points[num_pixels] = {0};
	struct Point expected_points[num_pixels] = {0};

	int start_coords[2] = {0,10};
	int end_coords[2] = {9,10};
	
	// produce expected array
	for(int i = 0; i < num_pixels; i++){
		expected_points[i].x = i;
		expected_points[i].y = 10;
	}

	// produce actual array
	draw_line(start_coords, end_coords, points);

	for(int i = 0; i < num_pixels; i++){
		// assert the pixels are the same
	}

}

