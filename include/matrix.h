#ifndef MATRIX_H
#define MATRIX_H
struct Mat3 {
	float m[3][3];
};

struct Vec3f mat3_mul_vec3(struct Mat3 m, struct Vec3f v);
#endif
