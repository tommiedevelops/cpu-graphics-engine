#include <stdio.h>

#include "matrix.h"
#include "vec3f.h"

struct Vec3f mat3_mul_vec3(struct Mat3 m, struct Vec3f v) {
	struct Vec3f result;
	result.x = m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z;
	result.y = m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z;
	result.z = m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z;
	return result;
}
