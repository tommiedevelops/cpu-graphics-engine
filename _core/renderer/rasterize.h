#ifndef RASTERIZE_H
#define RASTERIZE_H
#include <stdint.h>
typedef struct Material Material;
typedef struct Triangle Triangle;
typedef struct Light Light;
typedef struct FrameBuffer FrameBuffer;
void rasterize_triangle(Triangle* tri, Light* lights, Material* mat, FrameBuffer* fb);
#endif
