#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene_manager.h"
#include "app.h"
#include "matrix.h"
#include "constants.h"

typedef struct Scene {
	Camera *cam;

	GameObject **gos;
	size_t go_len, go_cap;

	Light **lights;
	size_t l_len, l_cap;

} Scene;

Scene* scene_init() {

	Camera* cam = camera_create(transform_default());

	int go_len = 0; int go_cap = 2;
	GameObject** gos = malloc(go_cap*sizeof(GameObject*));

	int l_len = 1; int l_cap = 2;
	Light** lights = malloc(l_cap*sizeof(Light*)); 
	lights[0] = light_default();

	Scene* scene = malloc(sizeof(Scene));

	if(!scene || !cam || !gos || !lights) return NULL;

	scene->cam = cam;	
	scene->gos = gos;
	scene->go_len = go_len;
	scene->go_cap = go_cap;
	scene->lights = lights;
	scene->l_len = l_len;
	scene->l_cap = l_cap;

	return scene;
}

int scene_get_num_gos(Scene* scene) {
	return scene->go_len;
}

int scene_get_num_lights(Scene* scene){
	return scene->l_len;
}
Camera* scene_get_camera(Scene* scene) {
	return scene->cam;
}

int scene_add_light(Scene* scene, Light* light) {

	bool at_capacity = (scene->l_cap == scene->l_len);

	if(!at_capacity) {
		scene->lights[scene->l_len++] = light;
		return scene->l_len;
	}

	int new_cap = scene->l_cap + 2;
	Light** new_array = realloc(scene->lights, new_cap*sizeof(Light*));	

	for(int i = 0; i < scene->l_len; i++) new_array[i] = scene->lights[i];

	free(scene->lights);
	scene->lights = new_array;
	scene->l_cap = new_cap;

	int light_idx = scene->l_len;
	scene->lights[light_idx] = light;
	scene->l_len++;

	return light_idx;
}

Light* scene_get_light(Scene* scene, int l_idx){
	if(l_idx < 0 || l_idx > scene->l_len) {
		printf("invalid light idx\n");
		return NULL;
	}

	return scene->lights[l_idx];
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

	for(int i = 0; i < scene->l_len; i++) free(scene->lights[i]);
	free(scene->lights);

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

