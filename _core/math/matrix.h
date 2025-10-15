#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

#define MAT4_ZERO ((struct Mat4){ { \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}  \
} })

typedef struct Mat3 {
	float m[3][3];
} Mat3;

typedef struct Mat4 {
	float m[4][4];
} Mat4;

bool mat4_are_equal(Mat4 m0,Mat4 m1);
void print_mat4(Mat4 m);
Vec3f mat3_mul_vec3(Mat3 m, Vec3f v);
Mat4 mat4_mul_mat4(Mat4 m0, Mat4 m1);
Vec4f mat4_mul_vec4(Mat4 m, Vec4f v);
Mat3 mat3_transpose(Mat3 m);
Mat3 scal_mul_mat3(float s, Mat3 m);

#endif

