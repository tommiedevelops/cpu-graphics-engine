#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scene_manager.h"
#include "matrix.h"



struct Mat3 get_rotation_matrix(struct Transform tr) {
	float cy = cosf(tr.rotation.y);
	float sy = sinf(tr.rotation.y);

	struct Mat3 ry = {{
		{cy, 0, sy},
		{0, 1, 0},
		{-sy, 0, cy}
	}};
	
	return ry;
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

struct Mat4 get_rotation_matrix_mat4(struct Transform tr);

struct Mat4 get_model_matrix(struct Transform tr){
	
}

struct Vec3f* get_vertices_from_game_object(struct GameObject go) {
	// Prepare vertex array
	struct Vec3f* vertices = malloc(go.mesh.num_vertices*sizeof(struct Vec3f));

	// Prepare rotation matrix (in future: transformation matrix)
	struct Mat3 rotation = get_rotation_matrix(go.transform);

	// Apply rotation to each vertex
	for(int i = 0; i < go.mesh.num_vertices; i++){
		struct Vec3f vertex = go.mesh.vertices[i];
		vertices[i] = mat3_mul_vec3(rotation, vertex);
	}	

	return vertices;
}


