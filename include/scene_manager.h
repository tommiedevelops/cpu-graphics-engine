#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H
#include "vec3f.h"
#include "obj_parser.h"

struct Transform {
	struct Vec3f position;  // in world coords
	struct Vec3f rotation;  // (0,0,0) => facing +z
	struct Vec3f scale;
};

struct Mesh {
	struct Vec3f* vertices;
	int num_vertices;
	int* triangles;
	int num_triangles;
};

struct GameObject {
	struct Transform transform;
	struct Mesh mesh;
};

struct Material {
	struct Color color;
	struct Texture *tex;
};

struct LightSource {
	struct Vec3f direction;
};

struct Vec3f* get_vertices_from_game_object(struct GameObject go);

struct Scene {
	// For now, can only hold a single GameObject	
	struct GameObject gameObject;
};


#endif
