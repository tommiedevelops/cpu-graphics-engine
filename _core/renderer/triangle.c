#include "triangle.h"
#include "bounds.h"

Bounds tri_get_bounds(const Triangle* tri){
	Bounds bounds = BOUNDS_DEFAULT;
	for(int i = 0; i <= 2; i++){
		if(tri->v[i].pos.x > bounds.xmax) {bounds.xmax = tri->v[i].pos.x;}
		if(tri->v[i].pos.y > bounds.ymax) {bounds.ymax = tri->v[i].pos.y;}
		if(tri->v[i].pos.z > bounds.zmax) {bounds.zmax = tri->v[i].pos.z;}
		if(tri->v[i].pos.x < bounds.xmin) {bounds.xmin = tri->v[i].pos.x;}
		if(tri->v[i].pos.y < bounds.ymin) {bounds.ymin = tri->v[i].pos.y;}
		if(tri->v[i].pos.z < bounds.zmin) {bounds.zmin = tri->v[i].pos.z;}

	}
	return bounds;
}

void tri_apply_perspective_divide(Triangle* tri) {
	tri->v[0].pos = vec4f_homogeneous_divide(tri->v[0].pos);
	tri->v[1].pos = vec4f_homogeneous_divide(tri->v[1].pos);
	tri->v[2].pos = vec4f_homogeneous_divide(tri->v[2].pos);
}

void tri_apply_transformation(Mat4 tr, Triangle* tri) {
	tri->v[0].pos = mat4_mul_vec4(tr, tri->v[0].pos);
	tri->v[1].pos = mat4_mul_vec4(tr, tri->v[1].pos);
	tri->v[2].pos = mat4_mul_vec4(tr, tri->v[2].pos);
}	

