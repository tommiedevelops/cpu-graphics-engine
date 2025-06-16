#ifndef LINE_H
#define LINE_H

#include <stdint.h>

void draw_line_general(uint32_t* framebuffer, int* line_start, int* line_end);
int identify_octant(int dx, int dy);

#endif
