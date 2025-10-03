#include <stdio.h>
#include <assert.h>
#include "transformation.h"
#include "scene_manager.h"
#include "matrix.h"
#include "quaternion.h"

void test_clipping(){
	printf("test_clipping\n");
	

	printf("succes\n");
}

void test_mat4_affine_orthonormal_inverse(){
	printf("test_mat4_affine_orthonormal_inverse\n");

	struct Vec4f v = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f}; 

	// represents a translation of 1.0f in x direction and a rotation of 90deg about Z
	struct Mat4 m = {{
		{0, -1, 0, 1},
		{1, 0, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0 ,1}
	}};
	
	struct Vec4f u = mat4_mul_vec4(m,v);
	u = mat4_mul_vec4(mat4_affine_orthonormal_inverse(m), u);
	
	assert(vec4f_are_equal(u,v));
	printf("success\n");
}

void test_get_scale_matrix() {
	printf("test_get_scale_matrix\n");

	struct Transform tr;
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

	struct Transform tr;
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

	struct Transform tr;
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

void test_get_model_matrix(){
	printf("test_get_model_matrix\n");
	struct Transform tr;
	struct Vec3f euler_rot = {.x = 0.0f, .y = PI/2, .z = 0.0f};
	tr.rotation = quat_normalize(euler_to_quat(euler_rot));

	struct Vec3f scale = {.x= 0.5f, .y=0.5f, .z=0.5f};
	tr.scale = scale;

	struct Vec3f pos = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
	tr.position = pos;	

	struct Mat4 m = get_model_matrix(tr);	
	struct Vec4f v = {.x = 0.0f, .y = 0.0f, .z=1.0f, .w=1.0f};
	struct Vec4f u = mat4_mul_vec4(m,v);
	print_mat4(m);
	print_vec4f(u);
	
	printf("success\n");
}

void test_get_view_matrix(){
	printf("test_get_view_matrix\n");
	// ok how tf do i test this bad boy
	// C on (0,0), facing +x
	// v on x = 2, y = 0, z = 0
	// after Model - View, v should be (0,0,2)

	printf("test_case_1\n");
	struct Quaternion z_to_x = quat_normalize(euler_to_quat((struct Vec3f){0.0f, PI/2, 0.0f}));
	
	struct Transform cam_transform = {
		.scale = VEC3F_1,
		.position = VEC3F_0,
		.rotation = z_to_x
	};

	struct Camera cam = {0};
	cam.transform = cam_transform;

	struct Vec3f test_pos = (struct Vec3f){2.0f, 0.0f, 0.0f,};

	struct Transform test = {
		.position = test_pos,
		.rotation = QUAT_IDENTITY,
		.scale = VEC3F_1
	};

	struct Mat4 model = get_model_matrix(test);
	struct Mat4 view = get_view_matrix(cam);

	// In the frame of the test transform
	struct Vec4f test_vec4 = VEC4F_0;
	test_vec4.w = 1.0f;

	struct Vec4f result_vec4 = mat4_mul_vec4(model,test_vec4);
	result_vec4 = mat4_mul_vec4(view, result_vec4);

	struct Vec3f result = {
		.x = result_vec4.x, .y = result_vec4.y, .z = result_vec4.z
	};

	struct Vec3f expected = (struct Vec3f){0.0f, 0.0f, 2.0f};

	assert(vectors_are_equal(result,expected));
	printf("test_case_2\n");

	cam_transform.rotation = QUAT_IDENTITY;
	cam.transform = cam_transform;
	view = get_view_matrix(cam);

	// In the frame of the test transform
	test_vec4 = VEC4F_0;
	test_vec4.w = 1.0f;
	test_vec4.z = -1.0f;

	result_vec4 = mat4_mul_vec4(view, test_vec4);

	result.x = result_vec4.x;
	result.y = result_vec4.y;
	result.z = result_vec4.z;

	expected = (struct Vec3f){0.0f, 0.0f, -1.0f};

	assert(vectors_are_equal(result,expected));

	printf("success\n");
}
