#ifndef RENDER_H
#define RENDER_H

#include "edge.h"
int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(uint32_t* framebuffer, struct Edge* wireframe_edges, int num_edges);

#endif
