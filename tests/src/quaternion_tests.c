#include <stdio.h>
#include "quaternion.h"
#include "matrix.h"

#include <assert.h>

#define PI (3.14159265359f)
#define SQRT1_2 0.7071067811865476f

void test_quat_slerp(){
	printf("test_quat_slerp\n");

	printf("test_case_1\n");

	// identity (no rot)
	Quat q = { .q0 = 1.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f };
	printf("q=");print_quat(q);

	// 180 deg about z axis
	Quat p = { .q0 = 0.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 1.0f };
	printf("p="); print_quat(p);

	float t = 0.5f;

	Quat result = quat_slerp(q,p,t); 
	printf("result="); print_quat(result);

	//90 deg rotation about z axis
	Quat expected = {.q0 = SQRT1_2, .q1 = 0.0f, .q2 = 0.0f, .q3 = SQRT1_2};
	printf("expected="); print_quat(expected);

	assert(quat_are_about_equal(result,expected, 0.00001f));

	printf("success\n");
}

void test_quat_to_mat4(){

	printf("test_quat_to_mat4\n");

	printf("test case 1\n");

	Quat q = {.q0 = 1.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f};
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

	Quat q1 = {.q0 = 0.0f, .q1 = 1.0f, .q2 = 0.0f, .q3 = 0.0f};

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

	const float EPS = 1e-6f;

	printf("test case 1\n");

	Quat q = { .q0 = 2.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f };
	Quat r = quat_normalize(q);
	Quat expected = {.q0 = 1.0f, .q1 = 0.0f, .q2 = 0.0f, .q3 = 0.0f};

	printf("result:"); print_quat(r);
	printf("expected:"); print_quat(expected);

	assert(quat_are_about_equal(r, expected, EPS));

	printf("test case 2\n");

	Quat q2 = { .q0 = 0.0f, .q1 = 3.0f, .q2 = 4.0f, .q3 = 0.0f };
	Quat r2 = quat_normalize(q2);
	// The length of (0,3,4,0) is 5 → normalized = (0, 0.6, 0.8, 0)
	Quat expected2 = { .q0 = 0.0f, .q1 = 0.6f, .q2 = 0.8f, .q3 = 0.0f };
	
	printf("result:"); print_quat(r2);
	printf("expected:"); print_quat(expected2);

	assert(quat_are_about_equal(r2, expected2, EPS));

	printf("success\n");
}

void test_euler_to_quat(){
	printf("test_euler_to_quat\n");

	const float EPS = 1e-6f;

	printf("test case 1\n");
	Vec3f euler_rot1 = {.x = PI/2, .y = 0.0f, .z = 0.0f };
	Quat q1 = { .q0 = SQRT1_2, .q1 = SQRT1_2, .q2 = 0.0f,     .q3 = 0.0f     }; // (π/2,0,0)
	Quat p1 = quat_normalize(euler_to_quat(euler_rot1));

	printf("q1 =");	print_quat(q1);
	printf("p1 =");	print_quat(p1);

	assert(quat_are_about_equal(q1,p1,EPS));

	printf("test case 2\n");
	Vec3f euler_rot2 = {.x = 0.0f, .y = PI/2, .z = 0.0f };
	Quat q2 = { .q0 = SQRT1_2, .q1 = 0.0f, .q2 = SQRT1_2, .q3 = 0.0f     }; // (0,π/2,0)
	Quat p2 = quat_normalize(euler_to_quat(euler_rot2));

	printf("q2 =");	print_quat(q2);
	printf("p2 =");	print_quat(p2);

	assert(quat_are_about_equal(q2,p2,EPS));

	printf("test case 3\n");
	Vec3f euler_rot3 = {.x = 0.0f, .y = 0.0f, .z = PI/2 };
	Quat q3 = { .q0 = SQRT1_2, .q1 = 0.0f, .q2 = 0.0f, .q3 = SQRT1_2 }; // (0,0,π/2)
	Quat p3 = quat_normalize(euler_to_quat(euler_rot3));

	printf("q3 =");	print_quat(q3);
	printf("p3 =");	print_quat(p3);

	assert(quat_are_about_equal(q3,p3,EPS));

	printf("success\n");
}

void test_quat_mul(){
	printf("test_quat_mul\n");

	printf("test case 1\n");

	Quat I = {1.0f, 0.0f, 0.0f, 0.0f};
	Quat q = {0.0f, 1.0f, 2.0f, 3.0f};

	Quat r1 = quat_mul(I, q);  // I * q
	Quat r2 = quat_mul(q, I);  // q * I

	printf("q="); print_quat(q);
	printf("r1="); print_quat(r1);
	printf("r2="); print_quat(r2);

	assert(quat_are_about_equal(q,r1,0.01f));
	assert(quat_are_about_equal(q,r2,0.01f));

	printf("test case 2\n");

	Quat i = {0.0f, 1.0f, 0.0f, 0.0f};
	Quat j = {0.0f, 0.0f, 1.0f, 0.0f};
	Quat k = {0.0f, 0.0f, 0.0f, 1.0f};

	Quat r3 = quat_mul(i, j);  // Expected: k
	assert(quat_are_about_equal(k,r3,0.0f));

	printf("test case 3\n");

	Quat r4 = quat_mul(j, k); // Expected: i

	printf("j="); print_quat(j);
	printf("k="); print_quat(k);
	printf("r4="); print_quat(r4);

	assert(quat_are_about_equal(i, r4, 0.0f));

	printf("success\n");
}

