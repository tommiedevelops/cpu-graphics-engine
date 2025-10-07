#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "matrix.h"
#include "vector.h"
#include "scene_manager.h"

struct Mat4 mat4_affine_orthonormal_inverse(struct Mat4 mat);

struct Mat4 get_rotation_matrix(struct Transform tr);
struct Mat4 get_scale_matrix(struct Transform tr);
struct Mat4 get_translation_matrix(struct Transform tr); 
struct Mat4 get_model_matrix(struct Transform tr);

struct Mat4 get_view_matrix(struct Camera cam);
struct Mat4 get_projection_matrix(struct Camera cam, float aspect);
struct Mat4 get_viewport_matrix(struct Camera cam);

struct Vec4f perspective_divide(struct Vec4f v);

void apply_perspective_divide(struct Triangle* tri); 
void apply_transformation(struct Mat4 tr, struct Triangle* tri);
#endif
