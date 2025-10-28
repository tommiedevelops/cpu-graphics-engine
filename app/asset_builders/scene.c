#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene.h"
#include "app.h"
#include "matrix.h"

#define PI (3.14159265359)
typedef struct Scene {
	Lighting* lighting;
	Camera *cam;

	GameObject **gos;
	size_t go_len, go_cap;
	
} Scene;

Scene* scene_init() {

	// No camera, No game objects and no lighting
	int go_len = 0; int go_cap = 2;
	GameObject** gos = malloc(go_cap*sizeof(GameObject*));

	Lighting* lighting = lighting_init();

	Scene* scene = malloc(sizeof(Scene));

	if(!scene || !gos || !lighting) return NULL;

	scene->cam = NULL;	
	scene->gos = gos;
	scene->go_len = go_len;
	scene->go_cap = go_cap;
	scene->lighting = lighting;

	return scene;
}

int scene_get_num_gos(Scene* scene) {
	return scene->go_len;
}

Lighting* scene_get_lighting(Scene* scene){
	return scene->lighting;
}

Camera* scene_get_camera(Scene* scene) {
	return scene->cam;
}

void scene_add_camera(Scene* scene, Camera* cam) {
	scene->cam = cam;
}

int scene_add_game_object(Scene* scene, GameObject* go)  {

	bool at_capacity = (scene->go_cap == scene->go_len);

	if(!at_capacity) {
		scene->gos[scene->go_len++] = go;
		return scene->go_len;
	}

	int new_cap = scene->go_cap + 2;
	GameObject** new_array = realloc(scene->gos, new_cap*sizeof(GameObject*));	

	for(int i = 0; i < scene->go_len; i++) new_array[i] = scene->gos[i];

	free(scene->gos);
	scene->gos = new_array;
	scene->go_cap = new_cap;

	int go_idx = scene->go_len;
	scene->gos[go_idx] = go;
	scene->go_len++;
	return go_idx;
}

int scene_add_light(Scene* scene, Light* light) {
	int idx = lighting_add_light(scene->lighting, light);
	return idx;
}

Light* scene_get_light(Scene* scene, int idx) {
	return lighting_get_light(scene->lighting, idx);
}

GameObject* scene_get_game_object(Scene* scene, int go_idx) {
	if(go_idx < 0 || go_idx > scene->go_len) {
		printf("invalid game object idx\n");
		return NULL;
	}

	return scene->gos[go_idx];
}

void scene_uninit(Scene* scene) {

	for(int i = 0; i < scene->go_len; i++) free(scene->gos[i]);
	free(scene->gos);
	
	lighting_uninit(scene->lighting);

	free(scene->cam);

	free(scene);
}

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
Camera* camera_create(Transform tr, int width, int height){
	Camera* cam = malloc(sizeof(Camera));
	cam->transform = tr;
	cam->fov = PI*0.33f;
	cam->near = 0.01f;
	cam->far = 20.0f;
	cam->screen_width = width;
	cam->screen_height = height;
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

