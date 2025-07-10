#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "bounds.h"

// single vector methods
void scale_vector(struct Vec3f* vector, float scalar){
	// accepts struct Vec3f ptr, not array

	if(vector == NULL) {
		perror("src/vector.c/scale_vector: vector was null");
		exit(EXIT_FAILURE);
	}

	vector->x *= scalar;
	vector->y *= scalar;
	vector->z *= scalar;
}

void translate_vector(struct Vec3f* vector, float dx, float dy, float dz){
	// accepts struct Vec3f ptr, not array

	if(vector == NULL) {
		perror("src/vector.c/translate_vector: vector was null");
		exit(EXIT_FAILURE);
	}

	vector->x += dx;
	vector->y += dy;
	vector->z += dz;
}

bool vectors_are_equal(struct Vec3f a, struct Vec3f b) {
	printf("checking {%f,%f,%f} == {%f,%f,%f}\n", a.x,a.y,a.z,b.x,b.y,b.z);
	if(a.x != b.x){ return false; }
	if(a.y != b.y){ return false; }
	if(a.z != b.z){ return false; }
	return true;
}

/* vector array methods */

/* Shifts coordinates on all three axes to [0, imax - imin] */
static void shift_to_origin(struct Bounds bounds, struct Vec3f* vectors, int num_vectors) {
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x -= bounds.xmin;
                vectors[i].y -= bounds.ymin;
                vectors[i].z -= bounds.zmin;
        }
}

/* Normalizes values to between [-1,1] - assumes min = 0 for all axes */
static void normalize_lengths(struct Bounds bounds, struct Vec3f* vectors, int num_vectors) {

        // normalizes between [0,1]
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x = (float)vectors[i].x / bounds.xmax;
                vectors[i].y = (float)vectors[i].y / bounds.ymax;
                vectors[i].z = (float)vectors[i].z / bounds.zmax;
        }

        // scale and translate to [-1,1]
        for(int i = 0; i < num_vectors; i++){
                scale_vector(vectors + i, 2);
                translate_vector(vectors + i, -1.0f,-1.0f,-1.0f);
        }
}

/* Takes normalized vectors between [-1,1] and scales them to [-target_length/2, target_length/2] */
static void scale_lengths(float target_length, struct Bounds bounds, struct Vec3f* vectors, int num_vectors){
        for(int i = 0; i < num_vectors; i++){
                scale_vector(vectors + i, (float)target_length/2.0f);
        }
}

/*
   normalize vectors to fit in box, side length s that is centered
   on the origin all axes scaled equally.
*/
void normalize_vectors(
			float sidelength,
			struct Vec3f* vectors,
			int num_vectors
){
        struct Bounds bounds = get_bounds(vectors, num_vectors);

        shift_to_origin(bounds, vectors, num_vectors);
	bounds = get_bounds(vectors, num_vectors);

        normalize_lengths(bounds, vectors, num_vectors);
	bounds = get_bounds(vectors, num_vectors);

        scale_lengths(sidelength, bounds, vectors, num_vectors);
}
