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

	/* Parse Mesh from .obj file */
	struct Mesh mesh = parse_obj("./models/bunny.obj");
	
	struct Mesh ground_mesh = {
	};
	
	// Prepare Transform and GameObjects
	
	struct Transform transform = {
		.position = VEC3F_0, 
		.rotation = QUAT_IDENTITY, 
		.scale = VEC3F_1
	};

	struct Vec3f pos1 = {.x = 2.0f, .y = 0.0f, .z = 0.0f};

	struct Transform tr1 = {
		.position = pos1,
		.rotation = QUAT_IDENTITY,
		.scale = VEC3F_1
	};

	struct Vec3f pos2 = {.x = -2.0f, .y = 0.0f, .z = 0.0f};

	struct Transform tr2 = {
		.position = pos2,
		.rotation = QUAT_IDENTITY,
		.scale = VEC3F_1
	};

	struct GameObject go = {
		.mesh 	    = mesh     , 
		.transform  = transform 
	};

	struct GameObject go1 = {
		.mesh = mesh ,
		.transform = tr1
	};

	struct GameObject go2 = {
		.mesh = mesh ,
		.transform = tr2
	};

	int num_gameObjects = 3;
	struct GameObject* gameObjects[num_gameObjects];
	gameObjects[0] = &go;
	gameObjects[1] = &go1;
	gameObjects[2] = &go2;
		
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
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
					break;
			}
                }	

		// Hide the cursor
		const Uint8* kb = SDL_GetKeyboardState(NULL);

		// ---- SCRIPTING SECTION -----
		// Apply transformations to game object
		float angular_velocity = 0.2f; 
		float cam_speed = -1.0f;
		float yaw = time.delta_time * angular_velocity * mouse_dx;
		float pitch = time.delta_time * angular_velocity * mouse_dy;

		// input handling 	
		struct Quaternion cam_delta_lr = quat_normalize(quat_angle_axis(yaw, VEC3F_Y));
		
		struct Vec3f ud_rot_axis = vec3f_cross(quat_get_forward(cam.transform.rotation), VEC3F_Y);
		struct Quaternion cam_delta_ud = quat_normalize(quat_angle_axis(pitch,ud_rot_axis));
		cam.transform.rotation = quat_normalize(quat_mul(cam.transform.rotation, cam_delta_lr));
		cam.transform.rotation = quat_normalize(quat_mul(cam.transform.rotation, cam_delta_ud));

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
		cam.transform.position = vec3f_add(cam.transform.position, move_vec);

		// --- END OF SCRIPTING SECTION ---
		render_scene(framebuffer, zbuffer, scene);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
        }

        /* Clean Up */

	free(mesh.vertices);
	free(mesh.triangles);

        destroy_window(window_data);
        return 0;
}
