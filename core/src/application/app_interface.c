#include "cpu_engine.h"
#include "application.h" 

void app_init(App* app, AppVTable* v_table, AppCfg* cfg) {
	app->config  = cfg;	
	app->v_table = v_table;
	Pipeline* dflt_pl = pipeline_create(vs_default, fs_unlit);
	app->renderer = renderer_create(dflt_pl);
	app->fb     = frame_buffer_create(cfg->w_width, cfg->w_height, cfg->w_clear_color);
	app->window = window_create(cfg->w_width, cfg->w_height, cfg->w_name);
	app->scene  = NULL;
	app->assets = NULL;
}

void app_uninit(App* app) {
	renderer_destroy(app->renderer);	
	frame_buffer_destroy(app->fb);
	window_destroy(app->window);
	// scene , assets? 	
}

void app_run(App* app) {
	// main loop
	for(;;) {
		frame_buffer_clear(app->fb);
	}
}


