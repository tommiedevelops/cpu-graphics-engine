#ifndef COMPUTE_LINE_H
#define COMPUTE_LINE_H

#include <stdint.h>
#include <stdbool.h>

struct Point {
	int x;
	int y;
};

int compute_num_points(int x0, int y0, int x1, int y1);
struct Point* draw_line_easy(int x0, int y0, int x1, int y1);
void draw_line_general(uint32_t* framebuffer, int* line_start, int* line_end);
int identify_octant(int dx, int dy);
bool PointsAreEqual(struct Point a, struct Point b);

#endif
