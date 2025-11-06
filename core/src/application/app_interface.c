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
	
	AppVTable* vt = app->v_table;

	Time time;
	time_init(&time);

	SDL_Event e;
	vt->on_start();

	bool running = true;
	while(running) {

		update_time(&time);
		float dt = time.delta_time;
		
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) running = false;
			vt->on_event(&e);
		}

		vt->on_update(dt);
		vt->on_render();
	}

	vt->on_shutdown();
}


