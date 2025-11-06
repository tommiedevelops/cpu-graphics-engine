#ifndef APP_INTERFACE
#define APP_INTERFACE

#include <stdint.h>

typedef struct AppVTable {
	void (*on_start)(void* ud);
	void (*on_event)(void* ud);
	void (*on_update)(void* ud, float dt);
	void (*on_render)(void* ud);
	void (*on_shutdown)(void* ud);
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
