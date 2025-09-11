#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vec3f.h"
#include "render.h"
#include "color.h"
#include "matrix.h"
#include "scene_manager.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
	if( (x > WIDTH) || (x < 0) )
		printf("render.c/place_pixel: invalid x value. pixel= {%d,%d}\n", x,y); return;
	if( (y > HEIGHT) || (y < 0) )
		printf("render.c/place_pixel: invalid y value. pixel= {%d,%d}\n", x,y); return;
}

void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene) {
	if(scene.gameObjects == NULL){
		printf("src/render.c/render_scene: no gameObjects to render\n");
		return;
	}
	
	for(int i = 0; i < scene.num_gameObjects; i++) {
		
		// Primitive Assembly
		struct GameObject go = *scene.gameObjects[i];
		struct Material mat = go.material;
		struct Vec3f* vertices = go.mesh.vertices;
		int* triangles = go.mesh.triangles;

		// Transform and rasterize each triangle
		for(int j = 0; j < go.mesh.num_triangles; j++) {

			struct Triangle tri = {
				.v0 = vertices[triangles[3*j]],
				.v1 = vertices[triangles[3*j+1]],
				.v2 = vertices[triangles[3*j+2]]
			};	
			
			// Model to World 
			tri = apply_transformation(get_model_matrix(go.transform), tri);
				
			// World to Camera
			tri = apply_transformation(get_view_matrix(*scene.cam) ,tri);

			// Camera to Clip
			bool clipped = false;	
			tri = apply_perspective_projection(
					&clipped,
					get_projection_matrix(*scene.cam),
					tri
			);

			if(!clipped) return;

			// Clip to Viewport
			tri = apply_transformation(get_viewport_matrix(*scene.cam),tri);

			// Rasterize
			rasterize_triangle(tri, &mat, framebuffer, zbuffer);
		}
	}
}

