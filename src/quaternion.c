#include <stdio.h>
#include <math.h>
#include "quaternion.h"

/*
Unit quaternion q = u + v where u is a scalar and v is an R3 vector can be rewritten as
q = cos(theta/2) + n*sin(theta/2) where n is a unit vector.

When applied to a pure quaternion (which maps directly to an R3 vector), this corresponds
to a rotation of theta around vector n.
*/

bool quat_are_equal(struct Quaternion q0, struct Quaternion q1);
struct Quaternion quat_conjugate(struct Quaternion q);
struct Quaternion quat_inverse(struct Quaternion q);

struct Quaternion quat_mul(struct Quaternion p, struct Quaternion q) {
	// ORDER MATTERS
	struct Quaternion r;	
	r.q0 = p.q0*q.q0 - p.q1*q.q1 - p.q2*q.q2 - p.q3*q.q3;
	r.q1 = p.q0*q.q1 + p.q1*q.q0 - p.q2*q.q3 + p.q3*q.q2;
	r.q2 = p.q0*q.q2 + p.q2*q.q0 + p.q1*q.q3 - p.q3*q.q1;
	r.q3 = p.q0*q.q3 + p.q3*q.q0 - p.q1*q.q2 + p.q2*q.q3;
	return r;
}

struct Quaternion quat_normalize(struct Quaternion q){
	float n = quat_norm(q);
	
	if(n == 0) {
		printf("src/quaternion.c/normalize_quat: division by 0 error. returning QUAT_IDENTITY\n");
		return QUAT_IDENTITY;
	}
	
	struct Quaternion result = {
		.q0 = q.q0/n,
		.q1 = q.q1/n,
		.q1 = q.q2/n,
		.q3 = q.q3/n
	};

	return result;
}

float quat_norm(struct Quaternion q) {
	return sqrtf(q.q0*q.q0 + q.q1*q.q1 + q.q2*q.q2 + q.q3*q.q3);
}
	
struct Quaternion quat_angle_axis(float angle, struct Vec3f axis) {
	struct Quaternion q;
	float cs = cosf(angle * 0.5f);
	float sn = sinf(angle * 0.5f);

	q.q0 = cs;
	q.q1 = sn*axis.x;	
	q.q2 = sn*axis.y;
	q.q3 = sn*axis.z;
	
	return q;
}

struct Quaternion euler_to_quaternion(float pitch, float yaw, float roll) {
	// pitch => rotation about x axis in radians
	// yaw => rotation about y axis in radians
	// roll => rotation about z axis in radians
	float cy = cosf(yaw * 0.5f);
	float sy = sinf(yaw * 0.5f);
	float cp = cosf(pitch * 0.5f);
	float sp = sinf(pitch * 0.5f);
	float cr = cosf(roll * 0.5f);
	float sr = sinf(roll * 0.5f);

	struct Quaternion q;
	q.q0 = cr * cp * cy + sr * sp * sy;
	q.q1 = sr * cp * cy - cr * sp * sy;
	q.q2 = cr * sp * cy + sr * cp * sy;
	q.q3 = cr * cp * sy - sr * sp * cy;
	return q;
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

