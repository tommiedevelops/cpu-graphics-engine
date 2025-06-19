#include <stdio.h>
#include "vertex.h"
#include "bounds.h"

// single vertex methods
void scale_vertex(struct Vertex vertex, float scalar){
	vertex.x *= scalar;
	vertex.y *= scalar;
	vertex.z *= scalar;
}

void translate_vertex(struct Vertex vertex, float dx, float dy, float dz){
	vertex.x += dx;
	vertex.y += dy;
	vertex.z += dz;
}

bool vertices_are_equal(struct Vertex a, struct Vertex b) {
	if(a.x != b.x) return false;
	if(a.y != b.y) return false;
	if(a.z != b.z) return false;
	return true;
}

/* vertex array methods */

/* Shifts coordinates on all three axes to [0, imax - imin] */
void shift_to_origin(struct Bounds bounds, struct Vertex* vertices, int num_vertices) {
        for(int i = 0; i < num_vertices; i++){
                vertices[i].x -= bounds.xmin;
                vertices[i].y -= bounds.ymin;
                vertices[i].z -= bounds.zmin;
        }

        update_bounds(&bounds, vertices, num_vertices);
}

/* Normalizes values to between [-1,1] - assumes min = 0 for all axes */
void normalize_lengths(struct Bounds bounds, struct Vertex* vertices, int num_vertices) {

        // normalizes between [0,1]
        for(int i = 0; i < num_vertices; i++){
                vertices[i].x = (float)vertices[i].x / bounds.xmax;
                vertices[i].y = (float)vertices[i].y / bounds.ymax;
                vertices[i].z = (float)vertices[i].z / bounds.zmax;
        }

        // scale and translate to [-1,1]
        for(int i = 0; i < num_vertices; i++){
                scale_vertex(vertices[i], 2);
                translate_vertex(vertices[i], -1.0f,-1.0f,-1.0f);
        }
        update_bounds(&bounds, vertices, num_vertices);

}

/* Takes normalized vertices between [-1,1] and scales them to [-target_length/2, target_length/2] */
void scale_lengths(float target_length, struct Bounds bounds, struct Vertex* vertices, int num_vertices){
        for(int i = 0; i < num_vertices; i++){
                scale_vertex(vertices[i], (float)target_length/2.0f);
        }
        update_bounds(&bounds, vertices, num_vertices);
}

// normalize vertices to fit in box, side length s that is centred on the origin all axes scaled equally.
void normalize_vertices(float sidelength, struct Vertex* vertices, int num_vertices){
        // find the maximum vertex value along x, y AND z.
        // divide all vertices by that value (now btn 0 and 1)
        struct Bounds bounds = get_bounds(vertices, num_vertices);

        shift_to_origin(bounds, vertices, num_vertices);
        normalize_lengths(bounds, vertices, num_vertices);
        scale_lengths(sidelength, bounds, vertices, num_vertices);

}
