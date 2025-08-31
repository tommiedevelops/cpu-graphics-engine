#include <stdio.h>
#include "bounds_tests.h"
#include "vec3f_tests.h"
#include "matrix_tests.h"
#include "scene_manager_tests.h"
#include "quaternion_tests.h"

void bounds_tests(){
	test_get_bounds_from_tri();	
}

void scene_manager_tests(){
	test_get_scale_matrix();
	test_get_translation_matrix();
	test_get_rotation_matrix();
}

void quaternion_tests(){
	test_quat_to_mat4();
	test_quat_normalize();
	test_euler_to_quat();
	test_quat_mul();
}
int main(){
	printf("Running unit tests...\n");
	//bounds_tests();
	test_vec3f_cross();
	test_mat4_mul_mat4();
	scene_manager_tests();
	quaternion_tests();
}

