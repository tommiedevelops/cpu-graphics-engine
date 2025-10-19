#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

typedef struct Mat4 Mat4;
typedef struct Triangle Triangle;
typedef struct Quaternion Quat;

Mat4 get_rotation_matrix(Quat rot);
Mat4 get_scale_matrix(Vec3f scale);
Mat4 get_translation_matrix(Vec3f pos); 
Mat4 get_model_matrix(Vec3f pos, Quat rot, Vec3f scale);

Mat4 get_view_matrix(Vec3f cam_pos, Quat cam_rot, Vec3f cam_scale);
Mat4 get_projection_matrix(float fov, float near, float far, float aspect);
Mat4 get_viewport_matrix(float near, float far, float vp_width, float vp_height);

void apply_perspective_divide(Triangle* tri); 
void apply_transformation(Mat4 tr, Triangle* tri);
#endif
