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
		if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. point= {%d,%d}\n", x,y); return;}
		if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. point= {%d,%d}\n", x,y); return;}

		framebuffer[x + WIDTH*y] = value;
}

int identify_octant(int dx, int dy) {
	float m = dy/dx;
	if( dx>0 && dy>0 ) return (m>=1) ? 2 : 1;
	if( dx>0 && dy<0 ) return (m<=-1) ? 7 : 8;
	if( dx<0 && dy<0 ) return (m>=1) ? 6 : 5;
	if( dx<0 && dy>0 ) return (m<=-1) ? 3 : 4;
}


// There is a bug in this function causing exactly 2 pixels to not be rendered.
// I can't be bothered figuring it out right now but I'll leave the debug code in
// for future reference.
void draw_line(int* line_start, int* line_end, struct Point* resulting_points) {
	/* supports lines in second octant only (dx>0,dy>0,m>1)*/
	if(resulting_points == NULL) {
		perror("provided struct Point* is null\n");
		return;
	}

	int dx = line_end[0] - line_start[0];
	int dy = line_end[1] - line_start[1];
	float m = dy/dx;

	int points_index = 0;
	int y_from = line_start[1];


	for(int i = 0; i < dx; i++){
		int x = line_start[0] + i;
		int y_to = line_start[1] + i*m;

		for(int y = y_from; y < y_to; y++){
			struct Point p = {x,y};
			resulting_points[points_index] = p;
			printf("draw_line:point={%d,%d}\n",x,y);
			//printf("draw_line:points_index={%d}\n", points_index);
			y_from = y_to;
			points_index++;
		}

	}
}


/*Function reflects 2D points along the line y = x*/
void reflect_on_yx(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++) {
		int temp = points[i].x;
		points[i].x = points[i].y;
		points[i].y = temp;
	}
}

/* Function reflects 2D points along x axis */
void reflect_on_x(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++){
		points[i].x = -1 * points[i].x;
	}
}

/* Function reflects 2D points along y axis */
void reflect_on_y(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++){
		points[i].y = -1 * points[i].y;
	}
}

void transform_to_octant_2(int curr_octant, struct Point* points, int num_points) {
	switch(curr_octant) {
		case 1:
			reflect_on_yx(points, num_points);
			break;
		case 2:
			break;
		case 3:
			reflect_on_y(points, num_points);
			break;
		case 4:
			reflect_on_y(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		case 5:
			reflect_on_x(points, num_points);
			reflect_on_y(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		case 6:
			reflect_on_x(points, num_points);
			reflect_on_y(points, num_points);
			break;
		case 7:
			reflect_on_x(points, num_points);
			break;
		case 8:
			reflect_on_x(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		default:
			printf("line.c/transform_to_octant_2: invalid octant supplied: curr_octant={%d}\n",curr_octant);
	}
}


void transform_vars_to_octant_2(int* dx,int* dy){
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

	// draw the line in the 2nd quadrant
	draw_line(line_start, line_end, points);

	// transform points to correct octant
	// transform_to_octant_2(octant, points, num_pixels);

	// draw final line to the framebuffer
	for(int i = 0; i < num_pixels; i++){
		struct Point p = points[i];
		//printf("draw_line_general:placing point{%d,%d} into the framebuffer...\n",points[i].x, points[i].y);
		place_pixel(p.x, p.y, COLOR_RED, framebuffer);
	}

	free(points);
}



