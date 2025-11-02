#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


typedef struct Transform Transform;
typedef struct Mesh Mesh;
typedef struct Material Material;

typedef struct GameObj {
	Transform* tr;
	Mesh*      mesh;
	Material*  mat;
} GameObj;

GameObj* game_obj_create(Transform* tr, Mesh* mesh, Material* mat);
void game_object_destroy(GameObj* go);

#endif
