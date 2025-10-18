#ifndef RASTERIZE_H
#define RASTERIZE_H

void rasterize_triangle(Triangle* tri, Light* lights, Material* mat, uint32_t* framebuffer, float* zbuffer);

#endif
