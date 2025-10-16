#ifndef LIGHTING_H
#define LIGHTING_H

#include "triangle.h"
#include "scene_manager.h"
#include "shading.h"

typedef struct Triangle Triangle;

Vec3f compute_eyesight_vector(Vec3f cam_pos, Vec3f origin);

Vec4f compute_specular(float exponent, Vec4f light_col, Vec3f norm, Vec3f cam_pos, Vec3f light_dir, Triangle tri);

Vec4f compute_diffuse(Vec4f albedo, Vec3f light_dir, Vec4f light_col, Vec3f norm);

void precompute_lighting(Material* mat, Triangle tri, Scene* scene);

#endif
