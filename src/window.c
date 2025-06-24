
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "render.h"
#include "inputparser.h"
#include "obj_parser.h"
#include "edge.h"
#include "triangle.h"

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

	struct Vertex v0 = {.x = 50.0f, .y = 50.0f, .z = 0.0f};
	struct Vertex v1 = {.x = 150.0f, .y = 50.0f, .z = 0.0f};
	struct Vertex v2 = {.x = 50.0f, .y = 150.0f, .z = 0.0f};

	struct Vertex v3 = {.x = 200.0f, .y = 200.0f, .z = 0.0f};
	struct Vertex v4 = {.x = 200.0f, .y = 100.0f, .z = 0.0f};
	struct Vertex v5 = {.x = 300.0f, .y = 300.0f, .z = 0.0f};

	struct Triangle triangles[2] = {
		{.a = &v0, .b = &v1, .c = &v2},
		{.a = &v3, .b = &v4, .c = &v5}
	};

	int num_triangles = 2;

	render_triangles(framebuffer, triangles, num_triangles);

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
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
