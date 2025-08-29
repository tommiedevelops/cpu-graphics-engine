#include <stdio.h>
#include <assert.h>
#include "quaternion.h"
#include "matrix.h"

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
