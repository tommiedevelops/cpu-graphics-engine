#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"
#include "render.h"
#include "obj_parser.h"
#include "triangle.h"
#include "window.h"
#include "scene_manager.h"

// Expects a single string for cmd line input representing the obj that the user wishes to render

int main(int argc, char* argv[]) {

	/* Handle CLI */
	if(argc != 2) {
		printf("Please provide exactly one filename. ./models/{filename}.obj\n");
		exit(EXIT_FAILURE);
	}

	char filename[256] = {0};
	snprintf(filename, sizeof(filename), "./models/%s.obj", argv[1]);

	/* Parse Mesh from .obj file */
	struct Mesh mesh = parse_obj(filename);

	struct Transform transform = {
		.position = VEC3F_0, 
		.rotation = 0.0f, 
		.scale = VEC3F_1
	};

	struct GameObject go = {.mesh = mesh, .transform = transform };

	// Normalize vertices
	float length_scale = 500.0f;
	normalize_vertices(length_scale, mesh.vertices, mesh.num_vertices);

       	// Reflect vertices vertically
       	for(int i = 0; i < mesh.num_vertices; i++){
               	mesh.vertices[i].y = -1 * mesh.vertices[i].y;
       	}

       	// Translate vertices so model is in screen center
       	for(int i = 0; i < mesh.num_vertices; i++){
               	mesh.vertices[i].x += (int)WIDTH/2;
               	mesh.vertices[i].y += (int)HEIGHT/2;
       	}
		

	/* Initialize frame buffer */
	uint32_t framebuffer[WIDTH * HEIGHT] = {0};

        /* Prepare Window using SDL */
        struct SDL_Data window_data = initialise_window();

        /* Render Loop */
        bool running = true;
        SDL_Event event;
        while(running) {
		// Clear the framebuffer
		memset(framebuffer, 0x0, sizeof(framebuffer));

                // Event handling
                while (SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) running = false;
                }
		
		// Let Scene evolve with 'Time?'
		// - Maybe give the Transform an 'Angular Velocity?'
		
		// Extract vertices and triangles from the Scene in World Coordinates

		// Operate on those vertices to prepare them to be displayed on the screen

	
		// Rasterize triangles in the scene to the framebuffer
	        render_triangles(framebuffer, mesh.triangles, mesh.num_triangles);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
        }

        /* Clean Up */
	free(mesh.vertices);
	free(mesh.triangles);
        destroy_window(window_data);
        return 0;
}
