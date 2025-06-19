#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bounds.h"
#include "vertex.h"

bool bounds_are_equal(struct Bounds a, struct Bounds b){
	if(a.xmin != b.xmin) return false;
	if(a.xmax != b.xmax) return false;
	if(a.ymin != b.ymin) return false;
	if(a.ymax != b.ymax) return false;
	if(a.zmin != b.zmin) return false;
	if(a.zmax != b.zmax) return false;
	return true;
}

struct Bounds get_bounds(struct Vertex* vertices, int num_vertices) {
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

void update_bounds(struct Bounds* bounds, struct Vertex* vertices, int num_vertices) {
        for(int i = 0; i < num_vertices; i++){
                if( vertices[i].x > bounds->xmax ){ bounds->xmax = vertices[i].x; }
                if( vertices[i].x < bounds->xmin ){ bounds->xmin = vertices[i].x; }
                if( vertices[i].y > bounds->ymax ){ bounds->ymax = vertices[i].y; }
                if( vertices[i].y < bounds->ymin ){ bounds->ymin = vertices[i].y; }
                if( vertices[i].z > bounds->zmax ){ bounds->zmax = vertices[i].z; }
                if( vertices[i].z < bounds->zmin ){ bounds->zmin = vertices[i].z; }
        }
}

