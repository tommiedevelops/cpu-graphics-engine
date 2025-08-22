#include <stdio.h>
#include "color.h"

struct Color get_random_color() {
	struct Color color;

	color.r = rand() % 256;	
	color.g = rand() % 256; 
	color.b = rand() % 256; 
	color.a = 255; 

	return color;
}

uint32_t color_to_int(struct Color color) {
	uint32_t color_as_int = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
	return color_as_int;
}

