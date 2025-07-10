#include <stdio.h>
#include <stdlib.h>
#include "triangle.h"

struct Triangle create_triangle(
	struct Vec3f* a,
	struct Vec3f* b,
	struct Vec3f* c
){
	if( (a == NULL) || (b == NULL) || (c == NULL) ){
		perror("src/triangle.c/create_triangle:provided parameter(s) are null");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	struct Triangle tri = {.a=a,.b=b,.c=c};
	return tri;
}


void swap(void** a, void** b){
	void* temp = *a;
	*a = *b;
	*b = temp;
}

struct Vec3f** sort_vertices_by_y_asc(struct Triangle tri) {
	struct Vec3f** arr  = malloc(sizeof(struct Vec3f*)*3);
	// [ptr_0 ptr_1 ptr_2]

	if(arr == NULL){
		perror("src/triangle.c/sort_vertices_by_y:malloc failed");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	arr[0] = tri.a; arr[1] = tri.b; arr[2] = tri.c;

	// Mini 3-element bubble sort to order vertices by ascending y
	if (arr[1]->y < arr[0]->y) swap((void**)&arr[0],(void**)&arr[1]);
	if (arr[2]->y < arr[1]->y) swap((void**)&arr[2],(void**)&arr[1]);
	if (arr[1]->y < arr[0]->y) swap((void**)&arr[0],(void**)&arr[1]);

	return arr;
}


struct Vec3f** sort_vertices_by_x_asc(struct Triangle tri) {
	struct Vec3f** arr  = malloc(sizeof(struct Vec3f*)*3);
	// [ptr_0 ptr_1 ptr_2]

	if(arr == NULL){
		perror("src/triangle.c/sort_vertices_by_x:malloc failed");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	arr[0] = tri.a; arr[1] = tri.b; arr[2] = tri.c;

	// Mini 3-element bubble sort to order vertices by ascending y
	if (arr[1]->x < arr[0]->x) swap((void**)&arr[0],(void**)&arr[1]);
	if (arr[2]->x < arr[1]->x) swap((void**)&arr[2],(void**)&arr[1]);
	if (arr[1]->x < arr[0]->x) swap((void**)&arr[0],(void**)&arr[1]);

	return arr;
}

// Rasterizes a bounding box
// NOTE: currently assuming we are projecting onto x-y plane
struct PixelArray rasterize_bounding_box(struct Bounds bounds){

	struct Vec3f bot_left = {.x = bounds.xmin, .y= bounds.ymin, .z = 0.0f};
	struct Vec3f top_right = {.x = bounds.xmax, .y = bounds.ymax, .z = 0.0f};

	int num_pixels = ((int)bounds.xmax - (int)bounds.xmin)*((int)bounds.ymax - (int)bounds.ymin);

	struct Pixel* pixels = malloc(sizeof(struct Pixel)*num_pixels);

	int pixels_index = 0;
	for(int y = (int)bounds.ymin; y < (int)bounds.ymax; y++){
		for(int x = (int)bounds.xmin; x < (int)bounds.xmax; x++) {
			pixels[pixels_index].x = x;
			pixels[pixels_index].y = y;
			pixels_index++;
		}
	}
 
	struct PixelArray pixel_array = { .pixels = pixels, .num_pixels = num_pixels};
	return pixel_array;
}

struct PixelArray cull_pixels_not_in_triangle(struct PixelArray pixel_array, struct Triangle tri){
	// convert pixel_array to BaryPixelArray (copy) w.r.t tri vertices
	struct BaryPixelArray bary_pixel_array =
	convert_pixel_arr_to_bary_pixel_arr(pixel_array, tri);

	float alpha, beta, gamma;
	int num_new_pixels = 0;

	// first loop for counting number of pixels in new array
	for(int i =  0; i < bary_pixel_array.num_bary_pixels; i++){
		alpha = bary_pixel_array.bary_pixels[i].alpha;
		beta = bary_pixel_array.bary_pixels[i].beta;
		gamma = bary_pixel_array.bary_pixels[i].gamma;

		bool inside_triangle = (alpha >= 0) && (beta >= 0) && (gamma >= 0);
		if(inside_triangle){num_new_pixels++;}
	}

	// allocate memory for new struct Pixel*
	struct Pixel* new_pixels = malloc(sizeof(struct Pixel)*num_new_pixels);

	int j = 0; //index for new pixels
	// second loop for removing pixels not in triangle
	for(int i = 0; i < bary_pixel_array.num_bary_pixels; i++){

		alpha = bary_pixel_array.bary_pixels[i].alpha;
		beta = bary_pixel_array.bary_pixels[i].beta;
		gamma = bary_pixel_array.bary_pixels[i].gamma;

		bool inside_triangle = (alpha >= 0) && (beta >= 0) && (gamma >= 0);

		if(inside_triangle){
			new_pixels[j++] = pixel_array.pixels[i];
		} else {
			continue;
		}
	}


	struct PixelArray new_pixel_array = {
		.pixels = new_pixels,
		.num_pixels = num_new_pixels
	};

	free(pixel_array.pixels);
	return new_pixel_array;



	destroy_bary_pixel_array(bary_pixel_array);
}

// Rasterizes a single triangle
struct PixelArray rasterize_triangle(struct Triangle tri) {
	// sort vertices in ascending y
	struct Vec3f** sorted_verts = sort_vertices_by_y_asc(tri);

	struct Vec3f vertices[3] = {
		*sorted_verts[0],
		*sorted_verts[1],
		*sorted_verts[2]
	};

	struct Bounds bounds = get_bounds(vertices, 3);
	struct PixelArray pixel_array = rasterize_bounding_box(bounds);
	pixel_array = cull_pixels_not_in_triangle(pixel_array, tri);
	return pixel_array;
}

