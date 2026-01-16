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

	stbi_image_free(img);

	return tex;
}

void texture_destroy(Texture* tex){
	if(!tex) return;
	if(tex->map) free(tex->map);
	free(tex);
}

Vec4f texture_sample(Texture* tex, float u, float v){
	if(!tex || !tex->map) return VEC4F_0;

	// Tiling mode is Repeat / Wrap
	float my_u = u - floorf(u);		
	float my_v = v - floorf(v);

	// Nearest neighbour sampling (point sampling)
	float xf = my_u * (tex->width - 1);
	float yf = my_v * (tex->height - 1);

	int x = (int)(xf + 0.5f);
	int y = (int)(yf + 0.5f);

	if (x < 0) x = 0;
	if (x >= tex->width)  x = tex->width - 1;
	if (y < 0) y = 0;
	if (y >= tex->height) y = tex->height - 1;

	Vec4f value = tex->map[y*tex->width + x];

	return value;
}

