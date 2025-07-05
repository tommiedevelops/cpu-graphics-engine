#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3 {
	float x;
	float y;
	float z;
}; 

#define VECTOR3_X ((struct Vector3){1.0f, 0.0f, 0.0f})
#define VECTOR3_Y ((struct Vector3){0.0f, 1.0f, 0.0f})
#define VECTOR3_Z ((struct Vector3){0.0f, 0.0f, 1.0f})

float dot_product(struct Vector3 a, struct Vector3 b);
struct Vector3 cross_product(struct Vector3 a, struct Vector3 b);
#endif
