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


