#include <stdio.h>
#include "headerfiles/constants.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

/**
 * Draws a line on the framebuffer using given start and end coordinates.
 *
 * @param framebuffer  A pointer to a WIDTH × HEIGHT pixel buffer (defined in constants.h)
 * @param line_start   An int[2] array representing the start coordinates (x, y) of the line
 * @param line_end     An int[2] array representing the end coordinates (x, y) of the line
 *
 * @return void
 *
 * @sideeffects Modifies the framebuffer by setting pixel values along the line path
 */

struct Point {
	int x;
	int y;
};

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
		if( (x > WIDTH) || (x < 0) ) {printf("invalid x value: %d\n", x); return;}
		if( (y > HEIGHT)||(y < 0)) {printf("invalid y value: %d\n", y); return;}
		framebuffer[x + WIDTH*y] = value;	
}

int identify_octant(int dx, int dy) {
	float m = dy/dx;
	if( dx>0 && dy>0 ) return (m>=1) ? 2 : 1;
	if( dx>0 && dy<0 ) return (m<=-1) ? 7 : 8;
	if( dx<0 && dy<0 ) return (m>=1) ? 6 : 5;
	if( dx<0 && dy>0 ) return (m<=-1) ? 3 : 4;
}

void draw_line(int* line_start, int* line_end, struct Point* resulting_points) {
	/* supports lines in first quadrant (dx>0, dy>0)*/
	if(resulting_points == NULL) {
		perror("provided struct Point* is null\n");
		return;
	}
	//TODO
}

void draw_line_general(uint32_t* framebuffer, int* line_start, int* line_end) {
	// null checks
	if((framebuffer == NULL) || (line_start == NULL) || (line_end == NULL)) {
		perror("line.c/draw_line_general: Parameter(s) are null\n");
		return;
	}

	// extract coordinates
	int x0 = line_start[0];
	int y0 = line_start[1];
	int x1 = line_end[0];
	int y1 = line_end[1];

	// compute decision paremeters
	int dx = x1 - x0;
	int dy = y1 - y0;
	float m = dy/dx;

	//determine number of pixels needed
	int num_pixels = (dx >= dy) ? dx : dy;

	// allocate memory for Points
	struct Point* points = (struct Point*)malloc(num_pixels * sizeof(struct Point));

	// identify the octant
	int octant = identify_octant(dx, dy);

	// draw the line in the first quadrant
	//TODO

	free(points);
}



