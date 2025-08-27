#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H
#include "vec3f.h"
#include "obj_parser.h"
#include "bounds.h"
#include "quaternion.h"

// --- STRUCT DEFINITIONS --- 
struct Transform {
	struct Vec3f position;  // in world coords
	struct Quaternion rotation;  
	struct Vec3f scale;
};

struct Camera { 
	struct Transform transform;
	float fov;
	float near_plane;
	float far_plane;
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

struct Scene {
	struct Camera cam;
	struct GameObject *gameObjects;
	struct LightSource light;
};

// --- FUNCTIONS ---

// this function basically applies a simple rotation matrix
struct Vec4f* get_vertices_from_game_object(struct GameObject go);

struct Mat4 get_model_matrix(struct Transform tr);

struct Mat4 get_view_matrix(struct Camera cam);

struct Mat4 get_projection_matrix();

struct Mat3 get_screen_space_matrix();
#endif
