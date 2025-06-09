#include <stdio.h>
#include "constants.h"
#include <stdint.h>

// input: pixel buffer
// output: void
// sideffects: modifies pixel buffer to draw required line. dimensions contained in constants.h

// params:
// framebuffer - a WIDTH x HEIGHT length array (WIDTH and HEIGHT defined in constants.h)
// line_start - a int[2] representing the start coordinates of the line
// line_end - a int[2] representing the end coordinates of the line

void draw_line(uint32_t* framebuffer, int* line_start, int* line_end){
	// null checks
	if(framebuffer == NULL) {
		printf("Framebuffer is null\n");
		return;
	}
	
	if(line_start == NULL) {
		printf("line_start is null\n");
		return;
	}	

	if(line_end == NULL) {
		printf("line_end is null\n");
		return;
	}

	framebuffer[100 + 100 * WIDTH] = COLOR_RED;
}

