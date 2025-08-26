#include <stdio.h>
#include <assert.h>
#include "matrix.h"

void test_mat4_mul_mat4(){
	printf("test_mat4_mul_mat4\n");

	struct Mat4 identity;
	struct Mat4 test;	
	
	// constructing a mat4 to test
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			identity.m[i][j] = 0.0f;
			test.m[i][j] = 2.0f;
		}	
	}

	for(int i = 0; i < 4; i++) {identity.m[i][i] = 1.0f;}	

	struct Mat4 result = mat4_mul_mat4(identity, test);	
	
	printf("result\n");
	print_mat4(result);
	
	printf("test\n");
	print_mat4(test);

	// assert
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			assert(result.m[i][j] == test.m[i][j]);
		}
	}	
	printf("sucess\n");
}
