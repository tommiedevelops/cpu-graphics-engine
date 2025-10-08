#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "bounds.h"

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
struct Vec2f vec2f_add(struct Vec2f a, struct Vec2f b);

struct Vec3f vec3f_create(float x, float y, float z);

float vec2f_magnitude(struct Vec2f v);
void vec2f_normalize(struct Vec2f* v);
void print_vec2f(struct Vec2f v);
struct Vec2f vec2f_create(float x, float y);
struct Vec2f vec2f_scale(struct Vec2f v, float value);
float vec3f_dot(struct Vec3f a, struct Vec3f b);
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


#define VEC4F_X ((struct Vec4f){1.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_Y ((struct Vec4f){0.0f, 1.0f, 0.0f, 0.0f},)
#define VEC4F_Z ((struct Vec4f){0.0f, 0.0f, 1.0f, 0.0f})
#define VEC4F_W ((struct Vec4f){0.0f, 0.0f, 0.0f, 1.0f})
#define VEC4F_0 ((struct Vec4f){0.0f, 0.0f, 0.0f, 0.0f})
#define VEC4F_1 ((struct Vec4f){1.0f, 1.0f, 1.0f, 1.0f})

struct Vec4f {
	float x, y, z, w;
};

static inline struct Vec4f vec3f_to_vec4f(struct Vec3f v, float w) {
    return (struct Vec4f){ v.x, v.y, v.z, w };
}

struct Vec4f vec4f_create(float x, float y, float z, float w);

float vec4f_dot(struct Vec4f a, struct Vec4f b);
struct Vec4f vec4f_normalize(struct Vec4f v);
float vec4f_magnitude(struct Vec4f v);
struct Vec4f vec4f_add(struct Vec4f a, struct Vec4f b);
struct Vec4f vec4f_scale(struct Vec4f v, float value);
struct Vec4f vec4f_translate(struct Vec4f v, float dx, float dy, float dz, float dw);
bool vec4f_are_equal(struct Vec4f a, struct Vec4f b);
bool vec3f_are_equal(struct Vec3f a, struct Vec3f b);
void print_vec4f(struct Vec4f v);
bool vec4f_are_about_equal(struct Vec4f a, struct Vec4f b, float allowance); 
bool vec3f_are_about_equal(struct Vec3f a, struct Vec3f b, float allowance); 

#endif
