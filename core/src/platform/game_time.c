#include <stdio.h>
#include "game_time.h"

void time_init(Time* t){
	if(t == NULL){
		perror("src/game_time.c/time_init: struct Time* is NULL");
		exit(EXIT_FAILURE);
	}
	t->delta_time = 0;
	t->total_time = 0;
	t->last_time = clock();
}

void update_time(Time* t){
	if(t == NULL){
		perror("src/game_time.c/update_time: struct Time* is NULL");
		exit(EXIT_FAILURE);
	}	

	clock_t curr_time = clock();
	t->delta_time = (float)(curr_time - t->last_time) / CLOCKS_PER_SEC;
	t->last_time = curr_time;
	t->total_time += t->delta_time;
}

void print_time(Time* t){
	if(t == NULL){
		perror("src/game_time.c/update_time: struct Time* is NULL");
		exit(EXIT_FAILURE);
	}	
	printf("delta_time = {%f}; total_time = {%f}; last_time = {%ld}\n", t->delta_time, t->total_time, t->last_time);
}

void print_fps(Time* t){

	if(t == NULL){
		perror("src/game_time.c/update_time: struct Time* is NULL");
		exit(EXIT_FAILURE);
	}	
	int fps = 1 / t->delta_time;
	printf("fps = {%d}\n", fps);
}
