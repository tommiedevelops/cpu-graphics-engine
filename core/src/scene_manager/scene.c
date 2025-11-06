#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "scene_manager/scene.h"
#include "game_math/matrix.h"
#include "scene_manager/game_object.h"
#include "scene_manager/transform.h"
#include "scene_manager/camera.h"
#include "scene_manager/light.h"
#include "error_log.h"

#define PI (3.14159265359)

typedef struct Scene {
	Light* light;
	Camera *cam;
	GameObj **gos;
	size_t go_len, go_cap;
} Scene;

Scene* scene_create(Camera* cam, Light* light) {

	if(!cam || !light) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return NULL;
	}

	int go_len = 0; int go_cap = 2;
	GameObj** gos = malloc(go_cap*sizeof(GameObj*));
	Scene* scene = malloc(sizeof(Scene));

	if(!gos || !scene) {
		LOG_ERROR(MALLOC_ERROR);
		return NULL;
	}

	scene->cam    = cam;
	scene->light  = light;
	scene->gos    = gos;
	scene->go_len = go_len;
	scene->go_cap = go_cap;

	return scene;
}

int scene_get_num_gos(Scene* scene) {

	if(!scene) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return -1;
	}

	return scene->go_len;
}

Camera* scene_get_camera(Scene* scene) {
	if(!scene) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return NULL;
	}

	return scene->cam;
}

void scene_set_camera(Scene* scene, Camera* cam) {

	if(!scene || !cam) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	scene->cam = cam;
}

static int add_go_to_scene(Scene* scene, GameObj* go){
	scene->gos[scene->go_len] = go;
	game_obj_set_id(go, scene->go_len);
	return scene->go_len++;
}

static void realloc_go_array(Scene* scene){

	int new_cap = scene->go_cap + 2;
	GameObj** new_array = realloc(scene->gos, new_cap*sizeof(GameObj*));	

	for(int i = 0; i < scene->go_len; i++)
	       	new_array[i] = scene->gos[i];

	free(scene->gos);
	scene->gos    = new_array;
	scene->go_cap = new_cap;
}

static inline bool at_capacity(Scene* scene){
	return (scene->go_cap == scene->go_len);
}

int scene_add_game_obj(Scene* scene, GameObj* go)  {

	if(!scene || !go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return -1;
	}

	if( !at_capacity(scene) ) 
		return add_go_to_scene(scene, go);

	realloc_go_array(scene);

	return add_go_to_scene(scene, go);
}

Light* scene_get_light(Scene* scene) { return scene->light; } 

GameObj* scene_get_game_obj(Scene* scene, int go_idx) {
	if(go_idx < 0 || go_idx > scene->go_len) {
		printf("invalid game object idx\n");
		return NULL;
	}

	return scene->gos[go_idx];
}

void scene_destroy(Scene* scene) {
	for(int i = 0; i < scene->go_len; i++) 
		game_obj_destroy(scene->gos[i]);
	free(scene->gos);
	free(scene);
}

