#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compute_line.h"
#include "transform_line.h"

void test_can_draw_simple_horizontal_line(){

	/* In this test, we draw a line from (10,10)->(19,10) */

	int num_pixels = 10;

	struct Point* points = malloc(sizeof(struct Point) * num_pixels);
	struct Point* expected_points = malloc(sizeof(struct Point) * num_pixels);

	int start_coords[2] = {10,10}; int end_coords[2] = {19,10};
	// produce expected array
	for(int i = 0; i < num_pixels; i++){
		expected_points[i].x = 10+i;
		expected_points[i].y = 10;
	}

	// produce actual array
	draw_line(start_coords, end_coords, points);

	// assert
	for(int i = 0; i < num_pixels; i++){
		assert(PointsAreEqual(points[i],expected_points[i]));
	}
	
	free(points);
	free(expected_points);

}

void test_can_draw_simple_vertical_line(){

	/* In this test, we draw a line from (10,10)->(10,19) */

	int num_pixels = 10;

	struct Point* points = malloc(sizeof(struct Point) * num_pixels);
	struct Point* expected_points = malloc(sizeof(struct Point) * num_pixels);

	int start_coords[2] = {10,10};
	int end_coords[2] = {10,19};

	// produce expected array
	for(int i = 0; i < num_pixels; i++){
		expected_points[i].x = 10;
		expected_points[i].y = 10+i;
	}

	// produce actual array
	draw_line(start_coords, end_coords, points);

	// assert
	for(int i = 0; i < num_pixels; i++){
		assert(PointsAreEqual(points[i],expected_points[i]));
	}

	free(points);
	free(expected_points);

}

void test_can_draw_yx(){

	/* In this test, we draw a line from (0,10)->(9,10) */

	int num_pixels = 5;

	struct Point* points = malloc(sizeof(struct Point) * num_pixels);
	struct Point* expected_points = malloc(sizeof(struct Point) * num_pixels);


	int start_coords[2] = {10,10};
	int end_coords[2] = {14,14};

	// produce expected array
	for(int i = 0; i < num_pixels; i++){
		expected_points[i].x = 10+i;
		expected_points[i].y = 10+i;
	}

	// produce actual array
	draw_line(start_coords, end_coords, points);

	// assert
	for(int i = 0; i < num_pixels; i++){
		assert(PointsAreEqual(points[i],expected_points[i]));
	}

		
	free(points);
	free(expected_points);

}


void test_can_draw_minus_yx(){

	/* In this test, we draw a line from (0,10)->(9,10) */

	int num_pixels = 5;

	struct Point* points = malloc(sizeof(struct Point) * num_pixels);
	struct Point* expected_points = malloc(sizeof(struct Point) * num_pixels);


	int start_coords[2] = {14,14};
	int end_coords[2] = {10,10};

	// produce expected array
	for(int i = 0; i < num_pixels; i++){
		expected_points[i].x = 14-i;
		expected_points[i].y = 14-i;
	}

	// produce actual array
	draw_line(start_coords, end_coords, points);

	// assert
	for(int i = 0; i < num_pixels; i++){
		assert(PointsAreEqual(points[i],expected_points[i]));
	}

		
	free(points);
	free(expected_points);

}


void test_can_draw_y2x(){

	// TODO

	int num_pixels = 6;

	struct Point* points = malloc(sizeof(struct Point) * num_pixels);
	struct Point* expected_points = malloc(sizeof(struct Point) * num_pixels);	

	int start_coords[2] = {10,10};
	int end_coords[2] = {15,20};

	// produce actual array
	draw_line(start_coords, end_coords, points);

	// assert (just asserting it has been filled for now)
	
	free(points);
	free(expected_points);

}
