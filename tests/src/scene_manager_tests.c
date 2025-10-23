#include <stdio.h>
#include <assert.h>

#include "transformation.h"
#include "scene_manager.h"
#include "matrix.h"
#include "quaternion.h"

void test_mat4_affine_orthonormal_inverse(){
	printf("test_mat4_affine_orthonormal_inverse\n");

	Vec4f v = {.x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 4.0f}; 

	// represents a translation of 1.0f in x direction and a rotation of 90deg about Z
	struct Mat4 m = {{
		{0, -1, 0, 1},
		{1, 0, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0 ,1}
	}};
	
	Vec4f u = mat4_mul_vec4(m,v);
	u = mat4_mul_vec4(mat4_affine_orthonormal_inverse(m), u);
	
	assert(vec4f_are_equal(u,v));
	printf("success\n");
}

void test_get_scale_matrix() {
	printf("test_get_scale_matrix\n");

	Transform tr;
	Vec3f scale = {.x = 2.0f, .y = 3.0f, .z = 5.0f };
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

	Transform tr;
	Vec3f position = {.x = 5.0f, .y = 0.0f, .z = 0.0f};
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

	Transform tr;
	Vec3f euler_rot = {.x = 0, .y = PI/2, .z = 0};
	Quat rot = euler_to_quat(euler_rot);

	tr.rotation = rot;

	Vec4f starting_vec = {.x = 5.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f};
	struct Mat4 m = get_rotation_matrix(tr);
	Vec4f expected_vec = {.x = 0.0f, .y = 0.0f, .z = -5.0f, .w = 1.0f };
	Vec4f resulting_vec = mat4_mul_vec4(m,starting_vec); 

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
	Transform tr;
	Vec3f euler_rot = {.x = 0.0f, .y = PI/2, .z = 0.0f};
	tr.rotation = quat_normalize(euler_to_quat(euler_rot));

	Vec3f scale = {.x= 0.5f, .y=0.5f, .z=0.5f};
	tr.scale = scale;

	Vec3f pos = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
	tr.position = pos;	

	struct Mat4 m = get_model_matrix(tr);	
	Vec4f v = {.x = 0.0f, .y = 0.0f, .z=1.0f, .w=1.0f};
	Vec4f u = mat4_mul_vec4(m,v);
	print_mat4(m);
	print_vec4f(u);
	
	printf("success\n");
}

static inline float deg_to_rad(float deg){
	return (float)PI*deg/180.0f;
}

#define SQRT_2 (1.41421356237)

void test_get_view_matrix_1(){
	printf("test_case_1\n");
	Camera cam;
	Quat rot = quat_angle_axis(deg_to_rad(45),VEC3F_Y);

	cam.transform = transform_create(VEC3F_0, rot, VEC3F_1);
	Vec4f point = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);

	Vec4f expected = vec4f_create(0.0f, 0.0f, SQRT_2, 1.0f);

	struct Mat4 m = get_view_matrix(cam);

	Vec4f result = mat4_mul_vec4(m, point);

	float eps = 0.01f;
	assert(vec4f_are_about_equal(expected,result,eps));
}

void test_get_view_matrix_2(){
	printf("test_case_2\n");
	Camera cam;
	cam.transform = transform_create(VEC3F_0, QUAT_IDENTITY, VEC3F_1);
	Vec4f point = vec4f_create(0.0f, 0.0f, -1.0f, 1.0f);

	Vec4f expected = vec4f_create(0.0f, 0.0f, -1.0f, 1.0f);
	struct Mat4 m = get_view_matrix(cam);

	Vec4f result = mat4_mul_vec4(m, point);

	float eps = 0.01f;
	assert(vec4f_are_about_equal(expected,result,eps));
}

