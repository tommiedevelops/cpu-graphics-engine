#ifndef MESH_H
#define MESH_H

typedef struct Vec3f Vec3f;
typedef struct Vec2f Vec2f;

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

int mesh_recalculate_normals(Mesh* m);
Mesh mesh_parse_from_obj(char* obj_filename);
#endif
