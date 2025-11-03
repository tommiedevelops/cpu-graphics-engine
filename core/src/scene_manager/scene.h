#ifndef SCENE_H 
#define SCENE_H

#include "game_math/vector.h"
#include "game_math/quaternion.h"

typedef struct GameObj GameObj;

typedef struct Transform {
	Vec3f position;
	Quat rotation;  
	Vec3f scale;
} Transform;

typedef struct Camera { 
	Transform* transform;
	float fov;
	float near;
	float far;
	float screen_width;
	float screen_height;
} Camera;

typedef struct Light {
	Vec4f color;
	Vec3f direction;
} Light;

Light* light_create(Vec3f direction, Vec4f color);

typedef struct Scene Scene;

Scene*	    scene_create();
void        scene_destroy(Scene* scene);
int         scene_get_num_gos(Scene* scene);
Light*      scene_get_light(Scene* scene);
int         scene_add_light(Scene* scene, Light* light);
void        scene_add_camera(Scene* scene, Camera* cam);
Camera*     scene_get_camera(Scene* scene);
int         scene_add_game_object(Scene* scene, GameObj* go); // returns go id
GameObj*    scene_get_game_object(Scene* scene, int go_idx);
void        scene_delete_game_object(Scene* scene);

Transform* transform_create(Vec3f pos, Quat rot, Vec3f scale);
Camera* camera_create(Transform* tr, int screen_width, int screen_height);

void camera_set_fov_degrees(Camera* cam, float fov_degrees);
void camera_set_near(Camera* cam, float near);
void camera_set_far(Camera* cam, float far);

#endif
