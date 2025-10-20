#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"
#include "render.h"
#include "window.h"
#include "scene_manager.h"
#include "game_time.h"
#include "framebuffer.h"
#include "app.h"
#include "pipeline.h"

#define MAX_Z (100000.0f)
#define CLEAR_COLOR (0x87CEEBFF)

int main(void) {

	struct AppAssets assets = app_load_assets();
	Scene* scene = app_create_scene(assets);

	struct Time time;
	time_init(&time);

	FrameBuffer* fb = frame_buffer_create(WIDTH,HEIGHT);
	Pipeline*     p = pipeline_init();
	Renderer*     r = renderer_init(p, fb);

        struct SDL_Data window_data = initialise_window();
        SDL_Event event;

        bool running = true;
        while(running) {
		frame_buffer_clear(fb, CLEAR_COLOR);
		update_time(&time);
		app_update_scene(scene, time.delta_time, &event, &running);
		renderer_draw_scene(r, scene);
                update_window(window_data, fb->framebuffer);
	}

	app_destroy_scene(scene);
	app_destroy_assets(assets);
       	destroy_window(window_data);
        return 0;
}
