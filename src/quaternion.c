#include <math.h>
#include "quaternion.h"

/*
Unit quaternion q = u + v where u is a scalar and v is an R3 vector can be rewritten as
q = cos(theta/2) + n*sin(theta/2) where n is a unit vector.

When applied to a pure quaternion (which maps directly to an R3 vector), this corresponds
to a rotation of theta around vector n.
*/

bool quat_are_equal(struct Quaternion q0, struct Quaternion q1);
bool quat_mul_quat(struct Quaternion q0, struct Quaternion q1);
struct Quaternion quat_conjugate(struct Quaternion q);
struct Quaternion quat_inverse(struct Quaternion q);

float quat_norm(struct Quaternion q) {
	return sqrtf(q.q0*q.q0 + q.q1*q.q1 + q.q2*q.q2 + q.q3*q.q3);
}
	
struct Quaternion euler_to_quat(struct Vec3f euler_rotation) {
	// Vec3f = (pitch, yaw, roll) 
	// pitch => rotation about the x axis in radians
	// yaw => rotation about they axis in radians
	// roll => rotation about the z axis in radians
	
	// TODO		
}

struct Mat4 quat_to_mat4(struct Quaternion q) {

	// normalize
	float n = quat_norm(q);
	if(n == 0) {return MAT4_ZERO;}	

	float q0 = q.q0/n;
	float q1 = q.q1/n;
	float q2 = q.q2/n;
	float q3 = q.q3/n;

		
	float m00 = 2*q0*q0 + 2*q1*q1 - 1;
	float m01 = 2*q1*q2 - 2*q0*q3;
	float m02 = 2*q1*q3 + 2*q0*q2;
	float m10 = 2*q1*q2 + 2*q0*q3;
	float m11 = 2*q0*q0 + 2*q2*q2 - 1;
	float m12 = 2*q2*q3 - 2*q0*q1;
	float m20 = 2*q1*q3 - 2*q0*q2;
	float m21 = 2*q2*q3 + 2*q0*q1;
	float m22 = 2*q0*q0 + 2*q3*q3 - 1;

	struct Mat4 result = {{
		{m00,   m01,  m02,  0.0},
		{m10,   m11,  m12,  0.0},
		{m20,   m21,  m22,  0.0},	
		{0.0,   0.0,  0.0,  1.0}
	}};

	return result;
}

