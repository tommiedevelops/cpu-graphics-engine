#ifndef QUATERNION_H
#define QUATERNION_H
#include <stdbool.h>
#include "matrix.h"
struct Quaternion {
	float q0; // real
	float q1; // i
	float q2; // j
	float q3; // k
};

bool quat_are_equal(struct Quaternion q0, struct Quaternion q1);
bool quat_mul_quat(struct Quaternion q0, struct Quaternion q1);
struct Quaternion quat_conjugate(struct Quaternion q);
float quat_norm(struct Quaternion q);
struct Quaternion quat_inverse(struct Quaternion q);
struct Mat4 quat_to_mat4(struct Quaternion q);
struct Quaternion euler_to_quat(struct Vec3f euler_rotation);
#endif

