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
	framebuffer[x + WIDTH*y] = value;
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
		struct Vec2f* uvs = NULL;

		if(go.mesh != NULL) {
			vertices = go.mesh->vertices;
			uvs = go.mesh->uvs;
			triangles = go.mesh->triangles;
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

			struct Triangle tri = {0};

			// positions
			tri.v0 = vertices[triangles[3*j]];
			tri.v1 = vertices[triangles[3*j+1]];
			tri.v2 = vertices[triangles[3*j+2]];
	
			// uvs
			if(uvs != NULL) {
				tri.uv0 = uvs[triangles[3*j]];
				tri.uv1 = uvs[triangles[3*j+1]];
				tri.uv2 = uvs[triangles[3*j+2]];
			}; 
	
			// Model to World 
			tri = apply_transformation(get_model_matrix(go.transform), tri);
			
			// World to Camera
			tri = apply_transformation(get_view_matrix(*scene.cam) ,tri);

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
			rasterize_triangle(tri, &mat, framebuffer, zbuffer);
		}
	}
}

