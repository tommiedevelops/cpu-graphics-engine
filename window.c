#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "headerfiles/line.h"
#include "headerfiles/constants.h"
#include "headerfiles/inputparser.h"

int main() {
	// Initialise SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}

	// Create window
	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, 0
	);

	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Create texture
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT
	);

	uint32_t framebuffer[WIDTH * HEIGHT] = {0};

	// draw lines
	int num_coords = 0; 
	int** start_coords = NULL; 
	int** end_coords = NULL;
	parse_file("line.input", start_coords, end_coords, &num_coords);
	
	if( (start_coords == NULL) || (end_coords == NULL) ) {
		perror("something wrong with cooridnates");
		exit(0);
	}
	
	for(int i = 0; i<num_coords;i++){
		draw_line(framebuffer, start_coords[i], end_coords[i]); // testing linkage
	}

	bool running = true;
	SDL_Event event;
	while(running) {
		// Event handling
		while (SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) running = false;
		}

		// Update texture with framebuffer pixels
		SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));

		// Draw
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	// Clean Up
	free_coords_array(start_coords, num_coords);
	free_coords_array(end_coords, num_coords);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
