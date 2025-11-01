#include <SDL2/SDL.h>
#include <signal.h>
#include <stdint.h>

#include "app.h"
#include "texture.h"
#include "mesh.h"
#include "frag_shader.h"
#include "vert_shader.h"
#include "render.h"
#include "window.h"
#include "assets_manager.h"
#include "framebuffer.h"
#include "game_object.h"

typedef struct App {
	Scene*       scene;
	Assets*      assets;
	Renderer*    renderer;
	FrameBuffer* fb;
	Window*      window;
} App;

App* app_create(Scene* s, Assets* a, Renderer* r, FrameBuffer* fb, Window* w) {
	App* app = malloc(sizeof(App));
	app->scene    = s;
	app->assets   = a;
	app->renderer = r;
	app->fb       = fb;
	app->window   = w;
	return app;
}

void app_destroy(App* app) {
	scene_destroy(app->scene);
	assets_uninit(app->assets);
	renderer_uninit(app->renderer);
	frame_buffer_destroy(app->fb);
	window_destroy(app->window);
}

static Scene* scene;
static Camera* cam;
static Light* light;

void app_init(void) {
	// USER LOGIC 
}

void app_update(float dt) {
	// USER LOGIC
}

void app_shutdown(void) {
	// USER LOGIC
}

// Loading & Destroying Assets 
struct TexData load_textures(){
	int num_textures = 1;

	Texture** textures = malloc(sizeof(Texture*));
	Texture* tex = texture_load("./app/assets/textures/brickwall.png");
	textures[0] = tex;

	struct TexData data = {
		.textures = textures,
		.num_textures = num_textures
	};

	return data;
}

struct MeshData load_meshes(){
	/* User Defined */

	int num_meshes = 1;
	Mesh** meshes = malloc(sizeof(Mesh*)*num_meshes);

	Mesh* bunny_mesh = malloc(sizeof(Mesh));
	*bunny_mesh = mesh_parse_from_obj("./assets/models/homer.obj");	
	mesh_recalculate_normals(bunny_mesh);

	meshes[0] = bunny_mesh;

	struct MeshData data = {
		.meshes = meshes,
		.num_meshes = num_meshes
	};
	
	return data;
}

// --- Preparing Scene ---
GameObj* prepare_game_objects(struct AppAssets assets){

	/* User Defined */
	Texture** textures = assets.td.textures;
	Mesh** meshes = assets.md.meshes;

	// Bunny
	Vec4f lavender = vec4f_create(0.40,0.70,0.38,0.4);
	Pipeline* p = pipeline_create(vs_default, fs_phong);
	Material* bunny_material = material_create((Vec4f){1.0,1.0,0,1.0}, NULL, p); 	
	Vec3f bunny_pos = vec3f_create(0.0, 0.0f, 0.0f);
	Vec3f bunny_scale = vec3f_create(3.0f, 3.0f, 3.0f);

	Transform* bunny_tr = transform_create(bunny_pos, QUAT_IDENTITY, bunny_scale);
	GameObj* bunny_go  = game_obj_create(bunny_tr, meshes[0], bunny_material);

	return bunny_go;
}

Camera* prepare_camera(int width, int height){
	/* User Defined */
	Transform* tr = transform_create(VEC3F_0, QUAT_IDENTITY, VEC3F_1);
	tr->position = vec3f_create(0.0f, 0.0f, -10.0f);
	Camera* cam = camera_create(*tr, width, height);
	return cam;
}

// --- Event Handling ---
struct EventData {
	Vec3f mouse_input;
	Vec2f move_input;
};

struct EventData handle_event(SDL_Event* e, bool* running){
	/* User Can Register More Cases */
	struct EventData data = {0};

