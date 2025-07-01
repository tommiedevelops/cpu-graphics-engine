#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "compute_line.h"
#include "edge.h"
#include "vertex.h"
#include "triangle.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
                if( (x > WIDTH) || (x < 0) ) {printf("line.c/place_pixel: invalid x value. point= {%d,%d}\n", x,y); return;}
                if( (y > HEIGHT) || (y < 0) ) {printf("line.c/place_pixel: invalid y value. point= {%d,%d}\n", x,y); return;}

                framebuffer[x + WIDTH*y] = value;
}

void draw_points_to_framebuffer(struct Point* points, uint32_t *framebuffer, int num_points){
	bool error = false;

	if( (points == NULL) || (framebuffer == NULL) ) {
		perror("points or framebuffer is null");
		return;
	}

	for(int i = 0; i < num_points; i++){

		if( (points[i].x > WIDTH) || (points[i].x < 0) ) {
			printf("invalid x value. point={%d,%d}\n", points[i].x, points[i].y);
			error = true;
		}

		if( (points[i].y > HEIGHT) || (points[i].y < 0) ) {
			printf("invalid y value. point={%d,%d}\n", points[i].x, points[i].y);
			error = true;
		}
 		if(error) {return;}

		framebuffer[points[i].x + WIDTH * points[i].y] = COLOR_RED;
	}
}

int render_lines(uint32_t *framebuffer, int* coords, int num_coords) {
	// array organisation:
	// [[start_coord_x_0],[start_coord_y_0],[end_coord_x_0], [end_coord_y_0] ...]
	// where [start_coord_X_X] and [end_coord_X_X] are integers.
	for(int i = 0; i < num_coords; i++){
		int* coord = coords + 4*i;
		printf("drawing line: {%d,%d} -> {%d,%d}\n", *coord, *(coord+1),*(coord+2),*(coord+3));

		int x0 = *coord;
		int y0 = *(coord+1);
		int x1 = *(coord+2);
		int y1 = *(coord+3);

		struct Point* line = draw_line_easy(x0,y0,x1,y1);
		int num_points = compute_num_points(x0,y0,x1,y1);
		printf("render_lines:num_points={%d}\n", num_points);
		draw_points_to_framebuffer(line, framebuffer, num_points);
		free(line);
	}
	return 0;
}

/* Assuming wireframe_vertices are centred at (0,0) */
/* and structured like so: [x0,y0,z0,x1,y1,z1...]*/
void render_wireframe(uint32_t* framebuffer, struct Edge* wireframe_edges, int num_edges){

	if(framebuffer == NULL){
		perror("provided framebuffer is null");
		exit(EXIT_FAILURE);
	}
	if(wireframe_edges == NULL) {
		perror("provided edge array is null");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < num_edges; i++){
		if( (wireframe_edges[i].from == NULL) || (wireframe_edges[i].to == NULL) ){
			perror("provided edge was null");
			exit(EXIT_FAILURE); // HANDLE GRACEFULLY
		}

		struct Vertex* from = wireframe_edges[i].from;
		struct Vertex* to = wireframe_edges[i].to;

		convert_vertex_to_int_values(from);
		convert_vertex_to_int_values(to);

		// assumes we are neglecting z values
		// can probably generalise to a proper projection in the future
		struct Point* line = draw_line_easy(from->x, from->y, to->x, to->y);
		int num_points = compute_num_points(from->x, from->y, to->x, to->y);
		draw_points_to_framebuffer(line, framebuffer, num_points);

		free(line);
	}
}

void render_triangles(uint32_t* framebuffer, struct Triangle* triangles, int num_triangles){

	// draw edges
	for(int i = 0; i < num_triangles; i++){
		struct Point* line1 = draw_line_easy(triangles[i].a->x, triangles[i].a->y, triangles[i].b->x, triangles[i].b->y);
		struct Point* line2 = draw_line_easy(triangles[i].b->x, triangles[i].b->y, triangles[i].c->x, triangles[i].c->y);
		struct Point* line3 = draw_line_easy(triangles[i].a->x, triangles[i].a->y, triangles[i].c->x, triangles[i].c->y);
		draw_points_to_framebuffer(line1, framebuffer, compute_num_points(triangles[i].a->x, triangles[i].a->y, triangles[i].b->x, triangles[i].b->y));
		draw_points_to_framebuffer(line2, framebuffer, compute_num_points(triangles[i].b->x, triangles[i].b->y, triangles[i].c->x, triangles[i].c->y));
		draw_points_to_framebuffer(line3, framebuffer, compute_num_points(triangles[i].a->x, triangles[i].a->y, triangles[i].c->x, triangles[i].c->y));
	}

	// fill in middle
	for(int i = 0; i < num_triangles; i++){
		struct PointArray triangle_points = rasterize_triangle(triangles[i]);
		draw_points_to_framebuffer(triangle_points.points, framebuffer, triangle_points.num_points); 
		destroy_point_array(triangle_points);
	}
}
