#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"
#include "triangle.h"

void destroy_pixel_array(struct PixelArray pixel_array){
        free(pixel_array.pixels);
}

void destroy_bary_pixel_array(struct BaryPixelArray bary_pixel_array){
	free(bary_pixel_array.bary_pixels);
}

struct BaryPixel convert_pixel_to_bary_pixel(struct Pixel P, struct Triangle tri){

	struct Vec3f** sorted_verts = sort_vertices_by_y_asc(tri);
	struct Vec3f A = *sorted_verts[0];
	struct Vec3f B = *sorted_verts[1];
	struct Vec3f C = *sorted_verts[2];

	free(sorted_verts);

	float alpha = (A.x*(C.y-A.y)+(P.y-A.y)*(C.x-A.x)-P.x*(C.y-A.y))
			/((B.y-A.y)*(C.x-A.x)-(B.x-A.x)*(C.y-A.y));;
	float beta = ((P.y-A.y) - alpha*(B.y-A.y))/(C.y-A.y);
	float gamma = 1 - alpha - beta;

	struct BaryPixel bary_pixel = {
		.alpha = alpha,
		.beta = beta,
		.gamma = gamma
	};

	return bary_pixel;

}

struct BaryPixelArray convert_pixel_arr_to_bary_pixel_arr(
	struct PixelArray pixel_array,
	struct Triangle tri
){
	int num_pixels = pixel_array.num_pixels;

	struct BaryPixel* bary_pixels = malloc(sizeof(struct BaryPixel)*num_pixels);

	for(int i = 0; i < num_pixels; i++){
		bary_pixels[i] = convert_pixel_to_bary_pixel(pixel_array.pixels[i], tri);
	}

	struct BaryPixelArray bary_pixel_array = {
		.bary_pixels = bary_pixels,
		.num_bary_pixels = num_pixels
	};

	return bary_pixel_array;
}
