#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "matrix.h"
#include "vector.h"
#include "scene_manager.h"
#include "primitive.h"

Mat4 mat4_affine_orthonormal_inverse(Mat4 mat);

Mat4 get_rotation_matrix(Transform tr);
Mat4 get_scale_matrix(Transform tr);
Mat4 get_translation_matrix(Transform tr); 
Mat4 get_model_matrix(Transform tr);

Mat4 get_view_matrix(Camera cam);
Mat4 get_projection_matrix(Camera cam, float aspect);
Mat4 get_viewport_matrix(Camera cam);

Vec4f perspective_divide(Vec4f v);

void apply_perspective_divide(Triangle* tri); 
void apply_transformation(Mat4 tr, Triangle* tri);
#endif
