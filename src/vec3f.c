#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "bounds.h"

void print_vec3f(struct Vec3f v){
	printf("(x = %f, y = %f, z = %f)\n", v.x, v.y, v.z);
}

void print_vec2f(struct Vec2f v) {
	printf("(x = %f, y = %f)\n", v.x, v.y);
}

struct Vec2f vec2f_create(float x, float y) {
	struct Vec2f v = {
		.x = x,
		.y = y
	};

	return v;
}


// single vector methods
void scale_vector(struct Vec3f* vector, float scalar){
	// accepts struct Vec3f ptr, not array

	if(vector == NULL) {
		perror("src/vector.c/scale_vector: vector was null");
		exit(EXIT_FAILURE);
	}

	vector->x *= scalar;
	vector->y *= scalar;
	vector->z *= scalar;
}
struct Vec3f vec3f_add(struct Vec3f a, struct Vec3f b){
	struct Vec3f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

struct Vec3f vec3f_scale(struct Vec3f v, float value){
	struct Vec3f result;
	result.x = v.x * value;
	result.y = v.y * value;
	result.z = v.z * value;
	return result;
}	

float vec3f_magnitude(struct Vec3f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
struct Vec3f vec3f_normalize(struct Vec3f v){
	struct Vec3f result;
	float mag = vec3f_magnitude(v);	
	result.x = v.x/mag;
	result.y = v.y/mag;
	result.z = v.z/mag;
	return result;
}

struct Vec3f vec3f_cross(struct Vec3f a, struct Vec3f b) {
	struct Vec3f result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return result;
}

float dot_product(struct Vec3f a, struct Vec3f b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

void translate_vector(struct Vec3f* vector, float dx, float dy, float dz){
	// accepts struct Vec3f ptr, not array

	if(vector == NULL) {
		perror("src/vector.c/translate_vector: vector was null");
		exit(EXIT_FAILURE);
	}

	vector->x += dx;
	vector->y += dy;
	vector->z += dz;
}

bool vectors_are_equal(struct Vec3f a, struct Vec3f b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	return true;
}


