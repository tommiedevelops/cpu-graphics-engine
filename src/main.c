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

#include "app.h"

#define MAX_Z (100000.0f)
#define CLEAR_COLOR (0);

static inline void clear_buffers(uint32_t* framebuffer, float* zbuffer){
	for(int i = 0; i < WIDTH*HEIGHT; i++) zbuffer[i] = MAX_Z;
	for(int i = 0; i < WIDTH*HEIGHT; i++) framebuffer[i] = CLEAR_COLOR;
}

int main(void) {

	struct Scene* scene = app_create_scene();

	if(NULL == scene) {
		printf("scene is null\n");
	       	exit(-1);
	}

	struct Time time;
	time_init(&time);

	uint32_t framebuffer[WIDTH * HEIGHT] = {0};
	float zbuffer[WIDTH * HEIGHT] = {0};
	
        struct SDL_Data window_data = initialise_window();
        SDL_Event event;

        /* Render Loop */
        bool running = true;
        while(running) {
		clear_buffers(framebuffer, zbuffer);
		update_time(&time);
		app_update_scene(scene, time.delta_time, &event, &running);
		render_scene(framebuffer, zbuffer, *scene);
                update_window(window_data, framebuffer);
	}

        /* Clean Up */

	// destroy_scene(scene);
        destroy_window(window_data);
        return 0;
}
