#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H

#include "vector.h"
#include "bounds.h"
#include "quaternion.h"
#include "constants.h"
#include "shading.h"

// --- STRUCT DEFINITIONS --- 
typedef struct Transform {
	Vec3f position;  // in world coords
	Quat rotation;  
	Vec3f scale;
} Transform;

typedef struct Mesh {
	int num_vertices;
	Vec3f* vertices;
	int num_uvs;
	Vec2f* uvs;
	int num_normals;
	Vec3f* normals;
	int num_triangles;
	int* triangle_uvs;
	int* triangles;
	int* triangle_normals;
} Mesh;

typedef struct GameObject {
	Transform transform;
	Mesh* mesh;
	Material* material;
} GameObject;

typedef struct Camera { 
	Transform transform;
	float fov;
	float near;
	float far;
} Camera;

typedef struct Scene {
	Camera *cam;
	GameObject **gos;
	int num_gos;
	Light light;
} Scene;

Transform transform_create(Vec3f pos, Quat rot, Vec3f scale);
Scene* scene_create(Camera* cam, GameObject** gos, int num_gos, Light light);
Camera* camera_create(Transform tr);
Transform transform_default();
void camera_set_fov_degrees(Camera* cam, float fov_degrees);
void camera_set_near(Camera* cam, float near);
void camera_set_far(Camera* cam, float far);
GameObject* game_object_create(Transform tr, Mesh* mesh, struct Material* mat);

Mesh parse_obj(char* filename);
#endif
