#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

void print_mat4(struct Mat4 m) {
	
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%f ", m.m[i][j]);
		}
		printf("\n");
	}
}


bool mat4_are_equal(struct Mat4 m0, struct Mat4 m1){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(m0.m[i][j] != m1.m[i][j]) {return false;}
		}
	}
	return true;
}

struct Vec3f mat3_mul_vec3(struct Mat3 m, struct Vec3f v) {
	struct Vec3f result;
	result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z;
	result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z;
	result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z;
	return result;
}

struct Mat4 mat4_mul_mat4(struct Mat4 m0, struct Mat4 m1) {
    struct Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.m[row][col] += m0.m[row][k] * m1.m[k][col];
            }
        }
    }
    return result;
}

struct Vec4f mat4_mul_vec4(struct Mat4 m, struct Vec4f v) {
	struct Vec4f result;

	result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
	result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
	result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
	result.w = m.m[3][0]*v.z + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;

	return result;

}

