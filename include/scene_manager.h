#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H

struct Transform {
	struct Vector3 position = {0}; // in world coords
	struct Vector3 rotation = {0}; // (0,0,0) => facing +z
	struct Vector3 scale = {1.0f, 1.0f, 1.0f};
}

struct Mesh {
	struct Vertex* vertices;
	int num_vertices;
	int* triangles;
	int num_triangles;
};

struct GameObject {
	struct Transform transform;
	struct Mesh mesh;
};

struct Scene {
	// For now, can only hold a single GameObject	
	struct GameObject gameObject;
};

#endif
