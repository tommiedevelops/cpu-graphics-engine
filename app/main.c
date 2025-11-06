#include <stdio.h>
#include "cpu_engine.h"

#define CLEAR_COLOR (0x87CEEBFF)
#define W_WIDTH (512)
#define W_HEIGHT (392)
#define W_NAME ("cpu graphics engine")

typedef struct GameData {
	Scene*  scene;
	Assets* assets;
} GameData;

void on_start(void* game_data) {
	GameData* gd = (GameData*)game_data;
	gd->assets = assets_create();

	gd->scene  = scene_create(NULL, NULL);
}

void on_event(void* game_data, SDL_Event* e) {
	GameData* gd = (GameData*)game_data;
}

void on_update(void* game_data, float dt) {
	GameData* gd = (GameData*)game_data;
}

void on_render(void* game_data) {
	GameData* gd = (GameData*)game_data;
}

void on_shutdown(void* game_data) {
	GameData* gd = (GameData*)game_data;
	//scene_destroy(gd->scene); NEEDA FIX
	//assets_destroy(gd->assets);
}

int main(void) {

	App app;
	GameData gd;

	AppVTable v_table = {
		on_start,
		on_event,
		on_update,
		on_render,
		on_shutdown,
		(void*)&gd
	};

	AppCfg cfg = {
		.w_width       = W_WIDTH,
		.w_height      = W_HEIGHT,
		.w_name        = W_NAME,
		.w_clear_color = CLEAR_COLOR
	};

	app_init(&app, &v_table, &cfg);

	app_run(&app);
}
