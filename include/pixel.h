#ifndef PIXEL_H
#define PIXEL_H

#include "triangle.h"
struct Triangle;
struct Pixel {
	int x;
	int y;
};

struct PixelArray {
	struct Pixel* pixels;
	int num_pixels;
};

struct BaryPixel {
	float alpha;
	float beta;
	float gamma;
};

struct BaryPixelArray {
	struct BaryPixel* bary_pixels;
	int num_bary_pixels;
};

struct BaryPixel convert_pixel_to_bary_pixel(struct Pixel pixel, struct Triangle tri);

struct BaryPixelArray convert_pixel_arr_to_bary_pixel_arr(struct PixelArray pixel_array, struct Triangle tri);

// in terms of a triangle

void destroy_pixel_array(struct PixelArray pixel_array);

void destroy_bary_pixel_array(struct BaryPixelArray bary_pixel_array);
#endif
