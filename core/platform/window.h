#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
	int width, height;
} Window;

Window* window_create(int w_width, int w_height, const char* w_name);
void window_destroy(Window* w);
void window_update(Window* w, uint32_t* framebuffer);

#endif
