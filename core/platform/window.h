#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>


struct SDL_Data {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
	int width, height;
};

struct SDL_Data initialise_window(int width, int height);
void destroy_window(struct SDL_Data data);
void update_window(struct SDL_Data data, uint32_t* framebuffer);

#endif
