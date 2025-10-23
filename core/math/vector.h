#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define VEC2F_X ((Vec2f){1.0f, 0.0f})
#define VEC2F_Y ((Vec2f){0.0f, 1.0f})
#define VEC2F_0 ((Vec2f){0.0f, 0.0f})
#define VEC2F_1 ((Vec2f){1.0f, 1.0f})

#define VEC3F_X ((Vec3f){1.0f, 0.0f, 0.0f})
#define VEC3F_Y ((Vec3f){0.0f, 1.0f, 0.0f})
#define VEC3F_Z ((Vec3f){0.0f, 0.0f, 1.0f})
#define VEC3F_0 ((Vec3f){0.0f, 0.0f, 0.0f})
#define VEC3F_1 ((Vec3f){1.0f, 1.0f, 1.0f})

#define VEC4F_X ((Vec4f){1.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_Y ((Vec4f){0.0f, 1.0f, 0.0f, 0.0f},)
#define VEC4F_Z ((Vec4f){0.0f, 0.0f, 1.0f, 0.0f})
#define VEC4F_W ((Vec4f){0.0f, 0.0f, 0.0f, 1.0f})
#define VEC4F_0 ((Vec4f){0.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_1 ((Vec4f){1.0f, 1.0f, 1.0f, 1.0f})

typedef struct Vec2f {
	float x, y;
} Vec2f;

typedef struct Vec3f {
	float x, y, z;
} Vec3f;

typedef struct Vec4f {
	float x,y,z,w;
} Vec4f;

void print_vec3f(Vec3f v);
void print_vec4f(Vec4f v);

Vec2f vec2f_create(float x, float y);
Vec3f vec3f_create(float x, float y, float z);
Vec4f vec4f_create(float x, float y, float z, float w);

Vec2f vec2f_add(Vec2f a, Vec2f b);
Vec3f vec3f_add(Vec3f a, Vec3f b);
Vec4f vec4f_add(Vec4f a, Vec4f b);

Vec2f vec2f_scale(Vec2f v, float value);
Vec3f vec3f_scale(Vec3f v, float value);
Vec4f vec4f_scale(Vec4f v, float value);

void vec2f_normalize(Vec2f* v);
Vec3f vec3f_normalize(Vec3f v);
Vec4f vec4f_normalize(Vec4f v);

float vec2f_magnitude(Vec2f v);
float vec3f_magnitude(Vec3f v);
float vec4f_magnitude(Vec4f v);

float vec3f_dot(Vec3f a, Vec3f b);
float vec4f_dot(Vec4f a, Vec4f b);

Vec3f vec3f_sub(Vec3f u, Vec3f v);
Vec3f vec3f_cross(Vec3f a, Vec3f b);

Vec3f vec4f_to_vec3f(Vec4f v);
Vec4f vec3f_to_vec4f(Vec3f v, float w);

Vec4f vec4f_translate(Vec4f v, float dx, float dy, float dz, float dw);

bool vec4f_are_equal(Vec4f a, Vec4f b);
bool vec3f_are_equal(Vec3f a, Vec3f b);
bool vec4f_are_about_equal(Vec4f a, Vec4f b, float allowance); 
bool vec3f_are_about_equal(Vec3f a, Vec3f b, float allowance); 

Vec4f vec4f_homogeneous_divide(Vec4f v);
uint32_t vec4f_to_rgba32(Vec4f c);

#endif
