#ifndef VEC4F_H
#define VEC4F_H

#include <stdbool.h>

#define VEC4F_X ((struct Vec4f){1.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_Y ((struct Vec4f){0.0f, 1.0f, 0.0f, 0.0f},)
#define VEC4F_Z ((struct Vec4f){0.0f, 0.0f, 1.0f, 0.0f})
#define VEC4F_W ((struct Vec4f){0.0f, 0.0f, 0.0f, 1.0f})
#define VEC4F_0 ((struct Vec4f){0.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_1 ((struct Vec4f){1.0f, 1.0f, 1.0f, 1.0f})

struct Vec4f {
	float x;
	float y;
	float z;
	float w;
};

struct Vec4f vec4f_create(float x, float y, float z, float w);

float vec4f_dot(struct Vec4f a, struct Vec4f b);
struct Vec4f vec4f_normalize(struct Vec4f v);
float vec4f_magnitude(struct Vec4f v);
struct Vec4f vec4f_add(struct Vec4f a, struct Vec4f b);
struct Vec4f vec4f_scale(struct Vec4f v, float value);
struct Vec4f vec4f_translate(struct Vec4f v, float dx, float dy, float dz, float dw);
bool vec4f_are_equal(struct Vec4f a, struct Vec4f b);
void print_vec4f(struct Vec4f v);
bool vec4f_are_about_equal(struct Vec4f a, struct Vec4f b, float allowance); 
#endif

