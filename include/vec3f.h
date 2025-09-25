#ifndef VEC3F_H
#define VEC3F_H

#include <stdbool.h>

#define VEC2F_X ((struct Vec2f){1.0f, 0.0f})
#define VEC2F_Y ((struct Vec2f){0.0f, 1.0f})
#define VEC2F_0 ((struct Vec2f){0.0f, 0.0f})
#define VEC2F_1 ((struct Vec2f){1.0f, 1.0f})

#define VEC3F_X ((struct Vec3f){1.0f, 0.0f, 0.0f})
#define VEC3F_Y ((struct Vec3f){0.0f, 1.0f, 0.0f})
#define VEC3F_Z ((struct Vec3f){0.0f, 0.0f, 1.0f})
#define VEC3F_0 ((struct Vec3f){0.0f, 0.0f, 0.0f})
#define VEC3F_1 ((struct Vec3f){1.0f, 1.0f, 1.0f})

struct Bounds;

#pragma once
struct Vec3f {
	float x, y, z;
};

struct Vec2f {
	float x, y;
};

struct Vec3f vec3f_create(float x, float y, float z);


void print_vec2f(struct Vec2f v);
struct Vec2f vec2f_create(float x, float y);

float dot_product(struct Vec3f a, struct Vec3f b);
struct Vec3f vec3f_cross(struct Vec3f a, struct Vec3f b);
void print_vec3f(struct Vec3f v);
struct Vec3f vec3f_normalize(struct Vec3f v);
float vec3f_magnitude(struct Vec3f v);
struct Vec3f vec3f_add(struct Vec3f a, struct Vec3f b);
struct Vec3f vec3f_scale(struct Vec3f v, float value);
void convert_vec3f_to_int_values(struct Vec3f* vertex);
void scale_vector(struct Vec3f* vec3f, float scalar);
void translate_vector(struct Vec3f* vec3f, float dx, float dy, float dz);
bool vectors_are_equal(struct Vec3f a, struct Vec3f b);


#endif

