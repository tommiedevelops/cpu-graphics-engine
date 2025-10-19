#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H

#include "vector.h"
#include "bounds.h"
#include "quaternion.h"
#include "constants.h"
#include "material.h"
#include "mesh.h"

// --- STRUCT DEFINITIONS --- 
typedef struct Transform {
	Vec3f position;
	Quat rotation;  
	Vec3f scale;
} Transform;

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

typedef struct Light {
	Vec3f direction;
	Vec4f color;
} Light;

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
GameObject* game_object_create(Transform tr, Mesh* mesh, Material* mat);

#endif
