#include <stdio.h>
#include <assert.h>
#include "scene_manager.h"
#include "matrix.h"
#include "quaternion.h"

#define PI (3.14159265358979323846)

struct Transform tr;

void test_get_scale_matrix() {
	printf("test_get_scale_matrix\n");

	struct Vec3f scale = {.x = 2.0f, .y = 3.0f, .z = 5.0f };
	tr.scale = scale; 

	struct Mat4 result = get_scale_matrix(tr);
	
	struct Mat4 expected = {{
		{2.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 3.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 5.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	printf("expected\n");	
	print_mat4(expected);

	printf("result\n");
	print_mat4(result);

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			assert(expected.m[i][j] == result.m[i][j]);
		}
	}	

	printf("success\n");
}

void test_get_translation_matrix() {
	printf("test_get_translation_matrix\n");

	struct Vec3f position = {.x = 5.0f, .y = 0.0f, .z = 0.0f};
	tr.position = position;

	struct Mat4 result = get_translation_matrix(tr);
	
	struct Mat4 expected = {{
		{1.0f, 0.0f, 0.0f, 5.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	printf("expected\n");	
	print_mat4(expected);

	printf("result\n");
	print_mat4(result);

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			assert(expected.m[i][j] == result.m[i][j]);
		}
	}	


	printf("success\n");
}

void test_get_rotation_matrix() {
	printf("test_get_rotation_matrix\n");

	struct Vec3f euler_rot = {.x = 0, .y = PI/2, .z = 0};
	struct Quaternion rot = euler_to_quat(euler_rot);

	tr.rotation = rot;

	struct Vec4f starting_vec = {.x = 5.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
	struct Mat4 m = get_rotation_matrix(tr);
	struct Vec4f expected_vec = {.x = 0.0f, .y = 0.0f, .z = -5.0f, .w = 1.0f };
	struct Vec4f resulting_vec = mat4_mul_vec4(m,starting_vec); 

	printf("expected_vec\n");
	print_vec4f(expected_vec);
	printf("resulting_vec\n");
	print_vec4f(resulting_vec);

	printf("rotation_matrix\n");
	print_mat4(m);

	float allowance = 0.0001;
	assert(vec4f_are_about_equal(resulting_vec, expected_vec, allowance));

	printf("success\n");
}


