#include <stdio.h>
#include <assert.h>
#include "vec3f.h"
void test_vec3f_cross(){
	printf("test_vec3f_cross\n");	
	struct Vec3f u, v, result, expected;

	u.x = 1, u.y = 0, u.z = 0;
	v.x = 0, v.y = 1, v.z = 0;
	expected.x = 0, expected.y = 0, expected.z = 1;

	result = vec3f_cross(u,v);
	assert(vectors_are_equal(result,expected));

	printf("success\n");
}

