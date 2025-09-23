#include <stdio.h>
#include <stdlib.h>
#include "construct_plane.h"

struct Mesh create_square_plane() {

	float w = 0.5f;
	float l = 0.5f; 
	
	struct Vec3f bl = {
		.x = -w ,
		.y = 0.0f,
		.z = -l
	};

	struct Vec3f br = {
		.x = w ,
		.y = 0.0f,
		.z = -l
	};

	struct Vec3f tl = {
		.x = -w ,
		.y = 0.0f,
		.z = l
	};

	struct Vec3f tr = {
		.x = w ,
		.y = 0.0f,
		.z = l
	};

	struct Vec3f *vertices = malloc(4*sizeof(struct Vec3f));
	vertices[0] = bl;
	vertices[1] = tl;
	vertices[2] = br;
	vertices[3] = tr;

	int *triangles = malloc(6*sizeof(int));
	triangles[0] = 0;
	triangles[1] = 2;	
	triangles[2] = 1;
	triangles[3] = 2;
	triangles[4] = 3;
	triangles[5] = 1;

	struct Vec2f* uvs = malloc(4*sizeof(struct Vec2f));
	uvs[0] = vec2f_create(0.0f,0.0f);
	uvs[1] = vec2f_create(0.0f,1.0f);
	uvs[2] = vec2f_create(1.0f,0.0f);
	uvs[3] = vec2f_create(1.0f,1.0f);

	struct Mesh mesh = {
		.num_vertices = 4,
		.vertices = vertices,
	    	.triangles = triangles,
		.num_triangles = 2,
		.uvs = uvs
	};

	return mesh;
	
}
	
struct Mesh create_square_plane_xy() {

	float w = 0.5f;
	float l = 0.5f; 
	
	struct Vec3f bl = {
		.x = -w ,
		.y = -l,
		.z = 0.0f
	};

	struct Vec3f br = {
		.x = w ,
		.y = -l,
		.z = 0.0f
	};

	struct Vec3f tl = {
		.x = -w ,
		.y = l,
		.z = 0.0f
	};

	struct Vec3f tr = {
		.x = w ,
		.y = l,
		.z = 0.0f 
	};

	struct Vec3f *vertices = malloc(4*sizeof(struct Vec3f));
	vertices[0] = bl;
	vertices[1] = tl;
	vertices[2] = br;
	vertices[3] = tr;

	int *triangles = malloc(6*sizeof(int));
	triangles[0] = 0;
	triangles[1] = 1;	
	triangles[2] = 2;
	triangles[3] = 1;
	triangles[4] = 3;
	triangles[5] = 2;

	struct Mesh mesh = {
		.num_vertices = 4,
		.vertices = vertices,
	    	.triangles = triangles,
		.num_triangles = 2		
	};

	return mesh;
	
}

