#include <stdio.h>
#include "scene_manager.h"

struct Mesh create_plane(float width, float length, float density) {
	/*
		width - width of the plane in world space
		length - length of the plane in world space
		density - vertices per unit length both ways
	*/

	// construct plane mesh
	int num_x_verts = density * width;
	int num_y_verts = density * length;

	struct Vec3f* vertices;
	int* triangles;	

	for(int y = 0; y < num_y_verts; y++){
		for(int x = 0; x < num_x_verts; x++){
						
		}
	}		
	
}
	
