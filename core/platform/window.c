
#include "window.h"

struct SDL_Data initialise_window(int width, int height){

	// Initialise SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Create window
	SDL_Window* window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, 0
	);

	SDL_ShowCursor(SDL_DISABLE);
	int ret = SDL_ShowCursor(SDL_QUERY);
	if(ret != SDL_DISABLE) exit(0);	

	if(window == NULL){
		perror("src/window.c/initialise_window: SDL_CreateWindow returned a NULL ptr");
		exit(EXIT_FAILURE);
	}

	// Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL){
		perror("src/window.c/initialise_window: SDL_CreateRenderer returned a NULL ptr");
		exit(EXIT_FAILURE);
	}

	// Create texture
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height
	);

	if(texture == NULL){
		perror("src/window.c/initialise_window: SDL_CreateTexture returned a NULL ptr");
		exit(EXIT_FAILURE);
	}

	struct SDL_Data data = {
		.window = window,
		.renderer = renderer,
		.texture = texture,
		.width = width,
		.height = height
	};

	return data;
}

void destroy_window(struct SDL_Data data){
	SDL_DestroyTexture(data.texture);
	SDL_DestroyRenderer(data.renderer);
	SDL_DestroyWindow(data.window);
	SDL_Quit();
}

void update_window(struct SDL_Data data, uint32_t* framebuffer) {
	// Update texture with framebuffer pixels
	SDL_UpdateTexture(data.texture, NULL, framebuffer, data.width * sizeof(uint32_t));

	// Draw
	SDL_RenderClear(data.renderer);
	SDL_RenderCopy(data.renderer, data.texture, NULL, NULL);
	SDL_RenderPresent(data.renderer);
}
