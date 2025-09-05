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
	struct Mesh mesh = parse_obj(filename);
	
	// Prepare Transform and GameObject
	struct Transform transform = {
		.position = VEC3F_0, 
		.rotation = QUAT_IDENTITY, 
		.scale = VEC3F_1
	};

	struct GameObject go = {
		.mesh 	    = mesh     , 
		.transform  = transform 
	};

	// Prepare Camera
	// To start, the camera is on position (0,0,5) facing the -Z direction
	struct Vec3f camera_pos = {.x = 0.0f, .y = 0.0f, .z = 20.0f};
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
		.cam = cam,
		.gameObjects = &go,
		.num_gameObjects = 1,
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
		print_fps(&time);

		// Clear the buffers	
		memset(framebuffer, 0x0, sizeof(framebuffer));
		memset(zbuffer, 0x0, sizeof(zbuffer));

                // Event handling
                while (SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) running = false;
                }	
		
		// ---- SCRIPTING SECTION -----
		// Apply transformations to game object
		float angular_velocity = 1.0f; 
		float cam_speed = 1.0f;
		float angle = time.delta_time * angular_velocity;

		struct Vec3f euler_rot = {.x = 0.0f, .y = angle, .z = angle};
		
		//struct Quaternion delta = quat_normalize(euler_to_quat(euler_rot));
		struct Vec3f axis = {.x = 0.0f, .y = 1.0f, .z= 0.0f};
		struct Quaternion delta = quat_angle_axis(angle, axis);
		go.transform.rotation = quat_normalize(quat_mul(go.transform.rotation, delta));
		go.transform.position.z += time.delta_time * cam_speed; 
		print_vec3f(go.transform.position);
		/* cam.transform.rotation:W
		 * = quat_normalize(quat_mul(delta, cam.transform.rotation)); */

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
