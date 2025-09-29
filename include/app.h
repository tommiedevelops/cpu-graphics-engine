#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "shading.h"
#include "scene_manager.h"

struct Assets {
	struct Mesh* meshes;
	struct Texture* textures;
};

struct InputData {
	struct Vec2f mouse_input;
	struct Vec2f move_input;
};

struct Assets app_load_assets(){
	// User Defined
}

struct Scene app_create_scene(){
	// User Defined		
}

void app_update_scene(struct Scene* scene, float delta_time, SDL_Event* event){
	// User Defined	
}

void app_destroy_assets(struct Assets* assets){
	//TODO
}

void app_destroy_scene(struct Scene* scene){
	//TODO
}

void update_inputs(SDL_Event* event, struct InputData* input) {
	//TODO
}
#endif
