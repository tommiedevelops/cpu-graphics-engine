#ifndef QUATERNION_H
#define QUATERNION_H
#include <stdbool.h>
#include "matrix.h"

#define QUAT_IDENTITY (struct Quaternion){1.0f, 0.0f, 0.0f, 0.0f}

struct Quaternion {
	float q0; // real
	float q1; // i
	float q2; // j
	float q3; // k
};

struct Quaternion quat_angle_axis(float angle, struct Vec3f axis); 
bool quat_are_equal(struct Quaternion q0, struct Quaternion q1);
struct Quaternion quat_mul(struct Quaternion q0, struct Quaternion q1);
struct Quaternion quat_conjugate(struct Quaternion q);
struct Quaternion quat_normalize(struct Quaternion q);
float quat_norm(struct Quaternion q);
struct Quaternion quat_inverse(struct Quaternion q);
struct Mat4 quat_to_mat4(struct Quaternion q);
struct Quaternion euler_to_quat(struct Vec3f euler_rotation);
#endif

