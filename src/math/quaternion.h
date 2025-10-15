#ifndef QUATERNION_H
#define QUATERNION_H

#include <stdbool.h>
#include "matrix.h"

#define QUAT_IDENTITY (struct Quaternion){1.0f, 0.0f, 0.0f, 0.0f}

typedef struct Quaternion {
	float q0, q1, q2, q3;
} Quat;

void print_quat(Quat q);

Vec3f quat_get_forward(Quat q);
Vec3f quat_get_right(Quat q);

bool quat_are_about_equal(Quat q0, Quat q1, float epsilon);
bool quat_are_equal(Quat q0, Quat q1);

Quat quat_slerp(Quat q, Quat p, float t);
Quat quat_angle_axis(float angle, Vec3f axis); 
Quat quat_mul(Quat q0, Quat q1);
Quat quat_conjugate(Quat q);
Quat quat_normalize(Quat q);
float quat_norm(Quat q);
Quat quat_inverse(Quat q);
Vec3f quat_mul_vec3(Quat q, Vec3f v);

Mat4 quat_to_mat4(Quat q);
Quat euler_to_quat(Vec3f euler_rotation);


#endif

