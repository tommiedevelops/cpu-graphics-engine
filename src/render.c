#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "line.h"
#include "vec3f.h"
#include "triangle.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
                if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. pixel= {%d,%d}\n", x,y); return;}
                if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. pixel= {%d,%d}\n", x,y); return;}

                framebuffer[x + WIDTH*y] = value;
}

void draw_pixels_to_framebuffer(struct Pixel* pixels, uint32_t *framebuffer, int num_pixels, uint32_t color){
	bool error = false;

	if( (pixels == NULL) || (framebuffer == NULL) ) {
		perror("pixels or framebuffer is null");
		return;
	}

	for(int i = 0; i < num_pixels; i++){

		if( (pixels[i].x > WIDTH) || (pixels[i].x < 0) ) {
			printf("invalid x value. pixel={%d,%d}\n", pixels[i].x, pixels[i].y);
			error = true;
		}

		if( (pixels[i].y > HEIGHT) || (pixels[i].y < 0) ) {
			printf("invalid y value. pixel={%d,%d}\n", pixels[i].x, pixels[i].y);
			error = true;
		}
 		if(error) {return;}

		framebuffer[pixels[i].x + WIDTH * pixels[i].y] = color;
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

		struct Pixel* line = draw_line(x0,y0,x1,y1);
		int num_pixels = compute_num_pixels(x0,y0,x1,y1);
		printf("render_lines:num_pixels={%d}\n", num_pixels);
		draw_pixels_to_framebuffer(line, framebuffer, num_pixels, COLOR_RED);
		free(line);
	}
	return 0;
}

/* Assuming wireframe_vertices are centred at (0,0) */
/* and structured like so: [x0,y0,z0,x1,y1,z1...]*/
void render_wireframe(uint32_t* framebuffer,struct Triangle* triangles, int num_triangles){

	if(framebuffer == NULL){
		perror("src/render.c/render_wireframe: uin32_t* is NULL");
		exit(EXIT_FAILURE);
	}
	if(triangles == NULL) {
		perror("src/render.c/render_wireframe: struct Triangle* is NULL");
		exit(EXIT_FAILURE);
	}

	//TODO Re-write this function once Camera system is implemented
}

void render_triangles(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles){
	for(int i = 0; i < num_triangles; i++){

		// select a random color
		uint8_t r, g, b, a;
		a = 255;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

		uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;

		// draw triangles
		struct PixelArray triangle_pixels = rasterize_triangle(triangles[i]);
		draw_pixels_to_framebuffer(triangle_pixels.pixels, framebuffer, triangle_pixels.num_pixels, color); 

		// free resources
		destroy_pixel_array(triangle_pixels);
	}

}
