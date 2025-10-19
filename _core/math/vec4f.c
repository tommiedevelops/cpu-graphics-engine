#include "vector.h"

Vec3f vec4f_to_vec3f(Vec4f v) {
    return (Vec3f){ v.x, v.y, v.z };
}

Vec4f vec4f_homogeneous_divide(Vec4f v){
	Vec4f result = {0};
	
	if(v.w != 0){
		result.x = v.x/v.w;
		result.y = v.y/v.w;
		result.z = v.z/v.w;		
		result.w = 1.0f;
	}
	
	return result;
}

bool vec4f_are_equal(Vec4f a, Vec4f b) {
	//printf("checking {%f,%f,%f,%f} == {%f,%f,%f,%f}\n", a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	if(a.w != b.w){ return false; }
	return true;
}

bool vec4f_are_about_equal(Vec4f a, Vec4f b, float allowance) {
	//printf("checking {%f,%f,%f,%f} == {%f,%f,%f,%f}\n", a.x,a.y,a.z,a.w,b.x,b.y,b.z,b.w);
	if( fabs(a.x - b.x) > allowance ) { return false; }
	if( fabs(a.y - b.y) > allowance ) { return false; }
	if( fabs(a.z - b.z) > allowance ) { return false; }
	if( fabs(a.w - b.w) > allowance ) { return false; }

	return true;
}

void print_vec4f(Vec4f v){
	printf("(x = %f, y = %f, z = %f, w = %f)\n", v.x, v.y, v.z, v.w);
}

Vec4f vec4f_create(float x, float y, float z, float w){
	Vec4f v = {
		.x = x,
		.y = y,
		.z = z,
		.w = w
	};

	return v;
}

Vec4f vec4f_add(Vec4f a, Vec4f b){
	Vec4f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	result.w = a.w + b.w;
	return result;
}

Vec4f vec4f_scale(Vec4f v, float value){
	Vec4f result;
	result.x = v.x * value;
	result.y = v.y * value;
	result.z = v.z * value;
	result.w = v.w * value;

	return result;
}	

float vec4f_magnitude(Vec4f v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}
Vec4f vec4f_normalize(Vec4f v){
	Vec4f result;
	float mag = vec4f_magnitude(v);	
	result.x = v.x/mag;
	result.y = v.y/mag;
	result.z = v.z/mag;
	result.w = v.w/mag;
	return result;
}

float vec4f_dot(Vec4f a, Vec4f b){
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec4f vec4f_translate(Vec4f vector, float dx, float dy, float dz, float dw){
	vector.x += dx;
	vector.y += dy;
	vector.z += dz;
	vector.w += dw;
}


