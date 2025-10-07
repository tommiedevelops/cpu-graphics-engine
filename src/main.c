#include <stdint.h>
#include "clip.h"
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

	struct Scene scene = create_scene();

	if(scene.gameObjects == NULL){
		printf("NULL\n");
		exit(0);
	}

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

		// Clear the buffers 
		memset(framebuffer, 0x0, sizeof(framebuffer));
		memset(zbuffer, 0x0, sizeof(zbuffer));

                // ----- Input & Event Handling -----
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

		const Uint8* kb = SDL_GetKeyboardState(NULL);
		struct Vec2f move_input = {0};
		if(kb[SDL_SCANCODE_W]) move_input.y += 1.0f;
		if(kb[SDL_SCANCODE_A]) move_input.x -= 1.0f; 
		if(kb[SDL_SCANCODE_S]) move_input.y -= 1.0f;
		if(kb[SDL_SCANCODE_D]) move_input.x += 1.0f; 

		vec2f_normalize(&move_input);

		// ----- Custom Scripting -----

		// First Person Camera	
		float cam_speed = 4.0f;	
		float cam_ang_vel = 1.0f;	
			
		float yaw = mouse_dx * time.delta_time * cam_ang_vel;
		struct Quaternion qYaw = quat_angle_axis(yaw, VEC3F_Y);
		scene.cam->transform.rotation = quat_mul(qYaw, scene.cam->transform.rotation);
		quat_normalize(scene.cam->transform.rotation);

		// camera translation
		struct Vec3f forward = quat_get_forward(scene.cam->transform.rotation);
		struct Vec3f right = quat_get_right(scene.cam->transform.rotation);

		struct Vec3f move_vec = 
			vec3f_add(
					vec3f_scale(forward, move_input.y),
					vec3f_scale(right, move_input.x)
				 );

		
		scene.cam->transform.position = 
			vec3f_add(
					scene.cam->transform.position, 
					vec3f_scale(move_vec, cam_speed * time.delta_time)
				 );
			
		// ----- Rendering -----
		render_scene(framebuffer, zbuffer, scene);

		// ---- Merging -----
                update_window(window_data, framebuffer);
	}
        /* Clean Up */

	// destroy_scene(scene);
        destroy_window(window_data);
        return 0;
}
