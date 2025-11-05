#ifndef APPLICATION
#define APPLICATION

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
} AppCfg;

typedef struct App {
	AppCfg*   config;
	AppVTable v_table;

} App;

#endif
