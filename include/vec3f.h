#ifndef VEC3F_H
#define VEC3F_H

struct Vec3f {
	float x;
	float y;
	float z;
}; 

#define VEC3F_X ((struct Vec3f){1.0f, 0.0f, 0.0f})
#define VEC3F_Y ((struct Vec3f){0.0f, 1.0f, 0.0f})
#define VEC3F_Z ((struct Vec3f){0.0f, 0.0f, 1.0f})
#define VEC3F_0 ((struct Vec3f){0.0f, 0.0f, 0.0f})
#define VEC3F_1 ((struct Vec3f){1.0f, 1.0f, 1.0f})

float dot_product(struct Vec3f a, struct Vec3f b);
struct Vec3f cross_product(struct Vec3f a, struct Vec3f b);
#endif
