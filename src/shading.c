#include <stdio.h>

#include "shading.h" 
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

struct Texture load_texture(char* filename){
	int width, height, channels;
	uint8_t* img = stbi_load(filename, &width, &height, &channels, 4);

	struct Vec4f* map = malloc(sizeof(struct Vec4f)*width*height);
	memset(map, 0x0, sizeof(struct Vec4f)*width*height);

	struct Texture tex = {
		.width = width,
		.height = height,
		.map = map
	};

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			int index = (j * width + i) * 4;

			struct Vec4f col_rgb = {
				.x = img[index],
				.y = img[index + 1],
				.z = img[index + 2],
				.w = img[index + 3]
			};
			
			map[j * width + i] = col_rgb;
		}
	}

	return tex;
}

void free_texture(struct Texture tex){
	if(tex.map != NULL) free(tex.map);
}
