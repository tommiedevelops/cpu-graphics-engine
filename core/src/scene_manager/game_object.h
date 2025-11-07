#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

typedef struct Transform Transform;
typedef struct Mesh Mesh;
typedef struct Material Material;

typedef struct GameObj {
	Transform* tr;
	Mesh*      mesh;
	Material*  mat;
	int id;
	const char * handle;
} GameObj;

GameObj*  game_obj_create(Transform* tr, Mesh* mesh, Material* mat);
void      game_obj_set_transform(GameObj* go, Transform* tr);
void      game_obj_set_mesh(GameObj* go, Mesh* mesh);
void      game_obj_set_material(GameObj* go, Material* mat);
void      game_obj_set_id(GameObj* go, int id);
Mesh*     game_obj_get_mesh(GameObj* go);
Material* game_obj_get_material(GameObj* go);
int       game_obj_get_id(GameObj* go);
void      game_obj_set_handle(GameObj* go, const char* handle);
void      game_obj_destroy(GameObj* go);
#endif
