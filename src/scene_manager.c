#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scene_manager.h"
#include "matrix.h"


struct Mat3 get_rotation_matrix(struct Transform tr) {
	float cx = cosf(tr.rotation.x);
	float sx = sinf(tr.rotation.x);
	float cy = cosf(tr.rotation.y);
	float sy = sinf(tr.rotation.y);
	float cz = cosf(tr.rotation.z);
	float sz = sinf(tr.rotation.z);

	struct Mat3 rx = {{
		{1, 0, 0},
		{0, cx, -sx},
		{0, sx, cx}
	}};

	struct Mat3 ry = {{
		{cy, 0, sy},
		{0, 1, 0},
		{-sy, 0, cy}
	}};

	struct Mat3 rz = {{
		{cz, -sz, 0},
		{sz, cz, 0},
		{0, 0, 1}
	}};

	// Multiply in ZYX order: R = Rz * Ry * Rx
	struct Mat3 temp;
	struct Mat3 result;

	// temp = Rz * Ry
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			temp.m[i][j] = 0.0f;
			for(int k = 0; k < 3; k++) {
			temp.m[i][j] += rz.m[i][k] * ry.m[k][j];
			}
		}
	}

	// result = temp * Rx
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			result.m[i][j] = 0.0f;
			for(int k = 0; k < 3; k++) {
			result.m[i][j] += temp.m[i][k] * rx.m[k][j];
			}
		}
	}
	

	return result;
}

struct Vec3f* get_vertices_from_game_object(struct GameObject go) {
	// Prepare vertex array
	struct Vec3f* vertices = malloc(go.mesh.num_vertices*sizeof(struct Vec3f));

	if(vertices == NULL){
		perror("src/scene_manager.c/get_vertices_from_game_object: malloc failed");
		exit(EXIT_FAILURE);
	}

	if(go.mesh.vertices == NULL){
		perror("src/scene_manager.c/get_vertices_from_game_object: vertex array is null");
		exit(EXIT_FAILURE);
	}

	// Prepare rotation matrix (in future: transformation matrix)
	struct Mat3 rotation = get_rotation_matrix(go.transform);

	// Apply rotation to each vertex
	for(int i = 0; i < go.mesh.num_vertices; i++){
		struct Vec3f vertex = go.mesh.vertices[i];
		vertices[i] = mat3_mul_vec3(rotation, vertex);
	}	

	return vertices;
}


