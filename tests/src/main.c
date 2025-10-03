#include <stdio.h>
#include "matrix_tests.h"
#include "scene_manager_tests.h"
#include "quaternion_tests.h"
#include "clip_tests.h"

void scene_manager_tests(){
	test_get_scale_matrix();
	test_get_translation_matrix();
	test_get_rotation_matrix();
	test_get_model_matrix();
	test_get_view_matrix();
}

void matrix_tests(){
	test_mat4_affine_orthonormal_inverse();
	test_mat4_mul_mat4();
}

void quaternion_tests(){
	/* test_quat_to_mat4(); */
	/* test_quat_normalize(); */
	/* test_euler_to_quat(); */
	/* test_quat_mul(); */
	/* test_quat_slerp(); */
}
int main(){
	test_lerp();
	test_sdf();
	test_inside();
	test_intersect();
	test_clip_against_plane();
	test_clip();
}