	while (SDL_PollEvent(e)) {
		switch(e->type) {
			case SDL_QUIT: running = false; break;
			case SDL_MOUSEMOTION:
				       data.mouse_input.x += e->motion.xrel;
				       data.mouse_input.y += e->motion.yrel;
				       break;
			case SDL_KEYDOWN:
				       if(e->key.keysym.scancode == SDL_SCANCODE_ESCAPE) *running = false;
				       break;
			case SDL_MOUSEBUTTONDOWN:
                    		printf("Mouse Position =  (x=%d, y=%d)\n", e->button.x, e->button.y);
                    		break;
		}
	}	

	const Uint8* kb = SDL_GetKeyboardState(NULL);
	if(kb[SDL_SCANCODE_W]) data.move_input.y += 1.0f;
	if(kb[SDL_SCANCODE_A]) data.move_input.x -= 1.0f; 
	if(kb[SDL_SCANCODE_S]) data.move_input.y -= 1.0f;
	if(kb[SDL_SCANCODE_D]) data.move_input.x += 1.0f; 

	vec2f_normalize(&data.move_input);

	return data;
}

void update_scene(Scene* scene, float dt, SDL_Event* event, bool* running){
	struct EventData ed = handle_event(event, running);

	/* User Defined */
	
	// First Person Camera	
	float cam_speed = 4.0f;	
	float cam_ang_vel = 1.0f;	

	float yaw = ed.mouse_input.x * dt * cam_ang_vel;
	float pitch = ed.mouse_input.y * dt * cam_ang_vel;

	Camera* cam = scene_get_camera(scene);

	Vec3f forward = quat_get_forward(cam->transform.rotation);
	Vec3f right = quat_get_right(cam->transform.rotation);

	Quat q_yaw = quat_angle_axis(yaw, VEC3F_Y);

	cam->transform.rotation = quat_mul(q_yaw, cam->transform.rotation);
	quat_normalize(cam->transform.rotation);

	// camera translation
	Vec3f fwd_delta = vec3f_scale(forward, ed.move_input.y);
	Vec3f side_delta = vec3f_scale(right, ed.move_input.x);
	Vec3f move_vec = vec3f_add(fwd_delta, side_delta);
	Vec3f delta_pos = vec3f_scale(move_vec, cam_speed * dt);
	cam->transform.position = vec3f_add(cam->transform.position, delta_pos);

	// Spin the bunny
	float bunny_ang_vel = 2.0f;
	Vec3f rot_axis = vec3f_create(0.0f, 1.0f, 0.0f);
	Quat bunny_rot = quat_angle_axis(dt*bunny_ang_vel, rot_axis);

	GameObj* bunny_go = scene_get_game_object(scene,0);
	Quat* curr_rot = &bunny_go->tr->rotation;
	*curr_rot = quat_normalize(quat_mul(*curr_rot, bunny_rot));
}

// ------ API USED BY CORE (DO NOT MODIFY) ------
struct AppAssets app_load_assets(){

	struct MeshData md = load_meshes();
	struct TexData td = load_textures();

	struct AppAssets assets;
	assets.md = md;
	assets.td = td;
	
	return assets;
}

Scene* app_create_scene(struct AppAssets assets, int width, int height){

	Camera* cam = prepare_camera(width, height);
	Light* light = light_create((Vec3f){-1.0f,-1.0f,1.0f}, VEC4F_1);

	Scene* scene = scene_create(cam, light);
	scene_add_game_object(scene, prepare_game_objects(assets));

	if(NULL == scene) return NULL;

	return scene;
}

void app_update_scene(Scene* scene, float dt, SDL_Event* event, bool* running){
	update_scene(scene, dt, event, running);
}

void app_destroy_scene(Scene* scene){
	scene_destroy(scene);
}

void destroy_textures(struct TexData textures) {

	for(int i = 0; i < textures.num_textures; i++){
		free(textures.textures[i]);
	}

	free(textures.textures);
}

void destroy_meshes(struct MeshData meshes) {
	for(int i = 0; i < meshes.num_meshes; i++){
		free(meshes.meshes[i]);
	}

	free(meshes.meshes);
}

void app_destroy_assets(struct AppAssets assets){
	destroy_meshes(assets.md);
	destroy_textures(assets.td);
}
