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
#include "quaternion.h"

// Expects a single string for cmd line input representing the obj that the user wishes to render

int main(int argc, char* argv[]) {

	/* Handle CLI */
	if(argc != 2) {
		printf("Please provide exactly one filename. ./models/{filename}.obj\n");
		exit(EXIT_FAILURE);
	}
	
	// Extract filename from CLI
	char filename[256] = {0};
	snprintf(filename, sizeof(filename), "./models/%s.obj", argv[1]);

	/* Parse Mesh from .obj file */
	struct Mesh mesh = parse_obj(filename);
	
	// Prepare Transform and GameObject
	struct Transform transform = {
		.position = VEC3F_0, 
		.rotation = QUAT_IDENTITY, 
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
		float angle = time.delta_time * angular_velocity;

		struct Vec3f euler_rot = {.x = angle, .y = angle, .z = angle }; 
		struct Quaternion delta = euler_to_quat(euler_rot);

		go.transform.rotation = quat_mul(go.transform.rotation, delta);	
		
		// Extract vertices and triangles from the Scene in World Coordinates
		struct Vec4f* vertices = get_vertices_from_game_object(go);
		
		// temporary sollution
		struct Vec3f* vertices3 = malloc(sizeof(struct Vec3f)*mesh.num_vertices);
		for(int i = 0; i < mesh.num_vertices; i++){
			vertices3[i].x = vertices[i].x;
			vertices3[i].y = vertices[i].y;
			vertices3[i].z = vertices[i].z;
		}	

		// Normalize vertices
		normalize_vertices(LENGTH_SCALE, vertices3, mesh.num_vertices);

       		// Reflect vertices3 vertically
       		for(int i = 0; i < mesh.num_vertices; i++){
       	        	vertices3[i].y = -1 * vertices3[i].y;
       		}

       		// Translate vertices3 so model is in screen center
       		for(int i = 0; i < mesh.num_vertices; i++){
       	        	vertices3[i].x += (int)WIDTH/2;
       	        	vertices3[i].y += (int)HEIGHT/2;
       		}

		// temporary so
		// Rasterize triangles in the scene to the framebuffer
	        render_triangles(
			       	 framebuffer       , 
				 zbuffer           , 
				 vertices3          , 
			       	 mesh.triangles    , 
				 mesh.num_triangles,
				 light_source
		);

		// Update SDL2 window w/ new framebuffer
                update_window(window_data, framebuffer);
		free(vertices);
		free(vertices3);
        }

        /* Clean Up */
	free(mesh.vertices);
	free(mesh.triangles);
        destroy_window(window_data);
        return 0;
}
