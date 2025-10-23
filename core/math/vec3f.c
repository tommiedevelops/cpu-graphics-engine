#include "vector.h"

void print_vec3f(Vec3f v){
	printf("(x = %f, y = %f, z = %f)\n", v.x, v.y, v.z);
}

Vec4f vec3f_to_vec4f(Vec3f v, float w) {
	return (Vec4f){v.x, v.y, v.z, w};	
}

bool vec3f_are_about_equal(Vec3f a, Vec3f b, float allowance) {
	bool x_equal = fabs(a.x - b.x) < allowance;
	bool y_equal = fabs(a.y - b.y) < allowance;
	bool z_equal = fabs(a.z - b.z) < allowance;

	if(x_equal && y_equal && z_equal) {
		return true; 
	} else {
		return false;
	}
}

Vec3f vec3f_create(float x, float y, float z){
	Vec3f v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

Vec3f vec3f_add(Vec3f a, Vec3f b){
	Vec3f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

Vec3f vec3f_scale(Vec3f v, float value){
	Vec3f result;
	result.x = v.x * value;
	result.y = v.y * value;
	result.z = v.z * value;
	return result;
}

float vec3f_magnitude(Vec3f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
Vec3f vec3f_normalize(Vec3f v){
	Vec3f result;
	float mag = vec3f_magnitude(v);	
	result.x = v.x/mag;
	result.y = v.y/mag;
	result.z = v.z/mag;
	return result;
}

Vec3f vec3f_cross(Vec3f a, Vec3f b) {
	Vec3f result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return result;
}

float vec3f_dot(Vec3f a, Vec3f b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool vec3f_are_equal(Vec3f a, Vec3f b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	return true;
}

Vec3f vec3f_sub(Vec3f u, Vec3f v) {
	Vec3f result;
	result.x = u.x - v.x;
	result.y = u.y - v.y;
	result.z = u.z - v.z;
	return result;
}
