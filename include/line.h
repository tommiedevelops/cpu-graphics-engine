#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>
#include "pixel.h"

int compute_num_pixels(int x0, int y0, int x1, int y1);
struct Pixel* draw_line(int x0, int y0, int x1, int y1);

int identify_octant(int dx, int dy);
void reflect_on_x(struct Pixel* pixels, int num_pixels);
void reflect_on_yx(struct Pixel* pixels, int num_pixels);
void transform_from_octant_2(int curr_octant, struct Pixel* pixels, int num_pixels);
void reflect_on_y(struct Pixel* pixels, int num_pixels);
#endif
