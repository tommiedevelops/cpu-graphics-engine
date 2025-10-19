#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

typedef struct Mat4 Mat4;
typedef struct Transform Transform;
typedef struct Camera Camera;
typedef struct Triangle Triangle;

Mat4 get_rotation_matrix(Transform* tr);
Mat4 get_scale_matrix(Transform* tr);
Mat4 get_translation_matrix(Transform* tr); 
Mat4 get_model_matrix(Transform* tr);

Mat4 get_view_matrix(Camera* cam);
Mat4 get_projection_matrix(Camera* cam, float aspect);
Mat4 get_viewport_matrix(Camera* cam);

Vec4f perspective_divide(Vec4f* v);

void apply_perspective_divide(Triangle* tri); 
void apply_transformation(Mat4 tr, Triangle* tri);
#endif
