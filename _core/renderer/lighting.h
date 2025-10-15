#ifndef LIGHTING_H
#define LIGHTING_H

#include "triangle.h"
#include "scene_manager.h"
#include "shading.h"

struct Triangle;
struct Scene;

struct Vec3f compute_eyesight_vector(struct Vec3f cam_pos, struct Vec3f origin);

struct Vec4f compute_specular(float exponent, struct Vec4f light_col, struct Vec3f norm, struct Vec3f cam_pos, struct Vec3f light_dir, struct Triangle tri);

struct Vec4f compute_diffuse(struct Vec4f albedo, struct Vec3f light_dir, struct Vec4f light_col, struct Vec3f norm);

void precompute_lighting(struct Material* mat, struct Triangle tri, struct Scene scene);

#endif
