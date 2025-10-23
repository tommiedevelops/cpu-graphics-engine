#ifndef GAME_TIME_H
#define GAME_TIME_H
#include <time.h>
#include <stdlib.h>
struct Time {
	float delta_time; // unit: seconds
	float total_time; // unit: seconds
	clock_t last_time; // unit: cpu clock cycles
};
float get_delta_time();
void time_init(struct Time* t);
void update_time(struct Time* t);
void print_time(struct Time* t);
void print_fps(struct Time* t);
#endif