void test_get_view_matrix_3(){
	printf("test_case_3\n");

	Camera cam;

	Vec3f pos = vec3f_create(2.0f, 0.0f, 2.0f);
	Quat rot = quat_angle_axis(deg_to_rad(225.0f),VEC3F_Y);

	cam.transform = transform_create(pos, rot, VEC3F_1);

	Vec4f point_front = vec4f_create(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4f point_behind = vec4f_create(3.0f, 0.0f, 3.0f, 1.0f);

	Vec4f expected_front = vec4f_create(0.0f, 0.0f, 2*SQRT_2, 1.0f);
	Vec4f expected_behind = vec4f_create(0.0f, 0.0f, -SQRT_2, 1.0f);

	struct Mat4 m = get_view_matrix(cam);

	Vec4f result_front = mat4_mul_vec4(m, point_front);
	Vec4f result_behind = mat4_mul_vec4(m, point_behind);

	float eps = 0.01f;
	assert(vec4f_are_about_equal(expected_front,result_front,eps));
	assert(vec4f_are_about_equal(expected_behind,result_behind,eps));
}


void test_get_view_matrix(){
	printf("test_get_view_matrix\n");
	test_get_view_matrix_1();
	test_get_view_matrix_2();
	test_get_view_matrix_3();
	printf("success\n");
}

#define SQRT_3 (1.73205080757)

void test_get_projection_matrix_1(){
	printf("test_case_1\n");

	Camera cam = {0};
	cam.fov = (float)PI/3.0f;
	cam.near = 1.0f;
	cam.far = 2.0f;
	float aspect = 2.0f; // h / w
	
	struct Mat4 m = get_projection_matrix(cam, aspect);	

	Vec4f near = vec4f_create(0.0f,0.0f,cam.near, 1.0f);
	Vec4f far = vec4f_create(0.0f,0.0f, cam.far, 1.0f);
	// using equation y = 1/sqrt(3) * z and z = 1.5
	Vec4f top = vec4f_create(0.0f, (float)1.5f/SQRT_3, 1.5f, 1.0f);
	// using equation y = -1/sqrt(3) * z and z = 1.5
	Vec4f bot = vec4f_create(0.0f, (float)-1.5f/SQRT_3, 1.5f, 1.0f);
	// using equation x = sqrt(3)/2 * z and z = 1.5 (derive using fov and aspect)
	Vec4f right = vec4f_create(1.5f*0.5f*(float)1.0f/SQRT_3, 0.0f, 1.5f, 1.0f);
	// using equation x = -sqrt(3)/2 * z and z = 1.5 (derive using fov and aspect)
	Vec4f left = vec4f_create(1.5f*-0.5f*(float)1.0f/SQRT_3, 0.0f, 1.5f, 1.0f);
	
	Vec4f results[6];
	results[0] = mat4_mul_vec4(m,near);
	results[1] = mat4_mul_vec4(m,far);
	results[2] = mat4_mul_vec4(m,top);
	results[3] = mat4_mul_vec4(m,bot);
	results[4] = mat4_mul_vec4(m,right);
	results[5] = mat4_mul_vec4(m,left);

	Vec4f expected[6];
	expected[0] = vec4f_create(0.0f,0.0f,0.0f,1.0f); // z = 0
	expected[1] = vec4f_create(0.0f,0.0f,2.0f,2.0f); // z = w
	expected[2] = vec4f_create(0.0f,1.5f,1.0f,1.5f); // y = w
	expected[3] = vec4f_create(0.0f,-1.5f,1.0f,1.5f); // y = -w 
	expected[4] = vec4f_create(1.5f,0.0f,1.0f,1.5f); // x = w
	expected[5] = vec4f_create(-1.5f,0.0f,1.0f,1.5f); // x = -w

	float eps = 0.01f;
	for(int i = 0; i < 6; i++){
		assert(vec4f_are_about_equal(expected[i], results[i], eps));
	}

}

void test_get_projection_matrix(){
	printf("test_get_projection_matrix\n");
	test_get_projection_matrix_1();
	printf("success\n");
}
