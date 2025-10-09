#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H

#include "vector.h"
#include "obj_parser.h"
#include "bounds.h"
#include "quaternion.h"
#include "constants.h"

// --- STRUCT DEFINITIONS --- 
struct Transform {
	struct Vec3f position;  // in world coords
	struct Quaternion rotation;  
	struct Vec3f scale;
};

struct Camera { 
	struct Transform transform;
	float fov;
	float near;
	float far;
};

struct Scene {
	struct Camera *cam;
	struct GameObject **gos;
	int num_gos;
	struct LightSource light;
};

struct Transform transform_create(struct Vec3f pos, struct Quaternion rot, struct Vec3f scale);
struct Transform transform_default();

struct Scene* scene_create(struct Camera* cam, struct GameObject** gos, int num_gos, struct LightSource ls);

struct Camera* camera_create(struct Transform tr);

void camera_set_fov_degrees(struct Camera* cam, float fov_degrees);
void camera_set_near(struct Camera* cam, float near);
void camera_set_far(struct Camera* cam, float far);

struct Mesh {
	struct Vec3f* vertices;
	struct Vec2f* uvs;
	int num_vertices;
	int* triangles;
	int* triangle_uvs;
	int num_triangles;
	int num_uvs;
};

struct GameObject {
	struct Transform transform;
	struct Mesh* mesh;
	struct Material* material;
};

struct GameObject* game_object_create(struct Transform tr, struct Mesh* mesh, struct Material* mat);

void normalize_vertices(float sidelength, struct Vec3f* vertices, int num_vertices);

#endif
