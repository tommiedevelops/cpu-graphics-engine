#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "compute_line.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
                if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. point= {%d,%d}\n", x,y); return;}
                if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. point= {%d,%d}\n", x,y); return;}

                framebuffer[x + WIDTH*y] = value;
}

void draw_points_to_framebuffer(struct Point* points, uint32_t *framebuffer, int num_points){
	bool error = false;

	if( (points == NULL) || (framebuffer == NULL) ) {
		perror("points or framebuffer is null");
		return;
	}

	for(int i = 0; i < num_points; i++){

		if( (points[i].x > WIDTH) || (points[i].x < 0) ) {
			printf("invalid x value. point={%d,%d}\n", points[i].x, points[i].y);
			error = true;
		}

		if( (points[i].y > HEIGHT) || (points[i].y < 0) ) {
			printf("invalid y value. point={%d,%d}\n", points[i].x, points[i].y);
			error = true;
		}
 		if(error) {return;}

		framebuffer[points[i].x + WIDTH * points[i].y] = COLOR_RED;
	}
}

int render_lines(uint32_t *framebuffer, int* coords, int num_coords) {
	// array organisation:
	// [[start_coord_x_0],[start_coord_y_0],[end_coord_x_0], [end_coord_y_0] ...]
	// where [start_coord_X_X] and [end_coord_X_X] are integers.
	for(int i = 0; i < num_coords; i++){
		int* coord = coords + 4*i;
		printf("drawing line: {%d,%d} -> {%d,%d}\n", *coord, *(coord+1),*(coord+2),*(coord+3));

		int x0 = *coord;
		int y0 = *(coord+1);
		int x1 = *(coord+2);
		int y1 = *(coord+3);

		struct Point* line = draw_line_easy(x0,y0,x1,y1);
		int num_points = compute_num_points(x0,y0,x1,y1);
		printf("render_lines:num_points={%d}\n", num_points);
		draw_points_to_framebuffer(line, framebuffer, num_points);
		free(line);
	}
	return 0;
}

/* Assuming wireframe_vertices are centred at (0,0) */
/* and structured like so: [x0,y0,z0,x1,y1,z1...]*/
void render_wireframe_from_3D_vertices(float* wireframe_vertices, int num_vertices){
	int centre_x = round(WIDTH / 2);
	int centre_y = round(HEIGHT / 2);

	//TODO
}






