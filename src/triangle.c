#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"
#include "color.h"
#include "render.h"
#include "constants.h"

struct Triangle create_triangle(
	struct Vec3f* v0,
	struct Vec3f* v1,
	struct Vec3f* v2 
){
	if( (v0 == NULL) || (v1 == NULL) || (v2 == NULL) ){
		perror("src/triangle.c/create_triangle:provided parameter(s) are null");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	struct Triangle tri = {.v0=v0,.v1=v1,.v2=v2};
	return tri;
}

struct Vec3f calculate_normal(struct Triangle tri){
	
	// u = v1 - v0
	struct Vec3f u = vec3f_add(*tri.v0, vec3f_scale(*tri.v1, -1));
	// v = v2 - v0
	struct Vec3f v = vec3f_add(*tri.v0, vec3f_scale(*tri.v2, -1));
	
	struct Vec3f n = vec3f_normalize(vec3f_cross(u,v));
	
	return n;
}

void swap(void** a, void** b){
	void* temp = *a;
	*a = *b;
	*b = temp;
}

void sort_vertices_by_y_asc(struct Triangle tri) {
	// Mini 3-element bubble sort to order vertices by ascending y
	if (tri.v1->y < tri.v0->y) swap((void**)&tri.v0,(void**)&tri.v1);
	if (tri.v2->y < tri.v1->y) swap((void**)&tri.v2,(void**)&tri.v1);
	if (tri.v1->y < tri.v0->y) swap((void**)&tri.v0,(void**)&tri.v1);
}

bool inside_triangle(float alpha, float beta, float gamma){
	return (alpha > 0) && (beta > 0) && (gamma > 0) && (alpha <= 1) && (beta <= 1) && (gamma <= 1);
}

float interpolate_depth(struct Triangle tri, float alpha, float beta, float gamma){
	// the basic idea is that each point x,y has a z value. i just need to calculate it
	float z0 = tri.v0->z;
	float z1 = tri.v1->z;
	float z2 = tri.v2->z;

	float depth = alpha*z0 + beta*z1 + gamma*z2;

	return depth;
}

void rasterize_triangle(struct Triangle tri, uint32_t* framebuffer, float* zbuffer, uint32_t color) {
	
	// Currently assuming camera fixed on z-axis and is orthographic
	sort_vertices_by_y_asc(tri);

	struct Vec3f* A = tri.v0;
	struct Vec3f* B = tri.v1;
	struct Vec3f* C = tri.v2;
	
	struct Bounds bounds = get_bounds_from_tri(tri);

	int xmin = (int)bounds.xmin;
	int xmax = (int)bounds.xmax;
	int ymin = (int)bounds.ymin;
	int ymax = (int)bounds.ymax;

	for(int y = ymin; y <= ymax; y++){
		for(int x = xmin; x <= xmax; x++) {
			// check if within bounds
			if( x > WIDTH || x < 0) return;
			if( y > HEIGHT || y < 0) return;

			// calculate barycentric coords
			float alpha = (A->x*(C->y-A->y)+(y-A->y)*(C->x-A->x)-x*(C->y-A->y))
					/((B->y-A->y)*(C->x-A->x)-(B->x-A->x)*(C->y-A->y));

			float beta = ((y-A->y) - alpha*(B->y-A->y))/(C->y-A->y);
			float gamma = 1 - alpha - beta;
								
			if( inside_triangle(alpha, beta, gamma) ) {
				float depth = interpolate_depth(tri, alpha, beta, gamma);	
				
				// Normalize depth to only positive values
				depth = depth / 2 + LENGTH_SCALE;

				if(depth >= zbuffer[x + y*WIDTH]){
					place_pixel(x,y,color,framebuffer);			
					zbuffer[x + y*WIDTH] = depth;
				}
			}	

		}

	}
}

