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

/* Assuming wireframe_vertices are centred at (0,0) */
/* and structured like so: [x0,y0,z0,x1,y1,z1...]*/
void render_wireframe(uint32_t* framebuffer,struct Triangle* triangles, int num_triangles){

	if(framebuffer == NULL){
		perror("src/render.c/render_wireframe: uin32_t* is NULL");
		exit(EXIT_FAILURE);
	} if(triangles == NULL) {
		perror("src/render.c/render_wireframe: struct Triangle* is NULL");
		exit(EXIT_FAILURE);
	}

	//TODO Re-write this function once Camera system is implemented
}

void render_triangles(uint32_t* framebuffer, uint32_t* zbuffer, struct Vec3f* vertices, int* triangles, int num_triangles){
	//TODO include zbuffer in rendering calculation
	// for each pixel on the screen, calculate the 'depth' of the pixel by interpolating the values in the triangle
	for(int i = 0; i < num_triangles; i++)	{

		// create a triangle
		struct Triangle tri = {
			.a = &vertices[triangles[3*i]],
			.b = &vertices[triangles[3*i+1]],
			.c = &vertices[triangles[3*i+2]]
		};

		// determine the color of the triangle
		uint8_t r, g, b, a;
		a = 255;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

		uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;

		// draw triangles
		struct PixelArray triangle_pixels = rasterize_triangle(tri);
		struct DepthPixelArray triangle_depth_pixels = rasterize_triangle_depths(tri);
		
		draw_depth_pixels_to_zbuffer(triangle_depth_pixels, zbuffer);
		draw_pixels_to_framebuffer(triangle_pixels.pixels, framebuffer, zbuffer, triangle_pixels.num_pixels, color); 

		// free resources
		destroy_pixel_array(triangle_pixels);
	}

}
