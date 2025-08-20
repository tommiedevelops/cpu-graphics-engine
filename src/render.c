#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vec3f.h"
#include "render.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
                if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. pixel= {%d,%d}\n", x,y); return;}
                if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. pixel= {%d,%d}\n", x,y); return;}

                framebuffer[x + WIDTH*y] = value;
}

void render_triangles(uint32_t* framebuffer, float* zbuffer, struct Vec3f* vertices, int* triangles, int num_triangles, struct LightSource light_source){
	for(int i = 0; i < num_triangles; i++)	{

		// create a triangle
		struct Triangle tri = {
			.v0 = &vertices[triangles[3*i]],
			.v1 = &vertices[triangles[3*i+1]],
			.v2 = &vertices[triangles[3*i+2]]
		};
		
		// Calculate the normal vector of the triangle
		struct Vec3f normal = calculate_normal(tri);		

		//Calculate the dot product between normal vector and light source
		float dot_prod = dot_product(normal, light_source.direction);
		
		// Square dot product to fit in [0,1] range
		dot_prod = dot_prod * dot_prod;
		
		// determine the color of the triangle
		uint8_t r, g, b, a;
		a = dot_prod* 256;
		r = dot_prod* 256;
		g = dot_prod* 256;
		b = dot_prod* 256;

		uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;

		// draw triangles
		rasterize_triangle(tri, framebuffer, zbuffer, color);
		
	}

}
