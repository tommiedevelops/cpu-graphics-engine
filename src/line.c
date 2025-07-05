#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "line.h"
#include "constants.h"

int compute_num_pixels(int x0, int y0, int x1, int y1){
	return (int)fmax(abs(x1-x0),abs(y1-y0));
}

struct Pixel* draw_line(int x0, int y0, int x1, int y1) {
	/* returns a malloc'd array that needs to be freed */
	/* NOT BRESENHAM */
	int dx = x1 - x0;
	int dy = y1 - y0;

	int num_pixels = compute_num_pixels(x0,y0,x1,y1);

	struct Pixel* pixels = malloc(num_pixels*sizeof(struct Pixel));

	float step = fmax(abs(dx), abs(dy));
	if(step != 0){
		float stepX = (float)dx / step;
		float stepY = (float)dy / step;
		for(int i = 0; i < step; i++){
			pixels[i].x = round(x0+i*stepX);
			pixels[i].y = round(y0+i*stepY);
		}
	}
	return pixels;
}

/*Function reflects 2D pixels along the line y = x*/
void reflect_on_yx(struct Pixel* pixels, int num_pixels) {
	for(int i = 0; i < num_pixels; i++) {
		int temp = pixels[i].x;
		pixels[i].x = pixels[i].y;
		pixels[i].y = temp;
	}
}

/* Function reflects 2D pixels along x axis */
void reflect_on_x(struct Pixel* pixels, int num_pixels) {
	for(int i = 0; i < num_pixels; i++){
		pixels[i].x = -1 * pixels[i].x;
	}
}

/* Function reflects 2D pixels along y axis */
void reflect_on_y(struct Pixel* pixels, int num_pixels) {
	for(int i = 0; i < num_pixels; i++){
		pixels[i].y = -1 * pixels[i].y;
	}
}

void transform_from_2_to_target_octant(int target_octant, struct Pixel* pixels, int num_pixels) {
	switch(target_octant) {
		case 1:
			reflect_on_yx(pixels, num_pixels);
			break;
		case 2:
			break;
		case 3:
			reflect_on_y(pixels, num_pixels);
			break;
		case 4:
			reflect_on_y(pixels, num_pixels);
			reflect_on_yx(pixels, num_pixels);
			break;
		case 5:
			reflect_on_x(pixels, num_pixels);
			reflect_on_y(pixels, num_pixels);
			reflect_on_yx(pixels, num_pixels);
			break;
		case 6:
			reflect_on_x(pixels, num_pixels);
			reflect_on_y(pixels, num_pixels);
			break;
		case 7:
			reflect_on_x(pixels, num_pixels);
			break;
		case 8:
			reflect_on_x(pixels, num_pixels);
			reflect_on_yx(pixels, num_pixels);
			break;
		default:
			printf("line.c/transform_to_octant_2: invalid octant supplied: curr_octant={%d}\n",target_octant);
	}
}





