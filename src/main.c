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

	// Prepare Camera
	// To start, the camera is on position (0,0,5) facing the -Z direction
	struct Vec3f camera_pos = {.x = 0.0f, .y = 0.0f, .z = 3.0f};
	struct Transform camera_transform = {
		.position = camera_pos,
		.rotation = QUAT_IDENTITY,
		.scale = VEC3F_1
	};

	struct Camera cam = {0};	
	cam.transform = camera_transform;

	// Prepare light source
	struct Vec3f light_source_pos = {
		.x = 0.0f,
		.y = 0.0f,
		.z = 1.0f
	};

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
		
		// ---- SCRIPTING SECTION -----
		// Apply transformations to game object
		float angular_velocity = 1.0f; 
		float angle = time.delta_time * angular_velocity;

		struct Vec3f euler_rot = {.x = 0.0f, .y = angle, .z = angle};
		
		//struct Quaternion delta = quat_normalize(euler_to_quat(euler_rot));
		struct Vec3f axis = {.x = 0.0f, .y = 1.0f, .z= 0.0f};
		struct Quaternion delta = quat_angle_axis(angle, axis);
		go.transform.rotation = quat_normalize(quat_mul(go.transform.rotation, delta));
		/* cam.transform.rotation = quat_normalize(quat_mul(delta, cam.transform.rotation)); */

		// --- END OF SCRIPTING SECTION ---

		// Extract vertices and triangles from the Scene in World Coordinates
		struct Mat4 model_matrix = get_model_matrix(go);
		struct Mat4 view_matrix = get_view_matrix(cam);

		float aspect = (float)HEIGHT / WIDTH;
		float fov = PI*60.0f/180.0f;
		float near = 0.1f;
		float far = 20.0f;

		struct Mat4 projection_matrix = get_projection_matrix(fov, aspect, near, far);	
		struct Mat4 viewport_matrix = get_viewport_matrix(near, far);	
		int num_vertices = go.mesh.num_vertices;
		
		struct Vec4f* vertices = malloc(num_vertices*sizeof(struct Vec4f));
		memset(vertices, 0x0, num_vertices*sizeof(struct Vec4f));

		struct Vec3f* vertices3 = malloc(num_vertices*sizeof(struct Vec3f));
		memset(vertices3, 0x0, num_vertices*sizeof(struct Vec3f));

		for(int i = 0; i < num_vertices; i++){

			struct Vec4f vertex = {
				.x = go.mesh.vertices[i].x,
				.y = go.mesh.vertices[i].y,
				.z = go.mesh.vertices[i].z,
				.w = 1.0f
			};

			vertices[i] = mat4_mul_vec4(model_matrix, vertex);
			vertices[i] = mat4_mul_vec4(view_matrix, vertices[i]);
			vertices[i] = mat4_mul_vec4(projection_matrix, vertices[i]);
			vertices[i] = perspective_divide(vertices[i]);
			vertices[i] = mat4_mul_vec4(viewport_matrix, vertices[i]);

			vertices3[i].x = vertices[i].x;
			vertices3[i].y = vertices[i].y;
			vertices3[i].z = vertices[i].z;
			/* print_vec4f(vertices[i]); */

			print_vec3f(vertices3[i]);
		}

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
        }

        /* Clean Up */
	free(mesh.vertices);
	free(mesh.triangles);
        destroy_window(window_data);
        return 0;
}
