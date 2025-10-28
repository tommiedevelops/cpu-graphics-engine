
#include "window.h"

Window* window_create(int width, int height, const char* name){

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Window* sdl_window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, 0
	);

	SDL_Renderer* renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height
	);

	if(!texture || !renderer || !sdl_window) return NULL;

	Window* win = malloc(sizeof(Window));

	win->window   = sdl_window;
	win->renderer = renderer;
	win->texture  = texture;
	win->width    = width;
	win->height   = height;

	return win;
}

void destroy_window(Window* w){
	SDL_DestroyTexture(w->texture);
	SDL_DestroyRenderer(w->renderer);
	SDL_DestroyWindow(w->window);
	free(w);
	SDL_Quit();
}

void update_window(Window* w, uint32_t* fb) {
	SDL_UpdateTexture(w->texture, NULL, fb, w->width*sizeof(uint32_t));
	SDL_RenderClear(w->renderer);
	SDL_RenderCopy(w->renderer, w->texture, NULL, NULL);
	SDL_RenderPresent(w->renderer);
}
