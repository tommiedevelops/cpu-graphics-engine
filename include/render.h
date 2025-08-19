#ifndef RENDER_H
#define RENDER_H

#include "triangle.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer); 
int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles);
void render_triangles(uint32_t* framebuffer, float* zbuffer, struct Vec3f* vertices, int* triangles, int num_triangles);

#endif
