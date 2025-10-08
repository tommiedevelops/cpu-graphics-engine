#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene_manager.h"
#include "app.h"
#include "matrix.h"
#include "constants.h"
#include "construct_plane.h"

// Transforms 
struct Transform transform_create(struct Vec3f pos, struct Quaternion rot, struct Vec3f scale){
	struct Transform tr;
	tr.position = pos;
	tr.rotation = rot;
	tr.scale = scale;
	return tr;
}

struct Transform transform_default(){
	struct Transform tr;
	tr.position = VEC3F_0;
	tr.rotation = QUAT_IDENTITY;
	tr.scale = VEC3F_1;
	return tr;
}

// Camera
struct Camera* camera_create(struct Transform tr){
	struct Camera* cam = malloc(sizeof(struct Camera));
	cam->transform = tr;
	cam->fov = PI*0.33f;
	cam->near = 0.01f;
	cam->far = 20.0f;
	return cam;
}

void camera_set_fov_degrees(struct Camera* cam, float fov_degrees){

	if(fov_degrees < 0.0f) fov_degrees = 0.0f;
	if(fov_degrees > 180.0f) fov_degrees = 180.0f;

	float fov_radians = PI*fov_degrees/180.0f;	
	cam->fov = fov_radians;
}


void camera_set_near(struct Camera* cam, float near){
	if(near < 0.0f) near = 0.0f;
	cam->near = near;
}

void camera_set_far(struct Camera* cam, float far){
	cam->far = far;
}

// GameObject
struct GameObject* game_object_create(struct Transform tr, struct Mesh* mesh, struct Material* mat){
	struct GameObject* go = malloc(sizeof(struct GameObject));
	go->transform = tr;
	go->mesh = mesh;
	go->material = mat;
	return go;
}


struct Scene* scene_create(struct Camera* cam, struct GameObject** gos, int num_gos, struct LightSource ls){

	struct Scene* scene = malloc(sizeof(struct Scene));
	if(NULL == scene) {
		LOG_ERROR("malloc failed"); 
		return NULL;
	}

	scene->cam = cam;
	scene->gos = gos;
	scene->num_gos = num_gos;
	scene->light = ls;

	return scene;
}
