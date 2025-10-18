#include "primitive.h"
#include "bounds.h"

Bounds triangle_get_bounds(const Triangle* tri){
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

