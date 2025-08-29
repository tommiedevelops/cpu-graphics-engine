#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "scene_manager.h"
#include "matrix.h"

struct Mat4 get_rotation_matrix(struct Transform tr) {
	return quat_to_mat4(tr.rotation);
}

struct Mat4 get_scale_matrix(struct Transform tr) {
	struct Vec3f scale = tr.scale;
	struct Mat4 scale_matrix = {{
		{scale.x, 0,       0,       0},
		{0,       scale.y, 0,       0},
		{0,       0,       scale.z, 0},
		{0,       0,       0,       1.0f},
	}};
	return scale_matrix;
}

struct Mat4 get_translation_matrix(struct Transform tr) {
	struct Vec3f pos = tr.position;
    	struct Mat4 translation_matrix = {{
       		{1, 0, 0, pos.x},
        	{0, 1, 0, pos.y},
        	{0, 0, 1, pos.z},
        	{0, 0, 0, 1    },
    	}};
    	return translation_matrix;	
}


struct Mat4 get_model_matrix(struct Transform tr){
	struct Mat4 result;
	result = get_scale_matrix(tr);
	result = mat4_mul_mat4(get_rotation_matrix(tr), result);
	result = mat4_mul_mat4(get_translation_matrix(tr), result);
	return result;
}

struct Vec4f* get_vertices_from_game_object(struct GameObject go) {
	// Prepare vertex array
	struct Vec4f* vertices = malloc(go.mesh.num_vertices*sizeof(struct Vec4f));
	memset(vertices, 0x0, go.mesh.num_vertices*sizeof(struct Vec4f));

	struct Mat4 model_matrix = get_model_matrix(go.transform);
	
	for(int i = 0; i < go.mesh.num_vertices; i++){
		struct Vec3f v = go.mesh.vertices[i];
		vertices[i].x = v.x;
		vertices[i].y = v.y;
		vertices[i].z = v.z;
		vertices[i].w = 1.0f;

		vertices[i] = mat4_mul_vec4(model_matrix, vertices[i]);
	}	

	return vertices;
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
void normalize_vertices(
			float sidelength,
			struct Vec3f* vertices,
			int num_vertices
){
        struct Bounds bounds = get_bounds(vertices, num_vertices);

        shift_to_origin(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        normalize_lengths(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        scale_lengths(sidelength, bounds, vertices, num_vertices);
}
