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

void draw_depth_pixels_to_zbuffer(struct DepthPixelArray depth_pixels, uint32_t* zbuffer){
	int num_pixels = depth_pixels.num_depth_pixels; 
};

void draw_pixels_to_framebuffer(struct Pixel* pixels, uint32_t *framebuffer, uint32_t *zbuffer, int num_pixels, uint32_t color){
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

void render_triangles(uint32_t* framebuffer, uint32_t* zbuffer, struct Vec3f* vertices, int* triangles, int num_triangles){
	//TODO include zbuffer in rendering calculation
	// for each pixel on the screen, calculate the 'depth' of the pixel by interpolating the values in the triangle
	for(int i = 0; i < num_triangles; i++)	{

		// create a triangle
		struct Triangle tri = {
			.v0 = &vertices[triangles[3*i]],
			.v1 = &vertices[triangles[3*i+1]],
			.v2 = &vertices[triangles[3*i+2]]
		};

		// determine the color of the triangle
		uint8_t r, g, b, a;
		a = 255;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

		uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;

		// draw triangles
		rasterize_triangle(tri, framebuffer, zbuffer, color);
		
	}

}
