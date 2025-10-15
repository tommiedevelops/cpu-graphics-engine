#include <stdio.h>
#include <stdlib.h>
#include "construct_plane.h"

Mesh create_square_plane() {

	float w = 0.5f;
	float l = 0.5f; 
	
	Vec3f bl = {
		.x = -w ,
		.y = 0.0f,
		.z = -l
	};

	Vec3f br = {
		.x = w ,
		.y = 0.0f,
		.z = -l
	};

	Vec3f tl = {
		.x = -w ,
		.y = 0.0f,
		.z = l
	};

	Vec3f tr = {
		.x = w ,
		.y = 0.0f,
		.z = l
	};

	Vec3f *vertices = malloc(4*sizeof(Vec3f));
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

	Vec2f* uvs = malloc(4*sizeof(Vec2f));
	uvs[0] = vec2f_create(0.0f,0.0f);
	uvs[1] = vec2f_create(0.0f,1.0f);
	uvs[2] = vec2f_create(1.0f,0.0f);
	uvs[3] = vec2f_create(1.0f,1.0f);

	Mesh mesh = {
		.num_vertices = 4,
		.vertices = vertices,
	    	.triangles = triangles,
		.num_triangles = 2,
		.uvs = uvs
	};

	return mesh;
	
}
	
Mesh create_square_plane_xy() {

	float w = 0.5f;
	float l = 0.5f; 
	
	Vec3f bl = {
		.x = -w ,
		.y = -l,
		.z = 0.0f
	};

	Vec3f br = {
		.x = w ,
		.y = -l,
		.z = 0.0f
	};

	Vec3f tl = {
		.x = -w ,
		.y = l,
		.z = 0.0f
	};

	Vec3f tr = {
		.x = w ,
		.y = l,
		.z = 0.0f 
	};

	Vec3f *vertices = malloc(4*sizeof(Vec3f));
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

	Mesh mesh = {
		.num_vertices = 4,
		.vertices = vertices,
	    	.triangles = triangles,
		.num_triangles = 2		
	};

	return mesh;
	
}

Mesh create_square_vertical_plane() {

	float w = 0.5f;
	float l = 0.5f; 
	
	Vec3f bl = {
		.x = -w,
		.y = -l,
		.z = 0.0f 
	};

	Vec3f br = {
		.x = w,
		.y = -l,
		.z = 0.0f
	};

	Vec3f tl = {
		.x = -w,
		.y = l,
		.z = 0.0f
	};

	Vec3f tr = {
		.x = w,
		.y = l,
		.z = 0.0f
	};

	Vec3f *vertices = malloc(4*sizeof(Vec3f));
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

	Vec2f* uvs = malloc(4*sizeof(Vec2f));
	uvs[0] = vec2f_create(0.0f,0.0f);
	uvs[1] = vec2f_create(0.0f,1.0f);
	uvs[2] = vec2f_create(1.0f,0.0f);
	uvs[3] = vec2f_create(1.0f,1.0f);

	Mesh mesh = {
		.num_vertices = 4,
		.vertices = vertices,
	    	.triangles = triangles,
		.num_triangles = 2,
		.uvs = uvs
	};

	return mesh;
}

