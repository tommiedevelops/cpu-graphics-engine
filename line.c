#include <stdio.h>
#include "constants.h"
#include <stdint.h>

// input: pixel buffer
// output: void
// sideffects: modifies pixel buffer to draw required line. dimensions contained in constants.h

void draw_line(uint32_t* framebuffer){
	printf("Drawing a line\n");
	framebuffer[100 + 100 * WIDTH] = COLOR_RED;
}
