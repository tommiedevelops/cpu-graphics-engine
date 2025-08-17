#include <stdio.h>
#include <assert.h>
#include "triangle.h"
#include "vec3f.h"
#include "bounds.h"

void test_get_bounds_from_tri(){
	printf("test_create_get_bounds_from_tri\n");	

	struct Vec3f v0 = {.x = 0, .y = 0, .z = 0 };
	struct Vec3f v1 = {.x = 0, .y = 1, .z = 0 };
	struct Vec3f v2 = {.x = 1, .y = 1, .z = 0 };

	struct Triangle tri = {
		.v0 = &v0,
		.v1 = &v1,
		.v2 = &v2
	};
	
	struct Bounds bounds = get_bounds_from_tri(tri); 
	
	print_bounds(bounds);
	assert(bounds.xmin == 0);
	assert(bounds.ymin == 0);
	assert(bounds.zmin == 0);
	assert(bounds.xmax == 1);
	assert(bounds.ymax == 1);
	assert(bounds.zmax == 0);

	printf("success\n");
}


