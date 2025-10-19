#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

void print_mat3(struct Mat3 m) {
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf("%f ", m.m[i][j]);
		}
		printf("\n");
	}
}

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

Vec3f mat3_mul_vec3(struct Mat3 m, Vec3f v) {
	Vec3f result;
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

Vec4f mat4_mul_vec4(struct Mat4 m, Vec4f v) {
	Vec4f result;

	result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
	result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
	result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
	result.w = m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;

	return result;

}

struct Mat3 mat3_transpose(struct Mat3 m){
	struct Mat3 result;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

struct Mat3 scal_mul_mat3(float s, struct Mat3 m){
	struct Mat3 result;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			result.m[i][j] = s*m.m[i][j];
		}
	}
	return result;
}

Mat4 mat4_affine_orthonormal_inverse(Mat4 mat) {
	// special case of Mat4 being an affine, orthonormal transformation	
	float (*m)[4] = mat.m; // alias the existing storage
	
	Mat3 sub = {{
		{m[0][0], m[0][1], m[0][2]},
		{m[1][0], m[1][1], m[1][2]},
		{m[2][0], m[2][1], m[2][2]}
	}};

	Vec3f t = {.x = m[0][3], .y = m[1][3], .z = m[2][3]};
	Mat3 r_T = mat3_transpose(sub);
	Mat3 mR_T = scal_mul_mat3(-1.0f, r_T);

	Vec3f final_vec = mat3_mul_vec3(mR_T, t);

	Mat4 result = {0};
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			result.m[i][j] = r_T.m[i][j];
		}
	} 
	
	result.m[0][3] = final_vec.x;
	result.m[1][3] = final_vec.y;
	result.m[2][3] = final_vec.z;
	result.m[3][3] = 1.0f;

	return result;
}
