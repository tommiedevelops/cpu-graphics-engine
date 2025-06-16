#include <stdio.h>
#include "constants.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "line_transform.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
		if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. point= {%d,%d}\n", x,y); return;}
		if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. point= {%d,%d}\n", x,y); return;}

		framebuffer[x + WIDTH*y] = value;
}

// There is a bug in this function causing exactly 2 pixels to not be rendered.
// I can't be bothered figuring it out right now but I'll leave the debug code in
// for future reference.
void draw_line(int dx, int dy, struct Point* resulting_points) {
	/* supports lines in second octant only (dx>0,dy>0,m>1)*/	
	/* function produces Point* starting at (0,0)->(dx,dx) and only in the 2nd octant */
	if(resulting_points == NULL) {
		perror("provided struct Point* is null\n");
		return;
	}

	float m = fmax(dy/dx, dx/dy);

	int points_index = 0;
	int y_from = 0;

	for(int x = 0; x < dx; x++){
		int y_to = x*m;

		for(int y = y_from; y < y_to; y++){
			struct Point p = {x,y};
			resulting_points[points_index] = p;
			//printf("draw_line:point={%d,%d}\n",x,y);
			//printf("draw_line:points_index={%d}\n", points_index);
			y_from = y_to;
			points_index++;
		}

	}
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

	//determine number of pixels needed
	int num_pixels = (dx >= dy) ? abs(dx) : abs(dy);
	//printf("draw_line_general:num_pixels={%d}\n", num_pixels);

	// allocate memory for Points
	struct Point* points = (struct Point*)malloc(num_pixels * sizeof(struct Point));

	// identify the octant
	int octant = identify_octant(dx, dy);

	// draw line in 2nd quadrant & from (0,0) -> (dx,dy)
	draw_line(abs(dx), abs(dy), points);

	// transform points from correct octant
	transform_from_octant_2(octant, points, num_pixels);

	printf("line.c/draw_line_general: point values after reflections\n");
	for(int i =0; i < num_pixels; i++) {printf("point={%d,%d}\n", points[i].x, points[i].y);}

	// translate points to correct position
	for(int i = 0; i < num_pixels; i++){
		points[i].x += x0;
		points[i].y += y0;
	}


	printf("line.c/draw_line_general: point values after translations\n");
	for(int i =0; i < num_pixels; i++) {printf("point={%d,%d}\n", points[i].x, points[i].y);}

	// draw final line to the framebuffer
	for(int i = 0; i < num_pixels; i++){
		struct Point p = points[i];
		//printf("draw_line_general:placing point{%d,%d} into the framebuffer...\n",points[i].x, points[i].y);
		place_pixel(p.x, p.y, COLOR_RED, framebuffer);
	}

	free(points);
}




