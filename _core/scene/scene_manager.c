#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene_manager.h"
#include "app.h"
#include "matrix.h"
#include "constants.h"

// Transforms 
Transform transform_create(Vec3f pos, Quat rot, Vec3f scale){
	Transform tr;
	tr.position = pos;
	tr.rotation = rot;
	tr.scale = scale;
	return tr;
}

Transform transform_default(){
	Transform tr;
	tr.position = VEC3F_0;
	tr.rotation = QUAT_IDENTITY;
	tr.scale = VEC3F_1;
	return tr;
}

// Camera
Camera* camera_create(Transform tr){
	Camera* cam = malloc(sizeof(Camera));
	cam->transform = tr;
	cam->fov = PI*0.33f;
	cam->near = 0.01f;
	cam->far = 20.0f;
	return cam;
}

void camera_set_fov_degrees(Camera* cam, float fov_degrees){

	if(fov_degrees < 0.0f) fov_degrees = 0.0f;
	if(fov_degrees > 180.0f) fov_degrees = 180.0f;

	float fov_radians = PI*fov_degrees/180.0f;	
	cam->fov = fov_radians;
}


void camera_set_near(Camera* cam, float near){
	if(near < 0.0f) near = 0.0f;
	cam->near = near;
}

void camera_set_far(Camera* cam, float far){
	cam->far = far;
}

// GameObject
GameObject* game_object_create(Transform tr, Mesh* mesh, struct Material* mat){
	GameObject* go = malloc(sizeof(GameObject));
	go->transform = tr;
	go->mesh = mesh;
	go->material = mat;
	return go;
}


Scene* scene_create(Camera* cam, GameObject** gos, int num_gos, Light light){

	Scene* scene = malloc(sizeof(Scene));
	if(NULL == scene) {
		LOG_ERROR("malloc failed"); 
		return NULL;
	}

	scene->cam = cam;
	scene->gos = gos;
	scene->num_gos = num_gos;
	scene->light = light;

	return scene;
}
