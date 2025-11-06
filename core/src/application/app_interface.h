#ifndef APP_INTERFACE
#define APP_INTERFACE

#include <stdint.h>

typedef SDL_Event SDL_Event;
typedef struct AppVTable AppVTable;

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
	bool is_running;
} App;

typedef struct AppVTable {
	void (*on_start)(App* app,  void* ud);
	void (*on_event)(App* app,  void* ud, SDL_Event* e);
	void (*on_update)(App* app, void* ud, float dt);
	void (*on_render)(App* app, void* ud);
	void (*on_shutdown)(App* app, void* ud);
	void* user_data;
} AppVTable;

void app_init(App* app, AppVTable* v_table, AppCfg* cfg);

void app_uninit(App* app);

void app_run(App* app);

void app_shutdown(App* app);

#endif
