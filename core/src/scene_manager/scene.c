#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene_manager/scene.h"
#include "game_math/matrix.h"
#include "scene_manager/game_object.h"

#define PI (3.14159265359)

typedef struct Scene {
	Light* light;
	Camera *cam;

	GameObj **gos;
	size_t go_len, go_cap;
	
} Scene;

Scene* scene_create(Camera* cam, Light* light) {
	// No camera, No game objects and no lighting
	int go_len = 0; int go_cap = 2;
	GameObj** gos = malloc(go_cap*sizeof(GameObj*));

	Scene* scene = malloc(sizeof(Scene));

	if(!scene || !gos || !light) return NULL;

	scene->cam    = cam;
	scene->light  = light;
	scene->gos    = gos;
	scene->go_len = go_len;
	scene->go_cap = go_cap;

	return scene;
}

void core_scene_save(Scene* scene, const char* filepath){
	//TODO
}

int scene_get_num_gos(Scene* scene) {
	return scene->go_len;
}

Light* light_create(Vec3f direction, Vec4f color) {
	Light* l = malloc(sizeof(Light));
	l->direction = direction;
	l->color = color;
	return l;
}

Camera* scene_get_camera(Scene* scene) {
	return scene->cam;
}

void scene_add_camera(Scene* scene, Camera* cam) {
	scene->cam = cam;
}

int scene_add_game_object(Scene* scene, GameObj* go)  {

	bool at_capacity = (scene->go_cap == scene->go_len);

	if(!at_capacity) {
		scene->gos[scene->go_len++] = go;
		return scene->go_len;
	}

	int new_cap = scene->go_cap + 2;
	GameObj** new_array = realloc(scene->gos, new_cap*sizeof(GameObj*));	

	for(int i = 0; i < scene->go_len; i++) new_array[i] = scene->gos[i];

	free(scene->gos);
	scene->gos = new_array;
	scene->go_cap = new_cap;

	int go_idx = scene->go_len;
	scene->gos[go_idx] = go;
	scene->go_len++;
	return go_idx;
}

inline Light* scene_get_light(Scene* scene) { return scene->light; } 

GameObj* scene_get_game_object(Scene* scene, int go_idx) {
	if(go_idx < 0 || go_idx > scene->go_len) {
		printf("invalid game object idx\n");
		return NULL;
	}

	return scene->gos[go_idx];
}

void scene_destroy(Scene* scene) {
	for(int i = 0; i < scene->go_len; i++) free(scene->gos[i]);
	free(scene->gos);
	free(scene->light);
	free(scene->cam);
	free(scene);
}

// Transforms 
Transform* transform_create(Vec3f pos, Quat rot, Vec3f scale){
	Transform* tr = malloc(sizeof(Transform));
	tr->position = pos;
	tr->rotation = rot;
	tr->scale = scale;
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

