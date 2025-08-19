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

void render_triangles(uint32_t* framebuffer, float* zbuffer, struct Vec3f* vertices, int* triangles, int num_triangles){
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
