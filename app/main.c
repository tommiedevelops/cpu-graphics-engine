#include <stdio.h>
#include "cpu_engine.h"

#define CLEAR_COLOR (0x87CEEBFF)
#define W_WIDTH (512)
#define W_HEIGHT (392)
#define W_NAME ("cpu graphics engine")

typedef struct GameData {
	Vec2f mouse_input;
	Vec2f move_input;
} GameData;

void on_init(App* app, void* game_data) {
	/* for preparing Assets */
	Assets* assets  = assets_create();

	Mesh* mesh = mesh_parse_from_obj("assets/models/homer.obj");
	assets_add_mesh(assets, mesh, "bunny");

	mesh_recalculate_normals(mesh);

	Vec4f col = (Vec4f){0.1, 0.2, 0.3, 1.0};
	Pipeline* p = pipeline_create(vs_default, fs_lit);
	Material*  mat = material_create(col, NULL, p);
	assets_add_material(assets, mat, "bunny");

	app->assets = assets;
}

void on_start(App* app, void* game_data) {
	/* for preparing Scene */
	GameData* gd = (GameData*)game_data;
	
	gd->move_input = VEC2F_0;
	gd->mouse_input = VEC2F_0;

	Transform* cam_tr = transform_default();
	cam_tr->position = (Vec3f){0.0f, 0.0f, -5.0f};

	Camera* cam  = camera_create(cam_tr, W_WIDTH, W_HEIGHT);
	Light* light = light_create(VEC3F_1, VEC4F_1);
	Scene* scene = scene_create(cam, light);

	const char * bunny_handle = "bunny";

	Transform* bunny_tr   = transform_default();
	Mesh*      bunny_mesh = assets_get_mesh(app->assets, bunny_handle);
	Material*  bunny_mat  = assets_get_material(app->assets, bunny_handle);

	GameObj* bunny = game_obj_create(bunny_tr, bunny_mesh, bunny_mat);

	scene_add_game_obj(scene, bunny, bunny_handle);
	app->scene = scene;
}

void on_event(App* app, void* game_data, SDL_Event* e) {
	GameData* gd = (GameData*)game_data;

	switch(e->type) {
		case SDL_MOUSEMOTION:
			gd->mouse_input.x += e->motion.xrel;
			gd->mouse_input.y += e->motion.yrel;
			break;
		case SDL_KEYDOWN:
			if(e->key.keysym.scancode == SDL_SCANCODE_ESCAPE) app_shutdown(app);
			break;
		case SDL_MOUSEBUTTONDOWN:
			printf("Mouse Position =  (x=%d, y=%d)\n", e->button.x, e->button.y);
			break;
	}
}

static inline bool approx(float x, float y, float eps) {
	return abs(x-y) < eps;
}

static void handle_movement(Transform* cam_tr, GameData* gd, float dt) {

	const Uint8* kb = SDL_GetKeyboardState(NULL);
	if(kb[SDL_SCANCODE_W]) gd->move_input.x += 1.0f;
	if(kb[SDL_SCANCODE_A]) gd->move_input.y -= 1.0f;
	if(kb[SDL_SCANCODE_S]) gd->move_input.x -= 1.0f;
	if(kb[SDL_SCANCODE_D]) gd->move_input.y += 1.0f; 

	float horiz_sens = 0.5f;

	Quat rot = quat_angle_axis(gd->mouse_input.x * horiz_sens * dt, VEC3F_Y);
	transform_apply_rotation(cam_tr, rot);

	Vec3f forward  = quat_get_forward(cam_tr->rotation);
	Vec3f right    = quat_get_right(cam_tr->rotation);

	Vec3f fwd_back   = vec3f_scale(forward, gd->move_input.x);
	Vec3f left_right = vec3f_scale(right, gd->move_input.y);

	Vec3f move_dir = vec3f_add(fwd_back, left_right);

	if(!approx(vec3f_magnitude(move_dir), 0.0f, 0.001)) 
		move_dir = vec3f_normalize(move_dir);

	float speed = 2.0f;

	Vec3f movement = vec3f_scale(move_dir, speed * dt);

	transform_apply_translation(cam_tr, movement);

	// Reset movement input
	gd->move_input = VEC2F_0;
	gd->mouse_input = VEC2F_0;
}

static void rotate_bunny(GameObj* go, float dt) {
	float ang_vel = 2.0f;
	Transform* tr = go->tr;
	float angle = dt * ang_vel;
	Vec3f axis = VEC3F_1;
	Quat rot = quat_angle_axis(angle, axis);
	transform_apply_rotation(tr,rot);
}

void on_update(App* app, void* game_data, float dt) {
	GameData* gd = (GameData*)game_data;

	Scene* scene = app->scene;
	Camera* cam = scene_get_camera(scene);

	handle_movement(cam->transform, gd, dt);

	GameObj* bunny_go = scene_get_game_obj(scene, "bunny");

	rotate_bunny(bunny_go, dt);
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
		on_init,
		on_start,
		on_event,
		on_update,
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
