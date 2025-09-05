#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bounds.h"

bool bounds_are_equal(struct Bounds a, struct Bounds b){
	if(a.xmin != b.xmin) return false;
	if(a.xmax != b.xmax) return false;
	if(a.ymin != b.ymin) return false;
	if(a.ymax != b.ymax) return false;
	if(a.zmin != b.zmin) return false;
	if(a.zmax != b.zmax) return false;
	return true;
}

struct Bounds get_bounds_from_tri(struct Triangle tri){
        /* return: [xmin, xmax, ymin, ymax, zmin, zmax] */
        struct Bounds bounds = BOUNDS_DEFAULT;
	if (tri.v0.x > bounds.xmax) {bounds.xmax = tri.v0.x;}
	if (tri.v0.y > bounds.ymax) {bounds.ymax = tri.v0.y;}
	if (tri.v0.z > bounds.zmax) {bounds.zmax = tri.v0.z;}

	if (tri.v1.x > bounds.xmax) {bounds.xmax = tri.v1.x;}
	if (tri.v1.y > bounds.ymax) {bounds.ymax = tri.v1.y;}
	if (tri.v1.z > bounds.zmax) {bounds.zmax = tri.v1.z;}

	if (tri.v2.x > bounds.xmax) {bounds.xmax = tri.v2.x;}
	if (tri.v2.y > bounds.ymax) {bounds.ymax = tri.v2.y;}
	if (tri.v2.z > bounds.zmax) {bounds.zmax = tri.v2.z;}

	if (tri.v0.x < bounds.xmin) {bounds.xmin = tri.v0.x;}
	if (tri.v0.y < bounds.ymin) {bounds.ymin = tri.v0.y;}
	if (tri.v0.z < bounds.zmin) {bounds.zmin = tri.v0.z;}

	if (tri.v1.x < bounds.xmin) {bounds.xmin = tri.v1.x;}
	if (tri.v1.y < bounds.ymin) {bounds.ymin = tri.v1.y;}
	if (tri.v1.z < bounds.zmin) {bounds.zmin = tri.v1.z;}

	if (tri.v2.x < bounds.xmin) {bounds.xmin = tri.v2.x;}
	if (tri.v2.y < bounds.ymin) {bounds.ymin = tri.v2.y;}
	if (tri.v2.z < bounds.zmin) {bounds.zmin = tri.v2.z;}

        return bounds;
}


struct Bounds get_bounds(struct Vec3f* vertices, int num_vertices) {
        /* return: [xmin, xmax, ymin, ymax, zmin, zmax] */
        struct Bounds bounds = BOUNDS_DEFAULT;

	if( vertices == NULL ) {
		perror("bounds.c/get_bounds");
		exit(EXIT_FAILURE);
	}

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

void update_bounds(struct Bounds* bounds, struct Vec3f* vertices, int num_vertices) {
        for(int i = 0; i < num_vertices; i++){
                if( vertices[i].x > bounds->xmax ){ bounds->xmax = vertices[i].x; }
                if( vertices[i].x < bounds->xmin ){ bounds->xmin = vertices[i].x; }
                if( vertices[i].y > bounds->ymax ){ bounds->ymax = vertices[i].y; }
                if( vertices[i].y < bounds->ymin ){ bounds->ymin = vertices[i].y; }
                if( vertices[i].z > bounds->zmax ){ bounds->zmax = vertices[i].z; }
                if( vertices[i].z < bounds->zmin ){ bounds->zmin = vertices[i].z; }
        }
}

void print_bounds(struct Bounds bounds){
	printf("|   |    x    |    y    |    z    |\n");
	printf("|max|%f|%f|%f|\n", bounds.xmax,bounds.ymax,bounds.zmax);
	printf("|min|%f|%f|%f|\n", bounds.xmin,bounds.ymin,bounds.zmin);
}
