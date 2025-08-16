#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "triangle.h"
#include "color.h"
#include "render.h"

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

// Rasterizes a single triangle
void rasterize_triangle(struct Triangle tri, uint32_t* framebuffer, uint32_t* zbuffer, uint32_t color) {

	// sort vertices in ascending y
	sort_vertices_by_y_asc(tri);

	struct Vec3f* A = tri.v0;
	struct Vec3f* B = tri.v1;
	struct Vec3f* C = tri.v2;

	struct Bounds bounds = get_bounds_from_tri(tri);

	int xmin = (int)bounds.xmin;
	int xmax = (int)bounds.xmax;
	int ymin = (int)bounds.ymin;
	int ymax = (int)bounds.ymax;
	
	for(int y = ymin; y < ymax; y++){
		for(int x = xmin; x < xmax; x++) {
			// calculate barycentric coords
			float alpha = (A->x*(C->y-A->y)+(y-A->y)*(C->x-A->x)-x*(C->y-A->y))
					/((B->y-A->y)*(C->x-A->x)-(B->x-A->x)*(C->y-A->y));

			float beta = ((y-A->y) - alpha*(B->y-A->y))/(C->y-A->y);
			float gamma = 1 - alpha - beta;

			// if inside triangle, place the pixel
			place_pixel(x,y,color,framebuffer);			
		}
	}
}

