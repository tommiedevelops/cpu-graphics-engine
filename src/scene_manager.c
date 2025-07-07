#include <stdio.h>
#include <stdlib.h>

#include "scene_manager.h"
#include "matrix.h"


struct Mat3 get_rotation_matrix(struct Transform tr){
}

struct Vertex* get_vertices_from_game_object(struct GameObject go) {
	// Prepare vertex array
	struct Vertex* vertices = malloc(go.mesh.num_vertices*sizeof(struct Vertex));

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
		vertices[i] = mat3_mul_vec3(rotation, go.mesh.vertices[i]);
	}	

	return vertices;
}


