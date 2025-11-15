#include "cpu_engine.h"
#include "application.h" 

static void validate_vtable(AppVTable* v_table) {
	if(!v_table)  {
		perror("function call backs are required to proceed");
		exit(-1);
	}

	int error = 0;

	if(!v_table->on_init) error = 1;
	if(!v_table->on_start) error = 1;
	if(!v_table->on_event) error = 1;
	if(!v_table->on_update) error = 1;
	if(!v_table->on_shutdown) error = 1;

	if(error) {
		perror("a v_table call back was null");
		exit(-1);
	}
}

void app_init(App* app, AppVTable* v_table, AppCfg* cfg) {

	if(!app) {
		perror("please provide an app to initialise");
		exit(-1);
	}

	if(!cfg) {
		perror("please provide an app config");
		exit(-1);
	}

	validate_vtable(v_table);

	app->config  = cfg;	
	app->v_table = v_table;

	Pipeline* dflt_pl = pipeline_create(vs_default, fs_unlit);
	app->renderer = renderer_create(dflt_pl);
	app->fb     = frame_buffer_create(cfg->w_width, cfg->w_height, cfg->w_clear_color);
	app->window = window_create(cfg->w_width, cfg->w_height, cfg->w_name);

	app->scene  = NULL;
	app->assets = NULL;
	app->print_fps = false;
	app->is_running = false;
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
	vt->on_init(app, vt->user_data);
	vt->on_start(app, vt->user_data);

	app->is_running = true;
	while(app->is_running) {
		frame_buffer_clear(app->fb);

		if(app->print_fps) print_fps(&time);
		update_time(&time);
		float dt = time.delta_time;
		
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				app->is_running = false;
			}

			vt->on_event(app, vt->user_data, &e);
		}

		if(!app->is_running) break;
		vt->on_update(app, vt->user_data, dt);

		renderer_draw_scene(app->renderer, app->fb, app->scene);
		window_update(app->window, app->fb->framebuffer);
	}

}

void app_shutdown(App* app) {
	app->is_running = false;
	AppVTable* vt = app->v_table;
	vt->on_shutdown(app, vt->user_data);
	app_uninit(app);
}

