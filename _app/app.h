#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <SDL2/SDL.h>

#include "normals.h"
#include "material.h"
#include "scene_manager.h"

struct TexData {
	struct Texture** textures;
	int num_textures;
};

struct MeshData {
	int num_meshes;
	Mesh** meshes;
};

struct AppAssets {
	struct MeshData md;
	struct TexData td;
};

struct AppAssets app_load_assets();
void app_destroy_assets(struct AppAssets assets);

Scene* app_create_scene();
void app_update_scene(Scene* scene, float delta_time, SDL_Event* event, bool* running);
void app_destroy_scene(Scene* scene);
#endif
