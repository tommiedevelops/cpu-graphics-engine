#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "shading.h"
#include "scene_manager.h"
#include "construct_plane.h"

struct Scene* app_create_scene();
void app_update_scene(struct Scene* scene, float delta_time, SDL_Event* event, bool* running);
void app_destroy_scene(struct Scene* scene);
#endif
