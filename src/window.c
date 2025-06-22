
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "render.h"
#include "inputparser.h"
#include "obj_parser.h"
#include "edge.h"

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

	// render from .obj
	char* obj_file_path = "./models/bunny.obj";

	struct Vertex* vertices = parse_vertices_from_obj(obj_file_path);
	int num_vertices = parse_num_vertices(obj_file_path);

	// transform vertices to fit the screen
	float centre_x = (float)WIDTH / 2.0f;
	float centre_y = (float)HEIGHT / 2.0f;

	for(int i = 0; i < num_vertices; i++){
		scale_vertex(vertices + i, -1.0f);
		translate_vertex(vertices + i, centre_x, centre_y, 0);
	}

	// produce wireframe edges
	struct Edge* wireframe_edges = parse_edges_from_obj(obj_file_path, vertices);
	int num_edges = parse_num_edges(obj_file_path);

	render_wireframe(framebuffer, wireframe_edges, num_edges);

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
	free(wireframe_edges);
	free(vertices);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
