#ifndef TEXTURE_H
#define TEXTURE_H

#include "vector.h"
typedef struct Texture Texture;

Texture* texture_load(char* filename);
void texture_free(struct Texture tex);
Vec4f texture_sample(struct Texture* tex, float u, float v);

#endif
