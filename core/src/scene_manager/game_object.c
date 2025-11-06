#include <stdlib.h>

#include "game_object.h"
#include "error_log.h"

GameObj* game_obj_create(Transform* tr, Mesh* mesh, Material* mat){
	GameObj* go = malloc(sizeof(GameObj));
	go->tr   = tr;
	go->mesh = mesh;
	go->mat  = mat;
	return go;
}

void game_obj_set_transform(GameObj* go, Transform* tr) {
	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	go->tr = tr;
}

void game_obj_set_mesh(GameObj* go, Mesh* mesh) {
	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	go->mesh = mesh;
}

void game_obj_set_material(GameObj* go, Material* mat) {
	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	go->mat = mat;
}

void game_obj_set_id(GameObj* go, int id) {
	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	go->id = id;
}
Mesh* game_obj_get_mesh(GameObj* go) {
	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return NULL;
	}

	return go->mesh;
}

Material* game_obj_get_material(GameObj* go) {

	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return NULL;
	}

	return go->mat;
}
int game_obj_get_id(GameObj* go) {

	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return -1;
	}

	return go->id;
}
void game_obj_destroy(GameObj* go) {

	if(!go) {
		LOG_ERROR(NULL_PARAM_ERROR);
		return;
	}

	free(go);
}


