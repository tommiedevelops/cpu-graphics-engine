#include <stdio.h>
#include <math.h>
#include "quaternion.h"

/*
Unit quaternion q = u + v where u is a scalar and v is an R3 vector can be rewritten as
q = cos(theta/2) + n*sin(theta/2) where n is a unit vector.

When applied to a pure quaternion (which maps directly to an R3 vector), this corresponds
to a rotation of theta around vector n.
*/

void print_quat(struct Quaternion q){
	printf("(%f,%f,%f,%f)\n", q.q0, q.q1, q.q2, q.q3);
}

bool quat_are_about_equal(struct Quaternion q0, struct Quaternion q1, float epsilon) {
	if(fabsf(q0.q0 - q1.q0) > epsilon) return false;
	if(fabsf(q0.q1 - q1.q1) > epsilon) return false;
	if(fabsf(q0.q2 - q1.q2) > epsilon) return false;
	if(fabsf(q0.q3 - q1.q3) > epsilon) return false;
	return true;
}

struct Quaternion quat_conjugate(struct Quaternion q);
struct Quaternion quat_inverse(struct Quaternion q);

struct Vec3f quat_get_forward(struct Quaternion q){
	struct Vec3f forward;
	forward.x = 2*(q.q1*q.q3 + q.q0*q.q2);
	forward.y = 2*(q.q2*q.q3 - q.q0*q.q1);
	forward.z = 1 - 2*(q.q1*q.q1 + q.q2*q.q2);
	return forward;
}
struct Quaternion quat_mul(struct Quaternion q0, struct Quaternion q1) {
	
	struct Quaternion p = quat_normalize(q0);
	struct Quaternion q = quat_normalize(q1);

	struct Mat4 m = {{
		{p.q0, -p.q1, -p.q2, -p.q3},
		{p.q1, p.q0, -p.q3, p.q2 },
		{p.q2, p.q3, p.q0, -p.q1},
		{p.q3, -p.q2, p.q1, p.q0}
	}};

	struct Vec4f q_vec = {.x = q.q0, .y = q.q1, .z = q.q2, .w = q.q3 }; 
	struct Vec4f r_vec = mat4_mul_vec4(m, q_vec);

	struct Quaternion r = {.q0 = r_vec.x, .q1 = r_vec.y, .q2 = r_vec.z, .q3 = r_vec.w };

	r = quat_normalize(r);
	return r;
}


float quat_norm(struct Quaternion q) {
	return sqrtf(q.q0*q.q0 + q.q1*q.q1 + q.q2*q.q2 + q.q3*q.q3);
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
		.q2 = q.q2/n,
		.q3 = q.q3/n
	};

	return result;
}
	
struct Quaternion quat_angle_axis(float angle, struct Vec3f axis) {
	struct Quaternion q;
	float cs = cosf(angle * 0.5f);
	float sn = sinf(angle * 0.5f);

	q.q0 = cs;
	q.q1 = sn*axis.x;	
	q.q2 = sn*axis.y;
	q.q3 = sn*axis.z;
	
	q = quat_normalize(q);	

	return q;
}
	
// euler_rot = {x=pitch, y=yaw, z=roll}  (radians)
// XYZ intrinsic order
struct Quaternion euler_to_quat(struct Vec3f euler_rot) {
    float pitch = euler_rot.x; // X
    float yaw   = euler_rot.y; // Y
    float roll  = euler_rot.z; // Z

    float cx = cosf(pitch * 0.5f), sx = sinf(pitch * 0.5f);
    float cy = cosf(yaw   * 0.5f), sy = sinf(yaw   * 0.5f);
    float cz = cosf(roll  * 0.5f), sz = sinf(roll  * 0.5f);

    struct Quaternion q;

    q.q0 =  cx*cy*cz + sx*sy*sz;   // w
    q.q1 =  sx*cy*cz - cx*sy*sz;   // x
    q.q2 =  cx*sy*cz + sx*cy*sz;   // y
    q.q3 =  cx*cy*sz - sx*sy*cz;   // z

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

