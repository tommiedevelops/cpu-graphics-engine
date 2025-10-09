#include "vector.h"

void print_vec3f(struct Vec3f v){
	printf("(x = %f, y = %f, z = %f)\n", v.x, v.y, v.z);
}

void print_vec2f(struct Vec2f v) {
	printf("(x = %f, y = %f)\n", v.x, v.y);
}

bool vec3f_are_about_equal(struct Vec3f a, struct Vec3f b, float allowance) {
	bool x_equal = fabs(a.x - b.x) < allowance;
	bool y_equal = fabs(a.y - b.y) < allowance;
	bool z_equal = fabs(a.z - b.z) < allowance;

	if(x_equal && y_equal && z_equal) {
		return true; 
	} else {
		return false;
	}
}

struct Vec3f vec4f_to_vec3f(struct Vec4f v) {
    return (struct Vec3f){ v.x, v.y, v.z };
}

struct Vec2f vec2f_create(float x, float y) {
	struct Vec2f v = {
		.x = x,
		.y = y
	};

	return v;
}
float vec2f_magnitude(struct Vec2f v){
	return sqrt(v.x*v.x + v.y*v.y);	
}

void vec2f_normalize(struct Vec2f* v){
	if(NULL == v) {
		// LOG_ERROR("vector is null");
		return;
	}
	
	float mag = vec2f_magnitude(*v);

	if(0.0 == mag){
		// LOG_ERROR("magnitude is 0. divide by 0 error
		return;
	}

	v->x = (float)v->x / mag;
	v->y = (float)v->y / mag;
}


struct Vec3f vec3f_create(float x, float y, float z){
	struct Vec3f v;
	v.x = x;
	v.y = y;
	v.z = z;
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

struct Vec2f vec2f_add(struct Vec2f a, struct Vec2f b){
	struct Vec2f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
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

struct Vec2f vec2f_scale(struct Vec2f v, float value){
	struct Vec2f result;
	result.x = v.x * value;
	result.y = v.y * value;
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

float vec3f_dot(struct Vec3f a, struct Vec3f b){
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

bool float_equal(float a, float b, float epsilon) {
    return fabsf(a - b) < epsilon;
}

bool vectors_are_equal(struct Vec3f a, struct Vec3f b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(!float_equal(a.x,b.x,0.000001)) {return false;}
	if(!float_equal(a.y,b.y,0.000001)) {return false;}
	if(!float_equal(a.z,b.z,0.000001)) {return false;}
	return true;
}

struct Vec4f vec4f_create(float x, float y, float z, float w){
	struct Vec4f v = {
		.x = x,
		.y = y,
		.z = z,
		.w = w
	};

	return v;
}

struct Vec4f vec4f_add(struct Vec4f a, struct Vec4f b){
	struct Vec4f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return result;
}

struct Vec4f vec4f_scale(struct Vec4f v, float value){
	struct Vec4f result;
	result.x = v.x * value;
	result.y = v.y * value;
	result.z = v.z * value;
	result.w = v.w * value;

	return result;
}	

float vec4f_magnitude(struct Vec4f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}
struct Vec4f vec4f_normalize(struct Vec4f v){
	struct Vec4f result;
	float mag = vec4f_magnitude(v);	
	result.x = v.x/mag;
	result.y = v.y/mag;
	result.z = v.z/mag;
	result.w = v.w/mag;
	return result;
}

float vec4f_dot(struct Vec4f a, struct Vec4f b){
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

struct Vec4f vec4f_translate(struct Vec4f vector, float dx, float dy, float dz, float dw){
	vector.x += dx;
	vector.y += dy;
	vector.z += dz;
	vector.w += dw;
}

bool vec3f_are_equal(struct Vec3f a, struct Vec3f b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	return true;
}

bool vec4f_are_equal(struct Vec4f a, struct Vec4f b) {
	//printf("checking {%f,%f,%f,%f} == {%f,%f,%f,%f}\n", a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	if(a.w != b.w){ return false; }
	return true;
}

bool vec4f_are_about_equal(struct Vec4f a, struct Vec4f b, float allowance) {
	//printf("checking {%f,%f,%f,%f} == {%f,%f,%f,%f}\n", a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);
	if( fabs(a.x - b.x) > allowance ) { return false; }
	if( fabs(a.y - b.y) > allowance ) { return false; }
	if( fabs(a.z - b.z) > allowance ) { return false; }
	if( fabs(a.w - b.w) > allowance ) { return false; }

	return true;
}

void print_vec4f(struct Vec4f v){
	printf("(x = %f, y = %f, z = %f, w = %f)\n", v.x, v.y, v.z, v.w);
}
