#ifndef SCENE_MANAGER_H 
#define SCENE_MANAGER_H

#include "vector.h"
#include "bounds.h"
#include "quaternion.h"
#include "constants.h"
#include "material.h"
#include "mesh.h"
#include "lighting.h"

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
	float screen_width;
	float screen_height;
} Camera;

typedef struct Scene Scene;

Scene*      scene_init();
void        scene_uninit(Scene* scene);
int         scene_get_num_gos(Scene* scene);
Light*      scene_get_light(Scene* scene, int light_idx);
int         scene_add_light(Scene* scene, Light* light);
Camera*     scene_get_camera(Scene* scene);
int         scene_add_game_object(Scene* scene, GameObject* go); // returns go id
GameObject* scene_get_game_object(Scene* scene, int go_idx);
void        scene_delete_game_object(Scene* scene);
void        scene_uninit(Scene* scene);

GameObject* game_object_create(Transform tr, Mesh* mesh, Material* mat);

Transform transform_create(Vec3f pos, Quat rot, Vec3f scale);
Camera* camera_create(Transform tr);
Transform transform_default();

void camera_set_fov_degrees(Camera* cam, float fov_degrees);
void camera_set_near(Camera* cam, float near);
void camera_set_far(Camera* cam, float far);

#endif
