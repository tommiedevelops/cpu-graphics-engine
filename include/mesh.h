#ifndef MESH_H
#define MESH_H
struct Mesh {
	struct Vertex* vertices;
	int num_vertices;
	int* triangles;
	int num_triangles;
};
#endif
