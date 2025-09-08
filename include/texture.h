#ifndef TEXTURE_H
#define TEXTURE_H


struct Texture {
	int width;
	int height;
	struct Color* map;
};

struct Texture load_texture(char* filename);

void free_texture(struct Texture tex);
#endif
