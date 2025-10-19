#include <SDL2/SDL.h>
#include "app.h"
#include "texture.h"
#include "mesh.h"

// ----- STRUCT DEFINITIONS (DO NOT MODIFY) -----
struct GameObjectContainer {
	GameObject** gos;
	int num_gos;
};

// ------ USER DEFINED SECTION ------

enum MeshHandles {
/* User Defined */
	BUNNY = 0,
	TEAPOT = 1,
	HOMER = 2	
};

enum TexHandles {
/* User Defined */
	BRICK = 0
};

// Loading & Destroying Assets 
struct TexData load_textures(){
	/* User Defined */

	int num_textures = 1;
	Texture** textures = malloc(sizeof(Texture*)*num_textures);

	Texture* tex = texture_load("./assets/textures/brickwall.png");

	textures[BRICK] = tex;

	struct TexData data = {
		.textures = textures,
		.num_textures = num_textures
	};

	return data;
}

struct MeshData load_meshes(){
	/* User Defined */

	int num_meshes = 3;
	Mesh** meshes = malloc(sizeof(Mesh*)*num_meshes);

	Mesh* homer_mesh = malloc(sizeof(Mesh));
	*homer_mesh = mesh_parse_from_obj("./assets/models/lucy.obj");
	
	Mesh* teapot_mesh = malloc(sizeof(Mesh));
	*teapot_mesh = mesh_parse_from_obj("./assets/models/teapot.obj");	

	Mesh* bunny_mesh = malloc(sizeof(Mesh));
	*bunny_mesh = mesh_parse_from_obj("./assets/models/bunny.obj");	
	mesh_recalculate_normals(bunny_mesh);

	meshes[BUNNY] = bunny_mesh;
	meshes[TEAPOT] = teapot_mesh;
	meshes[HOMER] = homer_mesh;

	struct MeshData data = {
		.meshes = meshes,
		.num_meshes = num_meshes
	};
	
	return data;
}

// --- Preparing Scene ---
struct GameObjectContainer prepare_game_objects(struct AppAssets assets){

	/* User Defined */
	Texture** textures = assets.td.textures;
	Mesh** meshes = assets.md.meshes;

	// Ground
	/* struct Material* ground_material = material_create(VEC4F_1, textures[BRICK]); */ 	
	/* Vec3f ground_pos = vec3f_create(0.0, -1.0f, 0.0f); */
	/* Vec3f ground_scale = vec3f_create(10.0f, 1.0f, 10.0f); */
	/* Transform ground_tr = transform_create(ground_pos, QUAT_IDENTITY, ground_scale); */
	/* GameObject* ground_go  = game_object_create(ground_tr,meshes[GROUND], ground_material); */

	// Bunny
	Vec4f lavender = vec4f_create(0.40,0.70,0.38,0.4);
	struct Material* bunny_material = material_create(lavender, textures[BRICK]); 	
	Vec3f bunny_pos = vec3f_create(0.0, 0.0f, 0.0f);
	Vec3f bunny_scale = vec3f_create(3.0f, 3.0f, 3.0f);

	Transform bunny_tr = transform_create(bunny_pos, QUAT_IDENTITY, bunny_scale);
	GameObject* bunny_go  = game_object_create(bunny_tr, meshes[BUNNY], bunny_material);
	int num_gos = 1;
	GameObject** gos = malloc(sizeof(GameObject*)*num_gos);
	gos[0] = bunny_go;
	/* gos[BUNNY] = bunny_go; */

	struct GameObjectContainer ctr = {
		.gos = gos,
		.num_gos = num_gos
	};

	return ctr;
}

Camera* prepare_camera(){
	/* User Defined */
	Transform tr = transform_default();
	tr.position = vec3f_create(0.0f, 0.0f, -5.0f);
	Camera* cam = camera_create(tr);
	return cam;
}

Light prepare_light_source(){
	/* User Defined */	
	Light light = {
		.direction = vec3f_create(1.0f, -1.0f, 0.0f),
		.color = VEC4F_1
	};
	return light;
}

// --- Event Handling ---
struct EventData {
	Vec3f mouse_input;
	Vec2f move_input;
};

struct EventData handle_event(SDL_Event* event, bool* running){
	/* User Can Register More Cases */
	struct EventData data = {0};

	while (SDL_PollEvent(event)) {
		switch(event->type) {
			case SDL_QUIT: running = false; break;
			case SDL_MOUSEMOTION:
				       data.mouse_input.x += event->motion.xrel;
				       data.mouse_input.y += event->motion.yrel;
				       break;
			case SDL_KEYDOWN:
				       if(event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) *running = false;
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

	Camera* cam = scene->cam;

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
	Vec3f rot_axis = vec3f_create(0.0f, -1.0f, 0.0f);
	Quat bunny_rot = quat_angle_axis(dt*bunny_ang_vel, rot_axis);

	Quat* curr_rot = &scene->gos[0]->transform.rotation;
//	*curr_rot = quat_normalize(quat_mul(*curr_rot, bunny_rot));
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

Scene* app_create_scene(struct AppAssets assets){

	struct GameObjectContainer go_ctr = prepare_game_objects(assets);
	if(NULL == go_ctr.gos){
		LOG_ERROR("gameobjects is null");
		return NULL;
	}

	Camera* cam = prepare_camera();
	if(NULL == cam){
		LOG_ERROR("cam is null");
		return NULL;
	}

	Light light = prepare_light_source();

	Scene* scene = scene_create(cam, go_ctr.gos, go_ctr.num_gos, light);
	if(NULL == scene){
		LOG_ERROR("scene is null");
		return NULL;
	}

	return scene;
}

void app_update_scene(Scene* scene, float dt, SDL_Event* event, bool* running){
	update_scene(scene, dt, event, running);
}

void app_destroy_scene(Scene* scene){
	if(NULL == scene) return;
	for(int i =0; i < scene->num_gos; i++){
		free(scene->gos[i]);		
	}
	free(scene->cam);
	free(scene);
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
