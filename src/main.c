#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"
#include "render.h"
#include "inputparser.h"
#include "obj_parser.h"
#include "edge.h"
#include "triangle.h"
#include "window.h"

// Expects a single string for cmd line input representing the obj that the user wishes to render

int main(int argc, char* argv[]) {

	/* Handle CLI */
	if(argc != 2) {
		printf("Please provide exactly one filename. ./models/{filename}.obj\n");
		exit(EXIT_FAILURE);
	}

	char filename[256] = {0};
	snprintf(filename, sizeof(filename), "./models/%s.obj", argv[1]);

	/* Parse 3D Model from .obj file */
	struct ObjData obj_data = parse_obj(filename);

	// Prepare a Scene with a single Transform
	// Create Mesh from obj_data and attach to Transform
		

	/* Initialize frame buffer */
	uint32_t framebuffer[WIDTH * HEIGHT] = {0};

        /* Prepare Window using SDL */
        struct SDL_Data window_data = initialise_window();

        /* Render Loop */
        bool running = true;
        SDL_Event event;
        while(running) {
                // Event handling
                while (SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) running = false;
                }
		
		// Let Scene evolve with 'Time?'
		// - Maybe give the Transform an 'Angular Velocity?'
		
		// Extract vertices and triangles from the Scene in World Coordinates

		// Operate on those vertices to prepare them to be displayed on the screen

		// Normalize vertices
		float length_scale = 500.0f;
		normalize_vertices(length_scale, obj_data.vertices, obj_data.num_vertices);

        	// Reflect vertices vertically
        	for(int i = 0; i < obj_data.num_vertices; i++){
                	obj_data.vertices[i].y = -1 * obj_data.vertices[i].y;
        	}

        	// Translate vertices so model is in screen center
        	for(int i = 0; i < obj_data.num_vertices; i++){
                	obj_data.vertices[i].x += (int)WIDTH/2;
                	obj_data.vertices[i].y += (int)HEIGHT/2;
        	}
		
		// Rasterize triangles in the scene to the framebuffer
	        render_triangles(framebuffer, obj_data.triangles, obj_data.num_triangles);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
        }

        /* Clean Up */
	destroy_obj_data(obj_data);
        destroy_window(window_data);
        return 0;
}
