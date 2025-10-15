#include "vector.h"

struct Vec3f vec4f_to_vec3f(struct Vec4f v) {
    return (struct Vec3f){ v.x, v.y, v.z };
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


