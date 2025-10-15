#include "bounds.h"

bool bounds_are_equal(Bounds3 a, Bounds3 b){
	if(a.xmin != b.xmin) return false;
	if(a.xmax != b.xmax) return false;
	if(a.ymin != b.ymin) return false;
	if(a.ymax != b.ymax) return false;
	if(a.zmin != b.zmin) return false;
	if(a.zmax != b.zmax) return false;
	return true;
}
Bounds3 get_bounds(Vec3f* vertices, int num_vertices) {
        Bounds3 bounds = BOUNDS_DEFAULT;
        for(int i = 0; i < num_vertices; i++){
                if( vertices[i].x > bounds.xmax ){ bounds.xmax = vertices[i].x; }
                if( vertices[i].x < bounds.xmin ){ bounds.xmin = vertices[i].x; }
                if( vertices[i].y > bounds.ymax ){ bounds.ymax = vertices[i].y; }
                if( vertices[i].y < bounds.ymin ){ bounds.ymin = vertices[i].y; }
                if( vertices[i].z > bounds.zmax ){ bounds.zmax = vertices[i].z; }
                if( vertices[i].z < bounds.zmin ){ bounds.zmin = vertices[i].z; }
        }
        return bounds;
}

void update_bounds(Bounds3* bounds, Vec3f* vertices, int num_vertices) {
        for(int i = 0; i < num_vertices; i++){
                if( vertices[i].x > bounds->xmax ){ bounds->xmax = vertices[i].x; }
                if( vertices[i].x < bounds->xmin ){ bounds->xmin = vertices[i].x; }
                if( vertices[i].y > bounds->ymax ){ bounds->ymax = vertices[i].y; }
                if( vertices[i].y < bounds->ymin ){ bounds->ymin = vertices[i].y; }
                if( vertices[i].z > bounds->zmax ){ bounds->zmax = vertices[i].z; }
                if( vertices[i].z < bounds->zmin ){ bounds->zmin = vertices[i].z; }
        }
}

void print_bounds(Bounds3 bounds){
	printf("|   |    x    |    y    |    z    |\n");
	printf("|max|%f|%f|%f|\n", bounds.xmax,bounds.ymax,bounds.zmax);
	printf("|min|%f|%f|%f|\n", bounds.xmin,bounds.ymin,bounds.zmin);
}
