#include <stdio.h>
#include "constants.h"
#include <stdint.h>

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

void draw_line(uint32_t* framebuffer, int* line_start, int* line_end){
	// null checks
	if((framebuffer == NULL) || (line_start == NULL) || (line_end == NULL)) {
		printf("Parameter(s) are null\n");
		return;
	}

	// extract coordinates
	int start_x = line_start[0];
	int start_y = line_start[1];
	int end_x = line_end[0];
	int end_y = line_end[1];

	// draw the line
	framebuffer[start_x + WIDTH * start_y] = COLOR_RED;
	framebuffer[end_x + WIDTH * end_y] = COLOR_RED;

	int h_diff = end_x - start_x;
	int v_diff = end_y - start_y;
	float grad = v_diff / h_diff;

	for(int i = 0; i < h_diff; i++){
		framebuffer[(start_x + i) + WIDTH * (start_y + i)] = COLOR_RED;
	}
}

