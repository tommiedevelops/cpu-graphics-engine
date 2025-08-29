#ifndef MATRIX_H
#define MATRIX_H
#include "vec3f.h"
#include "vec4f.h"

#define MAT4_ZERO ((struct Mat4){ { \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}  \
} })

struct Mat3 {
	float m[3][3];
};

struct Mat4 {
	float m[4][4];
};
bool mat4_are_equal(struct Mat4 m0, struct Mat4 m1);
void print_mat4(struct Mat4 m);
struct Vec3f mat3_mul_vec3(struct Mat3 m, struct Vec3f v);
struct Mat4 mat4_mul_mat4(struct Mat4 m0, struct Mat4 m1);
struct Vec4f mat4_mul_vec4(struct Mat4 m, struct Vec4f v);

#endif
