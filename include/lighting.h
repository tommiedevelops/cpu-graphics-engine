#ifndef LIGHTING_H
#define LIGHTING_H

#include "triangle.h"
#include "scene_manager.h"
#include "shading.h"

void precompute_lighting(struct Material* mat, struct Triangle tri, struct LightSource ls, struct Camera cam);

#endif
