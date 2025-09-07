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

// Expects a single string for cmd line input representing the obj that the user wishes to render

int main(int argc, char* argv[]) {

	/* Handle CLI */
	if(argc != 2) {
		printf("Please provide exactly one filename. ./models/{filename}.obj\n");
		exit(EXIT_FAILURE);
	}
	
	// Extract filename from CLI
	char filename[256] = {0};
	snprintf(filename, sizeof(filename), "./models/%s.obj", argv[1]);

	/* Parse Mesh from .obj file */
	struct Mesh bunny_mesh = parse_obj("./models/bunny.obj");
	struct Mesh teapot_mesh = parse_obj("./models/teapot.obj");
	struct Mesh dragon_mesh = parse_obj("./models/dragon.obj");
	
	// Prepare Transform and GameObjects
	
	struct Vec3f bunny_pos = {.x = 1.0f, .y = 0.0f, .z = 0.0f};
	struct Transform bunny_transform = {
		.position = bunny_pos, 
		.rotation = QUAT_IDENTITY, 
		.scale = VEC3F_1
	};

	struct Vec3f teapot_pos = {.x = -1.0f, .y = 0.0f, .z = 0.0f};
	struct Transform teapot_transform = {
		.position = teapot_pos, 
		.rotation = QUAT_IDENTITY, 
		.scale = VEC3F_1
	};

	struct Vec3f dragon_pos = {.x = 0.0f, .y = 0.0f, .z = 1.0f};
	struct Transform dragon_transform = {
		.position = VEC3F_Z, 
		.rotation = QUAT_IDENTITY, 
		.scale = VEC3F_1
	};

	struct GameObject bunny_go = {
		.mesh 	    = bunny_mesh     , 
		.transform  = bunny_transform 
	};

	struct GameObject dragon_go = {
		.mesh 	    = dragon_mesh     , 
		.transform  = dragon_transform 
	};

	struct GameObject teapot_go = {
		.mesh 	    = teapot_mesh     , 
		.transform  = teapot_transform 
	};

	int num_gameObjects = 3;
	struct GameObject* gameObjects[num_gameObjects];
	gameObjects[0] = &bunny_go;
	gameObjects[1] = &dragon_go;
	gameObjects[2] = &teapot_go;

	// Prepare Camera
	// To start, the camera is on position (0,0,5) facing the -Z direction
	struct Vec3f camera_pos = {.x = 0.0f, .y = 0.0f, .z = 3.0f};
	struct Transform camera_transform = {
		.position = camera_pos,
		.rotation = QUAT_IDENTITY,
		.scale = VEC3F_1
	};

	struct Camera cam = {0};	
	cam.transform = camera_transform;
	
	cam.fov = PI*60.0f/180.0f; 
	cam.near = 5.00f;
	cam.far = 20.0f;

	// Prepare light source
	struct Vec3f light_source_pos = {
		.x = 0.0f,
		.y = 0.0f,
		.z = 1.0f
	};

	struct LightSource light_source  = {
		.direction = light_source_pos
	};

	struct Scene scene = {
		.cam = &cam,
		.gameObjects = gameObjects,
		.num_gameObjects = num_gameObjects,
		.light = light_source
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

		mouse_dx = mouse_dx;
		// Hide the cursor
		const Uint8* kb = SDL_GetKeyboardState(NULL);

		// ---- SCRIPTING SECTION -----
		// Apply transformations to game object
		float angular_velocity = 0.2f; 
		float cam_speed = -1.0f;
		float angle =time.delta_time * angular_velocity * mouse_dx;

/* 		if(mouse_dx != 0){ */
/* 			printf("mouse_dx = %d\n", mouse_dx); */
/* 			printf("time.delta_time=%f\n", time.delta_time); */
/* 			printf("angular_velocity = %f\n", angular_velocity); */
/* 			printf("angle=%f\n", angle); */
/* 		} */	

		// input handling 	
		struct Quaternion cam_delta = quat_normalize(quat_angle_axis(angle, VEC3F_Y));
		cam.transform.rotation = quat_normalize(quat_mul(cam.transform.rotation, cam_delta));

		//printf("mousex= %d mousey= %d\n", mouse_dx, mouse_dy);
		struct Vec3f move_dir = {0};
		struct Vec3f move_vec = {0};

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
		
		cam.transform.position = vec3f_add(cam.transform.position, move_vec);

		// --- END OF SCRIPTING SECTION ---
		float go_angle = angular_velocity * time.delta_time;

		struct Quaternion bunny_rot = quat_normalize(quat_angle_axis(go_angle, VEC3F_Y));
		struct Quaternion dragon_rot = quat_normalize(quat_angle_axis(2*go_angle, VEC3F_Y));
		struct Quaternion teapot_rot = quat_normalize(quat_angle_axis(3*go_angle, VEC3F_Y));

		bunny_go.transform.rotation = quat_normalize(quat_mul(bunny_go.transform.rotation, bunny_rot));
		dragon_go.transform.rotation = quat_normalize(quat_mul(dragon_go.transform.rotation, dragon_rot));
		teapot_go.transform.rotation = quat_normalize(quat_mul(teapot_go.transform.rotation, teapot_rot));

		render_scene(framebuffer, zbuffer, scene);
		print_vec3f(quat_get_forward(cam.transform.rotation));

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
        }

        /* Clean Up */

	free(bunny_mesh.vertices);
	free(bunny_mesh.triangles);

	free(dragon_mesh.vertices);
	free(dragon_mesh.triangles);

	free(teapot_mesh.vertices);
	free(teapot_mesh.triangles);
        destroy_window(window_data);
        return 0;
}
