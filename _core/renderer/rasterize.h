#ifndef RASTERIZE_H
#define RASTERIZE_H

void rasterize_triangle(struct Triangle tri, Camera* cam, struct LightSource* ls, struct Material* mat, uint32_t* framebuffer, float* zbuffer);
#endif
