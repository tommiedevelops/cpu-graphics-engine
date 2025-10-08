#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "shading.h"
#include "scene_manager.h"
#include "construct_plane.h"

struct TexData {
	struct Texture** textures;
	int num_textures;
};

struct MeshData {
	int num_meshes;
	struct Mesh** meshes;
};

struct AppAssets {
	struct MeshData md;
	struct TexData td;
};

struct AppAssets app_load_assets();
void app_destroy_assets(struct AppAssets assets);

struct Scene* app_create_scene();
void app_update_scene(struct Scene* scene, float delta_time, SDL_Event* event, bool* running);
void app_destroy_scene(struct Scene* scene);
#endif
