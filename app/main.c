#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "cpu_engine.h"
#include "app.h"

#define MAX_Z (100000.0f)
#define CLEAR_COLOR (0x87CEEBFF)
#define W_WIDTH (512)
#define W_HEIGHT (392)
#define W_NAME ("cpu graphics engine")

int main(void) {

	Time time;
	time_init(&time);

	App app* = app_create(W_WIDTH, W_HEIGHT, W_NAME);
	app_init(app);
	app_run(app);

        bool running = true;
        while(running) {
		print_fps(&time);
		update_time(&time);
		app_update(app, time.delta_time);
	}

	app_shutdown(app);
        return 0;
}
