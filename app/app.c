#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>

#include "cpu_engine.h"

typedef struct App {
	Scene*       scene;
	Assets*      assets;
	Renderer*    renderer;
	FrameBuffer* fb;
	Window*      window;
} App;

App* app_create(int w_width, int w_height, const char * w_name) {
	App* app = malloc(sizeof(App));

	Scene*        s = scene_create();
	FrameBuffer* fb = frame_buffer_create(w_width,w_height);
	Pipeline*     p = pipeline_create(vs_default, fs_unlit);
	Renderer*     r = renderer_init(p, fb);
	Window*       w = window_create(w_width,w_height,w_name);

	app->scene    = s;
	app->assets   = a;
	app->renderer = r;
	app->fb       = fb;
	app->window   = w;

	return app;
}

static Scene* scene;
static Light* light;

void app_init(void) {
	// USER LOGIC 
}

void app_update(float dt) {
	// USER LOGIC
}

void app_shutdown(void) {
	// USER LOGIC
}

// --- Event Handling ---
struct EventData {
	Vec3f mouse_input;
	Vec2f move_input;
};

struct EventData handle_event(SDL_Event* e, bool* running){
	/* User Can Register More Cases */
	struct EventData data = {0};

	while (SDL_PollEvent(e)) {
		switch(e->type) {
			case SDL_QUIT: running = false; break;
			case SDL_MOUSEMOTION:
				       data.mouse_input.x += e->motion.xrel;
				       data.mouse_input.y += e->motion.yrel;
				       break;
			case SDL_KEYDOWN:
				       if(e->key.keysym.scancode == SDL_SCANCODE_ESCAPE) *running = false;
				       break;
			case SDL_MOUSEBUTTONDOWN:
                    		printf("Mouse Position =  (x=%d, y=%d)\n", e->button.x, e->button.y);
                    		break;
		}
	}	

	const Uint8* kb = SDL_GetKeyboardState(NULL);
	if(kb[SDL_SCANCODE_W]) data.move_input.y += 1.0f;
	if(kb[SDL_SCANCODE_A]) data.move_input.x -= 1.0f; 
	if(kb[SDL_SCANCODE_S]) data.move_input.y -= 1.0f;
	if(kb[SDL_SCANCODE_D]) data.move_input.x += 1.0f; 

	vec2f_normalize(&data.move_input);

	return data;
}

