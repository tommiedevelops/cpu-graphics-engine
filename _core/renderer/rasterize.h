#ifndef RASTERIZE_H
#define RASTERIZE_H
#include <stdint.h>
typedef struct Material Material;
typedef struct Triangle Triangle;
typedef struct Light Light;
void rasterize_triangle(Triangle* tri, Light* lights, Material* mat, uint32_t* framebuffer, float* zbuffer);
#endif
