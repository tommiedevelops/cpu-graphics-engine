#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct Vec4f Vec4f;
typedef struct Texture Texture;

Texture* texture_load(char* filename);
void texture_destroy(Texture* tex);
Vec4f texture_sample(Texture* tex, float u, float v);

#endif
