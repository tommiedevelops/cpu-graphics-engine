#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "render.h"
#include "obj_parser.h"
#include "triangle.h"
#include "window.h"
#include "scene_manager.h"
#include "game_time.h"
#include "quaternion.h"
#include "construct_plane.h"
#include "shading.h"

int main(void) {
	
	// Loading Assets
	struct Mesh  mesh        = parse_obj("./assets/models/bunny.obj");
	struct Mesh  teapot_mesh = parse_obj("./assets/models/teapot.obj");
	struct Mesh  ground_mesh = create_square_plane();

	struct Texture tex = texture_load("./assets/textures/brickwall.png");

	// creating materials
	struct Vec4f pink  = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f green = vec4f_create(0.2f, 0.8f, 0.2f, 1.0f);
	struct Vec4f blue  = vec4f_create(0.2f, 0.2f, 0.8f, 1.0f);

	struct Material bunny_material  = material_create(pink, NULL);
	struct Material teapot_material = material_create(green, NULL);
	struct Material dragon_material = material_create(blue, NULL);
	struct Material ground_material = material_create(VEC4F_1, &tex); 	

	// creating game_objects
	struct Vec3f      ground_scale = {.x = 5.0f, .y = 1.0f, .z = 5.0f};
	struct Transform  ground_tr    = transform_create(VEC3F_0, QUAT_IDENTITY, ground_scale);
	struct GameObject ground_go    = game_object_create(ground_tr, &ground_mesh, &ground_material);
		
	struct Vec3f      pos0      = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  transform = transform_create(pos0, QUAT_IDENTITY, VEC3F_1);
	struct GameObject go        = game_object_create(transform, &mesh, &bunny_material);

	struct Vec3f      pos1 = {.x = 2.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  tr1  = transform_create(pos1, QUAT_IDENTITY, VEC3F_1);
	struct GameObject go1  = game_object_create(tr1, &teapot_mesh, &teapot_material);

	struct Vec3f      pos2 = {.x = -2.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  tr2  = transform_create(pos2, QUAT_IDENTITY, VEC3F_1);
	struct GameObject go2  = game_object_create(tr2, &mesh, &dragon_material);
	
	// game_object array
	int num_gameObjects = 4;
	struct GameObject* gameObjects[num_gameObjects];
	gameObjects[0] = &go;
	gameObjects[1] = &go1;
	gameObjects[2] = &go2;
	gameObjects[3] = &ground_go;

	// creating & configuring camera
	
	struct Transform camera_transform = transform_create(vec3f_create(0.0f, 0.5f, 3.0f), QUAT_IDENTITY, VEC3F_1);
	struct Camera cam = camera_create(camera_transform);
	camera_set_fov_degrees(&cam, 60.0f); //remember to convert to radians in function 
	camera_set_near(&cam, 5.0f);
	camera_set_far(&cam, 20.0f);

	// creating scene
	struct Scene scene = {
		.cam = &cam,
	       	.gameObjects = gameObjects, 
		.num_gameObjects =num_gameObjects
	};
	
	// Initialize time struct
	struct Time time;
	time_init(&time);

	// Initialize framebuffer and z-buffer
	uint32_t framebuffer[WIDTH * HEIGHT] = {0};
	float zbuffer[WIDTH * HEIGHT] = {0};

        /* Prepare Window using SDL */
        struct SDL_Data window_data = initialise_window();

        /* Render Loop */
        bool running = true;
        SDL_Event event;
        while(running) {

		// Handle Time
		update_time(&time);
		/* print_fps(&time); */

		// Clear the buffers 
		memset(framebuffer, 0x0, sizeof(framebuffer));
		memset(zbuffer, 0x0, sizeof(zbuffer));

                // Event Handling
		int mouse_dx = 0;
		int mouse_dy = 0;

                while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_MOUSEMOTION:
					mouse_dx += event.motion.xrel;
					mouse_dy += event.motion.yrel;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
					break;
			}
                }	

		// input handling 	
		const Uint8* kb = SDL_GetKeyboardState(NULL);

		struct Vec3f move_dir = {0};
		struct Vec3f move_vec = {0};
		float cam_speed = -1.0f;

		if(kb[SDL_SCANCODE_W]) {
			move_dir = quat_get_forward(cam.transform.rotation);
			move_vec = vec3f_scale(move_dir, cam_speed * time.delta_time);
		}

		if(kb[SDL_SCANCODE_A]) {
			move_dir = vec3f_scale(quat_get_right(cam.transform.rotation), -1.0f);
			move_vec = vec3f_scale(move_dir, cam_speed * time.delta_time);
		}

		if(kb[SDL_SCANCODE_S]) {
			move_dir = vec3f_scale(quat_get_forward(cam.transform.rotation), -1.0f);
			move_vec = vec3f_scale(move_dir, cam_speed * time.delta_time);
		}

		if(kb[SDL_SCANCODE_D]) {
			move_dir = quat_get_right(cam.transform.rotation);
			move_vec = vec3f_scale(move_dir, cam_speed * time.delta_time);
		}

		// ---- SCRIPTING SECTION -----
		
		float angular_velocity = 0.2f; 
		float angle = time.delta_time * angular_velocity * mouse_dx;
		struct Quaternion cam_delta = quat_normalize(quat_angle_axis(angle, VEC3F_Y));
		cam.transform.rotation = quat_normalize(quat_mul(cam.transform.rotation, cam_delta));
		// Apply transformations to game object
		float yaw = time.delta_time * angular_velocity * mouse_dx;

		// input handling 	
		cam.transform.position = vec3f_add(cam.transform.position, move_vec);

		float go_angle = angular_velocity * time.delta_time;

		struct Vec3f rot_axis = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
		struct Vec3f rot_axis1 = {.x = -1.0f, .y = 1.0f, .z = 1.0f};
		struct Vec3f rot_axis2 = {.x = 1.0f, .y = 1.0f, .z = -1.0f};
		
		struct Quaternion rot = quat_normalize(quat_angle_axis(go_angle, rot_axis));
		struct Quaternion rot1 = quat_normalize(quat_angle_axis(2*go_angle, rot_axis1));
		struct Quaternion rot2 = quat_normalize(quat_angle_axis(3*go_angle, rot_axis2));

		go.transform.rotation = quat_normalize(quat_mul(go.transform.rotation, rot));
		go1.transform.rotation = quat_normalize(quat_mul(go1.transform.rotation, rot1));
		go2.transform.rotation = quat_normalize(quat_mul(go2.transform.rotation, rot2));
	
		// --- END OF SCRIPTING SECTION ---
		render_scene(framebuffer, zbuffer, scene);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
        }

        /* Clean Up */

	texture_free(tex);
	free(mesh.vertices);
	free(mesh.triangles);
        destroy_window(window_data);
        return 0;
}
