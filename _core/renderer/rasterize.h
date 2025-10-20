#ifndef RASTERIZE_H
#define RASTERIZE_H
#include <stdint.h>
typedef struct Material Material;
typedef struct Triangle Triangle;
typedef struct Lighting Lighting;
typedef struct FrameBuffer FrameBuffer;
void rasterize_triangle(Triangle* tri, Lighting* lgt, Material* mat, FrameBuffer* fb);
#endif
