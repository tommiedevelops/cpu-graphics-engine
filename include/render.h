#ifndef RENDER_H
#define RENDER_H

#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "triangle.h"
#include "scene_manager.h"
#include "constants.h"
#include "vector.h"
#include "transformation.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer); 
int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles);
void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene);
#endif
