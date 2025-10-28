#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "render.h"
#include "window.h"
#include "scene_manager.h"
#include "game_time.h"
#include "framebuffer.h"
#include "app.h"
#include "vert_shader.h"
#include "frag_shader.h"

#define MAX_Z (100000.0f)
#define CLEAR_COLOR (0x87CEEBFF)
#define WIDTH (1920)
#define HEIGHT (780)
#define W_NAME ("cpu graphics engine")

int main(void) {

	struct AppAssets assets = app_load_assets();
	Scene* scene = app_create_scene(assets, WIDTH, HEIGHT);

	struct Time time;
	time_init(&time);

	FrameBuffer* fb = frame_buffer_create(WIDTH,HEIGHT);
	Pipeline*     p = pipeline_create(vs_default, fs_unlit);
	Renderer*     r = renderer_init(p, fb);
	Window*       w = window_create(WIDTH,HEIGHT,W_NAME);

        SDL_Event event;

        bool running = true;
        while(running) {
		frame_buffer_clear(fb, CLEAR_COLOR);
		update_time(&time);
		app_update_scene(scene, time.delta_time, &event, &running);
		renderer_draw_scene(r, scene);
                update_window(w, fb->framebuffer);
	}

	app_destroy_scene(scene);
	app_destroy_assets(assets);
       	destroy_window(w);
        return 0;
}
