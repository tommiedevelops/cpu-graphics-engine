#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "constants.h"
#include "render.h"
#include "obj_parser.h"
#include "triangle.h"
#include "window.h"
#include "scene_manager.h"
#include "game_time.h"

// Expects a single string for cmd line input representing the obj that the user wishes to render

int main(int argc, char* argv[]) {

	/* Parse Mesh from .obj file */
	struct Mesh mesh = parse_obj("./models/bunny.obj");
	
	// Prepare Transform and GameObject
	struct Transform transform = {
		.position = VEC3F_0, 
		.rotation = VEC3F_0, 
		.scale = VEC3F_1
	};

	struct GameObject go = {
		.mesh 	    = mesh     , 
		.transform  = transform 
	};
	
	struct Vec3f light_source_pos = VEC3F_1;
	scale_vector(&light_source_pos, -1);

	struct LightSource light_source  = {
		.direction = light_source_pos
	};

	// Initialize time struct
	struct Time time;
	time_init(&time);

	// Initialize framebuffer and z-buffer
	uint32_t framebuffer[WIDTH * HEIGHT] = {0};
	float zbuffer[WIDTH * HEIGHT] = {0};

        /* Prepare Window using SDL */
        struct SDL_Data window_data = initialise_window();

        /* Render Loop */
        bool running = true;
        SDL_Event event;
        while(running) {
		
		// Handle Time
		update_time(&time);
		print_fps(&time);

		// Clear the buffers	
		memset(framebuffer, 0x0, sizeof(framebuffer));
		memset(zbuffer, 0x0, sizeof(zbuffer));

                // Event handling
                while (SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) running = false;
                }	

		// Apply transformations to game object
		float angular_velocity = 1.0f; 
		go.transform.rotation.y += time.delta_time * angular_velocity;	
		
		// Extract vertices and triangles from the Scene in World Coordinates
		struct Vec3f* vertices = get_vertices_from_game_object(go);
	
		// Normalize vertices
		normalize_vectors(LENGTH_SCALE, vertices, mesh.num_vertices);

       		// Reflect vertices vertically
       		for(int i = 0; i < mesh.num_vertices; i++){
       	        	vertices[i].y = -1 * vertices[i].y;
       		}

       		// Translate vertices so model is in screen center
       		for(int i = 0; i < mesh.num_vertices; i++){
       	        	vertices[i].x += (int)WIDTH/2;
       	        	vertices[i].y += (int)HEIGHT/2;
       		}

		// Rasterize triangles in the scene to the framebuffer
	        render_triangles(
			       	 framebuffer       , 
				 zbuffer           , 
				 vertices          , 
			       	 mesh.triangles    , 
				 mesh.num_triangles,
				 light_source
		);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
		free(vertices);
        }

        /* Clean Up */
	free(mesh.vertices);
	free(mesh.triangles);
        destroy_window(window_data);
        return 0;
}
