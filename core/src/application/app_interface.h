#ifndef APP_INTERFACE
#define APP_INTERFACE

#include <stdint.h>

typedef SDL_Event SDL_Event;
typedef struct AppVTable {
	void (*on_start)(void);
	void (*on_event)(SDL_Event* e);
	void (*on_update)(float dt);
	void (*on_render)();
	void (*on_shutdown)();
	void* userdata;
} AppVTable;

typedef struct AppCfg {
	int w_width, w_height;
	const char * w_name;
	uint32_t w_clear_color;
} AppCfg;

typedef struct App {
	AppCfg*      config;
	AppVTable*   v_table;
	Renderer*    renderer;
	FrameBuffer* fb;
	Window*      window;
	Assets*      assets;
	Scene*       scene;
} App;

void app_init(App* app, AppVTable* v_table, AppCfg* cfg);

void app_uninit(App* app);

void app_run(App* app);

#endif
