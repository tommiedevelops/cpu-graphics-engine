#ifndef GAME_TIME_H
#define GAME_TIME_H
#include <time.h>
#include <stdlib.h>
typedef struct Time {
	float delta_time; // unit: seconds
	float total_time; // unit: seconds
	clock_t last_time; // unit: cpu clock cycles
} Time;
float get_delta_time();
void  time_init(Time* t);
void  update_time(Time* t);
void  print_time(Time* t);
void  print_fps(Time* t);
#endif
