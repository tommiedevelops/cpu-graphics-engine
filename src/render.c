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
                if( (x > WIDTH) || (x < 0) ) {printf("render.c/place_pixel: invalid x value. pixel= {%d,%d}\n", x,y); return;}
                if( (y > HEIGHT) || (y < 0) ) {printf("render.c/place_pixel: invalid y value. pixel= {%d,%d}\n", x,y); return;}

                framebuffer[x + WIDTH*y] = value;
}

void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene) {
	if(scene.gameObjects == NULL){
		printf("src/render.c/render_scene: no gameObjects to render\n");
		return;
	}

	struct Mat4 V = get_view_matrix(*scene.cam);
	struct Mat4 P = get_projection_matrix(*scene.cam);
	struct Mat4 VP = get_viewport_matrix(*scene.cam);

	for(int i = 0; i < scene.num_gameObjects; i++) {
		struct GameObject go = scene.gameObjects[i];
		struct Mat4 M = get_model_matrix(go.transform);
			
		struct Vec3f* vertices = go.mesh.vertices;
		int* triangles = go.mesh.triangles;

		for(int i = 0; i < go.mesh.num_triangles; i++) {
			struct Triangle tri = {
				.v0 = vertices[triangles[3*i]],
				.v1 = vertices[triangles[3*i+1]],
				.v2 = vertices[triangles[3*i+2]]
			};	

			tri = apply_transformation(M, tri);
				
			// calculate lighting	
			struct Vec3f surf_norm = vec3f_normalize(calculate_normal(tri));
			struct Vec3f light = vec3f_normalize(scene.light.direction);	
			float dot_prod = dot_product(surf_norm, light);
			
			// calculate color
			struct Color color;
			color.a = 255;
			color.r = dot_prod * 256;
			color.g = dot_prod * 256;
			color.b = dot_prod * 256;

			uint32_t icolor = color_to_int(color);	
					
			tri = apply_transformation(V,tri);
			
			bool clipped = false;	
			tri = apply_perspective_projection(&clipped,P,tri);
			if(!clipped) return;	
			
			tri = apply_transformation(VP,tri);

			rasterize_triangle(tri, framebuffer, zbuffer, icolor);
		}
	}
}

