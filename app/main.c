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

void on_start(App* app, void* game_data) {
	GameData* gd = (GameData*)game_data;

	// Assets Init
	Assets* assets  = assets_create();
	Mesh* mesh = mesh_parse_from_obj("assets/models/bunny.obj");
	assets_add_mesh(assets, mesh);
	mesh_recalculate_normals(mesh);

	// Scene Init
	Vec4f col = (Vec4f){0.1, 0.2, 0.3, 1.0};
	Pipeline* p = pipeline_create(vs_default, fs_phong);

	Material* mat = material_create(col, NULL, p);
	Transform* bunny_tr = transform_default();
	GameObj* bunny = game_obj_create(bunny_tr, mesh, mat);

	Transform* cam_tr = transform_default();
	cam_tr->position = (Vec3f){0.0f, 0.0f, -5.0f};

	Camera* cam = camera_create(cam_tr, W_WIDTH, W_HEIGHT);
	Light* light = light_create(VEC3F_1, VEC4F_1);
	Scene* scene = scene_create(cam, light);

	scene_add_game_obj(scene, bunny);

	app->scene = scene;
	app->assets = assets;
}

void on_event(App* app, void* game_data, SDL_Event* e) {
	GameData* gd = (GameData*)game_data;
}

void on_render(App* app, void* game_data){
	GameData* gd = (GameData*)game_data;
}

void on_update(App* app, void* game_data, float dt) {
	GameData* gd = (GameData*)game_data;
}

void on_shutdown(App* app, void* game_data) {
	GameData* gd = (GameData*)game_data;
	//scene_destroy(gd->scene); NEEDA FIX
	//assets_destroy(gd->assets);
}

int main(void) {

	App         app;
	GameData     gd;

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
