#ifndef SCENE_H 
#define SCENE_H

#include "game_math/vector.h"
#include "game_math/quaternion.h"

typedef struct GameObj GameObj;
typedef struct Camera Camera;
typedef struct Light Light;
typedef struct Scene Scene;

Scene*	    scene_create(Camera* cam, Light* light);
void        scene_destroy(Scene* scene);
int         scene_get_num_gos(Scene* scene);
Light*      scene_get_light(Scene* scene);
int         scene_set_light(Scene* scene, Light* light);
void        scene_set_camera(Scene* scene, Camera* cam);
Camera*     scene_get_camera(Scene* scene);
int         scene_add_game_obj(Scene* scene, GameObj* go, const char * handle); // returns go id
GameObj*    scene_get_game_obj(Scene* scene, const char * handle);
GameObj**   scene_get_game_obj_arr(Scene* scene);
void        scene_delete_game_object(Scene* scene);
int         scene_get_num_game_obj(Scene* scene);

#endif
