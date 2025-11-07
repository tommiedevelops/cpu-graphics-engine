#include "game_math/vector.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

typedef struct Texture {
	int width;
	int height;
	Vec4f* map;
} Texture;

Texture* texture_load(char* filename){
	int width, height, channels;
	uint8_t* img = stbi_load(filename, &width, &height, &channels, 4);

	if(NULL == img) return NULL;

	Vec4f* map = malloc(sizeof(Vec4f)*width*height);
	memset(map, 0x0, sizeof(Vec4f)*width*height);

	Texture* tex = malloc(sizeof(Texture));
	tex->width = width;
	tex->height = height;
	tex->map = map;

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			int index = (j * width + i) * 4;

			Vec4f col_rgb = {
				.x = (float)img[index] / 255.0f,
				.y = (float)img[index + 1] / 255.0f,
				.z = (float)img[index + 2] / 255.0f,
				.w = (float)img[index + 3] / 255.0f
			};
			
			map[j * width + i] = col_rgb;
		}
	}

	return tex;
}

void texture_destroy(Texture* tex){
	free(tex->map);
	free(tex);
}

Vec4f get_pixel(Vec4f* data, int width, int height, int x, int y){

	int j = y%(height-1);
	int i = x%(width-1);

	int index = j * width + i;
	/* printf("index = %d", index); */
	/* printf("size = %d\n", width*height); */

	return data[index];
}

static inline float clamp01(float x) {return x < 0 ? 0 : (x > 1 ? 1 : x); }

Vec4f texture_sample(Texture* tex, float u, float v){
	if(!tex || !tex->map) return VEC4F_0;

	u = clamp01(u);
	v = clamp01(v);
	
	int x = (int)floorf(u *(tex->width - 1));
	int y = (int)floorf(v *(tex->height - 1));

	return get_pixel(tex->map, tex->width, tex->height, x, y);
}

