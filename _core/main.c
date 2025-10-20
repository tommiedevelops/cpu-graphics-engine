#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"
#include "render.h"
#include "window.h"
#include "scene_manager.h"
#include "game_time.h"

#include "app.h"

#define MAX_Z (100000.0f)
#define CLEAR_COLOR (0x87CEEBFF);

static inline void clear_buffers(uint32_t* framebuffer, float* zbuffer){
	for(int i = 0; i < WIDTH*HEIGHT; i++) zbuffer[i] = MAX_Z;
	for(int i = 0; i < WIDTH*HEIGHT; i++) framebuffer[i] = CLEAR_COLOR;
}

int main(void) {

	struct AppAssets assets = app_load_assets();
	Scene* scene = app_create_scene(assets);

	struct Time time;
	time_init(&time);

	uint32_t framebuffer[WIDTH * HEIGHT] = {0};
	float zbuffer[WIDTH * HEIGHT] = {0};
	
        struct SDL_Data window_data = initialise_window();
        SDL_Event event;

        bool running = true;
        while(running) {
		clear_buffers(framebuffer, zbuffer);
		update_time(&time);
		app_update_scene(scene, time.delta_time, &event, &running);
		render_scene(framebuffer, zbuffer, scene, NULL);
                update_window(window_data, framebuffer);
	}

	app_destroy_scene(scene);
	app_destroy_assets(assets);
       	destroy_window(window_data);
        return 0;
}
