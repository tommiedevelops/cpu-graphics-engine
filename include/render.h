#ifndef RENDER_H
#define RENDER_H

#include "triangle.h"

int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles);
void render_triangles(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles);
#endif
