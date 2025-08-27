#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
	result = get_rotation_matrix(tr);
	return result;

	result = get_scale_matrix(tr);
	result = mat4_mul_mat4(get_rotation_matrix(tr), result);
	result = mat4_mul_mat4(get_translation_matrix(tr), result);
	return result;
}

struct Vec4f* get_vertices_from_game_object(struct GameObject go) {
	// Prepare vertex array
	struct Vec4f* vertices = malloc(go.mesh.num_vertices*sizeof(struct Vec4f));

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


