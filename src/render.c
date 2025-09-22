#include <stdio.h> 
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vec3f.h"
#include "render.h"
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
		
		struct GameObject go = *scene.gameObjects[i];

		struct Vec3f* vertices = NULL;
		int* triangles = NULL;

		if(go.mesh != NULL) {
			struct Vec3f* vertices = go.mesh->vertices;
			int* triangles = go.mesh->triangles;
		} else {
			//LOG_ERROR("No mesh to render");
			return;
		}

		struct Material mat = material_default();

		if(go.material != NULL) {
			mat = *go.material;
		}

		// Transform and rasterize each triangle
		for(int j = 0; j < go.mesh->num_triangles; j++) {

			struct Triangle tri = {
				.v0 = vertices[triangles[3*j]],
				.v1 = vertices[triangles[3*j+1]],
				.v2 = vertices[triangles[3*j+2]]
			};	
			
			// Model to World 
			tri = apply_transformation(get_model_matrix(go.transform), tri);
				
			// World to Camera
			tri = apply_transformation(get_view_matrix(*scene.cam) ,tri);

			// ADDING TEXTURES LIVES BETWEEN HERE
		
			// decide on color based on go.mat

			bool clipped = false;	

			tri = apply_perspective_projection(
					&clipped,
					get_projection_matrix(*scene.cam),
					tri
			);

			if(!clipped) return;

			// at this point the tri contains 'fragments' - points in Clip Space
			// i need to calculate the color of each vertex, then interpolate nicely across its surface
			// what's the difference between color and lighting tho? 

			// Clip to Viewport
			tri = apply_transformation(get_viewport_matrix(*scene.cam),tri);

			// Rasterize
			rasterize_triangle(tri, &mat, framebuffer, zbuffer);

			// Triangle now contains 'fragments' (potential pixels)

			uint32_t icolor;

			if(i==3){
				// color the ground mesh red
				// hacky
				icolor = COLOR_BLUE;
			}

			// Rasterize
			rasterize_triangle(tri, go.material, framebuffer, zbuffer);

			// the fragment shader is actually after this point
			
			// merge(Fragment frag, uint32_t* backbuffer);
			// AND HERE
		}
	}
}

