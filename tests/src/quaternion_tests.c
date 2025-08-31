#include <stdio.h>
#include "quaternion.h"
#include "matrix.h"

#include <assert.h>

#define PI (3.14159265359f)
#define SQRT1_2 0.7071067811865476f

void test_quat_to_mat4(){
	printf("test_quat_to_mat4\n");

	printf("test case 1\n");

	struct Quaternion q = {.q0 = 1.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f};
	struct Mat4 m = {{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	struct Mat4 res = quat_to_mat4(q);
	printf("expected\n");
	print_mat4(m);
	printf("result\n");
	print_mat4(res);
	assert(mat4_are_equal(res,m));

	printf("test case 2\n");

	struct Quaternion q1 = {.q0 = 0.0f, .q1 = 1.0f, .q2 = 0.0f, .q3 = 0.0f};

	struct Mat4 m1 = {{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	res = quat_to_mat4(q1);

	printf("expected\n");
	print_mat4(m1);
	printf("result\n");
	print_mat4(res);
	assert(mat4_are_equal(res,m1));

	printf("success\n");
}

void test_quat_normalize(){
	printf("test_quat_normalize\n");

	printf("test case 1\n");
	struct Quaternion q = { .q0 = 2.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f };
	struct Quaternion r = quat_normalize(q);
	struct Quaternion expected = {.q0 = 1.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f};

	const float EPS = 1e-6f;
	printf("result:"); print_quat(r);
	printf("expected:"); print_quat(expected);

	assert(quat_are_about_equal(r, expected, EPS));
	
	printf("success\n");
}

void test_euler_to_quat(){
	printf("test_euler_to_quat\n");

	const float EPS = 1e-6f;

	printf("test case 1\n");
	struct Vec3f euler_rot1 = {.x = PI/2, .y = 0.0f, .z = 0.0f };
	struct Quaternion q1 = { .q0 = SQRT1_2, .q1 = SQRT1_2, .q2 = 0.0f,     .q3 = 0.0f     }; // (π/2,0,0)
	struct Quaternion p1 = quat_normalize(euler_to_quat(euler_rot1));

	printf("q1 =");	print_quat(q1);
	printf("p1 =");	print_quat(p1);

	assert(quat_are_about_equal(q1,p1,EPS));

	printf("test case 2\n");
	struct Vec3f euler_rot2 = {.x = 0.0f, .y = PI/2, .z = 0.0f };
	struct Quaternion q2 = { .q0 = SQRT1_2, .q1 = 0.0f, .q2 = SQRT1_2, .q3 = 0.0f     }; // (0,π/2,0)
	struct Quaternion p2 = quat_normalize(euler_to_quat(euler_rot2));

	printf("q2 =");	print_quat(q2);
	printf("p2 =");	print_quat(p2);

	assert(quat_are_about_equal(q2,p2,EPS));

	printf("test case 3\n");
	struct Vec3f euler_rot3 = {.x = 0.0f, .y = 0.0f, .z = PI/2 };
	struct Quaternion q3 = { .q0 = SQRT1_2, .q1 = 0.0f, .q2 = 0.0f, .q3 = SQRT1_2 }; // (0,0,π/2)
	struct Quaternion p3 = quat_normalize(euler_to_quat(euler_rot3));

	printf("q3 =");	print_quat(q3);
	printf("p3 =");	print_quat(p3);

	assert(quat_are_about_equal(q3,p3,EPS));

	printf("success\n");
}

void test_quat_mul(){
	printf("test_quat_mul\n");

	printf("test case 1\n");

	struct Quaternion I = {1.0f, 0.0f, 0.0f, 0.0f};
	struct Quaternion q = {0.0f, 1.0f, 2.0f, 3.0f};

	struct Quaternion r1 = quat_mul(I, q);  // I * q
	struct Quaternion r2 = quat_mul(q, I);  // q * I

	printf("q="); print_quat(q);
	printf("r1="); print_quat(r1);
	printf("r2="); print_quat(r2);

	assert(quat_are_about_equal(q,r1,0.0f));
	assert(quat_are_about_equal(q,r2,0.0f));

	printf("test case 2\n");

	struct Quaternion i = {0.0f, 1.0f, 0.0f, 0.0f};
	struct Quaternion j = {0.0f, 0.0f, 1.0f, 0.0f};
	struct Quaternion k = {0.0f, 0.0f, 0.0f, 1.0f};

	struct Quaternion r3 = quat_mul(i, j);  // Expected: k

	assert(quat_are_about_equal(k,r3,0.0f));

	printf("success\n");
}

