#include "vector.h"

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

struct Vec2f vec2f_add(struct Vec2f a, struct Vec2f b){
	struct Vec2f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

struct Vec2f vec2f_scale(struct Vec2f v, float value){
	struct Vec2f result;
	result.x = v.x * value;
	result.y = v.y * value;
	return result;
}	

