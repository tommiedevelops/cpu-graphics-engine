#include <stdio.h>

#include "shading.h" 
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

struct Texture* texture_load(char* filename){
	int width, height, channels;
	uint8_t* img = stbi_load(filename, &width, &height, &channels, 4);

	if(NULL == img){
		LOG_ERROR("something went wrong loading the .png");
	}

	struct Vec4f* map = malloc(sizeof(struct Vec4f)*width*height);
	memset(map, 0x0, sizeof(struct Vec4f)*width*height);

	struct Texture* tex = malloc(sizeof(struct Texture));
	tex->width = width;
	tex->height = height;
	tex->map = map;

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			int index = (j * width + i) * 4;

			struct Vec4f col_rgb = {
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

void texture_free(struct Texture tex){
	if(tex.map != NULL) free(tex.map);
}

struct Vec4f get_pixel(struct Vec4f* data, int width, int height, int x, int y){

	int j = y%(height-1);
	int i = x%(width-1);

	int index = j * width + i;
	/* printf("index = %d", index); */
	/* printf("size = %d\n", width*height); */

	return data[index];
}

static inline float clamp01(float x) {return x < 0 ? 0 : (x > 1 ? 1 : x); }

struct Vec4f texture_sample(struct Texture* tex, float u, float v){
	if(!tex || !tex->map) {LOG_ERROR("null texture"); return VEC4F_0;}

	u = clamp01(u);
	v = clamp01(v);
	
	int x = (int)floorf(u *(tex->width - 1));
	int y = (int)floorf(v *(tex->height - 1));

	return get_pixel(tex->map, tex->width, tex->height, x, y);
}


struct Material material_default(){
	struct Material mat = {
		.color = VEC4F_0,
		.tex = NULL
	};

	return mat;
}

struct Material* material_create(struct Vec4f color, struct Texture* tex){
	struct Material* mat = malloc(sizeof(struct Material));
	mat->color = color;
	mat->tex = tex;
	return mat;
}

struct Vec4f material_get_albedo(struct Material* mat, struct Vec2f uv) {
	if(NULL == mat){
		//LOG_ERROR("mat is NULL ptr");
		return VEC4F_0;
	}
	
	if(NULL == mat->tex){
		return mat->color;
	}

	return texture_sample(mat->tex, uv.x, uv.y);
}
