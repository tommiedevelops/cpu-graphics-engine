#include <stdlib.h>

#include "game_object.h"

GameObj* game_obj_create(Transform* tr, Mesh* mesh, Material* mat){
	GameObj* go = malloc(sizeof(GameObj));
	go->tr   = tr;
	go->mesh = mesh;
	go->mat  = mat;
	return go;
}
