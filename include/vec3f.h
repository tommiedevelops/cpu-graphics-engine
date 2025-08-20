#ifndef VEC3F_H
#define VEC3F_H

#include <stdbool.h>
#include "bounds.h"

#define VEC3F_X ((struct Vec3f){1.0f, 0.0f, 0.0f})
#define VEC3F_Y ((struct Vec3f){0.0f, 1.0f, 0.0f})
#define VEC3F_Z ((struct Vec3f){0.0f, 0.0f, 1.0f})
#define VEC3F_0 ((struct Vec3f){0.0f, 0.0f, 0.0f})
#define VEC3F_1 ((struct Vec3f){1.0f, 1.0f, 1.0f})

struct Bounds;

struct Vec3f {
	float x;
	float y;
	float z;
};

float dot_product(struct Vec3f a, struct Vec3f b);
struct Vec3f vec3f_cross(struct Vec3f a, struct Vec3f b);

struct Vec3f vec3f_normalize(struct Vec3f v);
float vec3f_magnitude(struct Vec3f v);
struct Vec3f vec3f_add(struct Vec3f a, struct Vec3f b);
struct Vec3f vec3f_scale(struct Vec3f v, float value);
void convert_vec3f_to_int_values(struct Vec3f* vertex);
void scale_vector(struct Vec3f* vec3f, float scalar);
void translate_vector(struct Vec3f* vec3f, float dx, float dy, float dz);
bool vectors_are_equal(struct Vec3f a, struct Vec3f b);

void normalize_vectors(float sidelength, struct Vec3f* vectors, int num_vectors);


#endif

