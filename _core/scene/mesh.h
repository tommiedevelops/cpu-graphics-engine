#ifndef MESH_H
#define MESH_H

typedef struct Mesh {
	int num_vertices;
	Vec3f* vertices;
	int* triangles;

	int num_uvs;
	Vec2f* uvs;
	int* triangle_uvs;

	int num_normals;
	Vec3f* normals;
	int* triangle_normals;

	int num_triangles;
} Mesh;

Mesh parse_obj(char* filename);
#endif
