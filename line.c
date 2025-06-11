#include <stdio.h>
#include "headerfiles/constants.h"
#include <stdint.h>
#include <math.h>

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

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
		if( (x > WIDTH) || (x < 0) ) {printf("invalid x value: %d\n", x); return;}
		if( (y > HEIGHT)||(y < 0)) {printf("invalid y value: %d\n", y); return;}
		framebuffer[x + WIDTH*y] = value;	
}

void draw_line(uint32_t* framebuffer, int* line_start, int* line_end){

	// null checks
	if((framebuffer == NULL) || (line_start == NULL) || (line_end == NULL)) {
		printf("Parameter(s) are null\n");
		return;
	}

	// extract coordinates
	int x0 = line_start[0];
	int y0 = line_start[1];
	int x1 = line_end[0];
	int y1 = line_end[1];

	// draw the line

	int dx = x1 - x0;
	int dy = y1 - y0;

	if(dx == 0) {
		printf("Division by 0 attempted\n");
		return;
	}

	float m = dy/dx;

	int curr_y = y0;
	for(int i = 0; i < dx+1; i++){
		for(int yval = curr_y; yval < y0+i*m; yval++){
			place_pixel(x0 + i, round(yval), COLOR_RED, framebuffer);
		}
		curr_y = y0+i*m;
	}

}

