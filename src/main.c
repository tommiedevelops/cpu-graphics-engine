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

		struct Vec3f forward = quat_get_forward(scene.cam->transform.rotation);
		struct Vec3f right = quat_get_right(scene.cam->transform.rotation);	

		// camera rotation
		struct Vec3f forward_projected = vec3f_create(forward.x, 0.0f, forward.z);
		struct Vec3f lr_axis = VEC3F_Y;		

		struct Quaternion lr_rot = quat_angle_axis(mouse_dx * time.delta_time * cam_ang_vel, lr_axis);

		scene.cam->transform.rotation =
			quat_normalize(
					quat_mul(
						scene.cam->transform.rotation,
						lr_rot
					        )
				      );

		// camera translation
		struct Vec3f move_vec = 
			vec3f_add(
					vec3f_scale(forward, -move_input.y),
					vec3f_scale(right, -move_input.x)
				 );

		
		scene.cam->transform.position = 
			vec3f_add(
					scene.cam->transform.position, 
					vec3f_scale(move_vec, cam_speed * time.delta_time)
				 );
			
		// Game Objects
		float angular_velocity = 1.0f;
		float go_angle = angular_velocity * time.delta_time;

		struct Vec3f rot_axis = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
		struct Vec3f rot_axis1 = {.x = -1.0f, .y = 1.0f, .z = 1.0f};
		struct Vec3f rot_axis2 = {.x = 1.0f, .y = 1.0f, .z = -1.0f};
		
		struct Quaternion rot = quat_normalize(quat_angle_axis(go_angle, rot_axis));
		struct Quaternion rot1 = quat_normalize(quat_angle_axis(2*go_angle, rot_axis1));
		struct Quaternion rot2 = quat_normalize(quat_angle_axis(3*go_angle, rot_axis2));

		struct GameObject* go = scene.gameObjects[0];

		struct GameObject* go1 = scene.gameObjects[1];

		struct GameObject* go2 = scene.gameObjects[2];
	
		go->transform.rotation = quat_normalize(quat_mul(go->transform.rotation, rot));
		go1->transform.rotation = quat_normalize(quat_mul(go1->transform.rotation, rot1));
		go2->transform.rotation = quat_normalize(quat_mul(go2->transform.rotation, rot2));

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
