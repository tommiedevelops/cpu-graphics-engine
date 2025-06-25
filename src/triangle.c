#include <stdio.h>
#include <stdlib.h>
#include "triangle.h"

struct Triangle create_triangle(
	struct Vertex* a,
	struct Vertex* b,
	struct Vertex* c
){
	if( (a == NULL) || (b == NULL) || (c == NULL) ){
		perror("src/triangle.c/create_triangle:provided parameter(s) are null");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	struct Triangle tri = {.a=a,.b=b,.c=c};
	return tri;
}


void swap(void** a, void** b){
	void* temp = *a;
	*a = *b;
	*b = temp;
}

struct Vertex** sort_vertices_by_y_asc(struct Triangle tri) {
	struct Vertex** arr  = malloc(sizeof(struct Vertex*)*3);
	// [ptr_0 ptr_1 ptr_2]

	if(arr == NULL){
		perror("src/triangle.c/sort_vertices_by_y:malloc failed");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	arr[0] = tri.a; arr[1] = tri.b; arr[2] = tri.c;

	// Mini 3-element bubble sort to order vertices by ascending y
	if (arr[1]->y < arr[0]->y) swap((void**)&arr[0],(void**)&arr[1]);
	if (arr[2]->y < arr[1]->y) swap((void**)&arr[2],(void**)&arr[1]);
	if (arr[1]->y < arr[0]->y) swap((void**)&arr[0],(void**)&arr[1]);

	return arr;
}


struct Vertex** sort_vertices_by_x_asc(struct Triangle tri) {
	struct Vertex** arr  = malloc(sizeof(struct Vertex*)*3);
	// [ptr_0 ptr_1 ptr_2]

	if(arr == NULL){
		perror("src/triangle.c/sort_vertices_by_x:malloc failed");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	arr[0] = tri.a; arr[1] = tri.b; arr[2] = tri.c;

	// Mini 3-element bubble sort to order vertices by ascending y
	if (arr[1]->x < arr[0]->x) swap((void**)&arr[0],(void**)&arr[1]);
	if (arr[2]->x < arr[1]->x) swap((void**)&arr[2],(void**)&arr[1]);
	if (arr[1]->x < arr[0]->x) swap((void**)&arr[0],(void**)&arr[1]);

	return arr;
}

// Rasterizes a bounding box
// NOTE: currently assuming we are projecting onto x-y plane
struct PointArray rasterize_bounding_box(struct Bounds bounds){

	struct Vertex bot_left = {.x = bounds.xmin, .y= bounds.ymin, .z = 0.0f};
	struct Vertex top_right = {.x = bounds.xmax, .y = bounds.ymax, .z = 0.0f};

	int num_points = (int)(bounds.xmax - bounds.xmin)*(bounds.ymax - bounds.ymin);

	struct Point* points = malloc(sizeof(struct Point)*num_points);


	printf("bounds.xmin = {%d} bounds.xmax = {%d}\n", (int)bounds.xmin, (int)bounds.xmax);
	printf("bounds.ymin = {%d} bounds.ymax = {%d}\n", (int)bounds.ymin, (int)bounds.ymax);

	int points_index = 0;
	for(int y = (int)bounds.ymin; y < (int)bounds.ymax; y++){
		for(int x = (int)bounds.xmin; x < (int)bounds.xmax; x++) {
			points[points_index].x = x;
			points[points_index].y = y;
			points_index++;
		}
	}

	struct PointArray point_array = { .points = points, .num_points = num_points};
	return point_array;
}

void cull_points_not_in_triangle(struct PointArray point_array, struct Triangle tri){
	// convert point_array to BaryPointArray (copy) w.r.t tri vertices
	struct BaryPointArray bary_point_array =
	convert_point_arr_to_bary_point_arr(point_array, tri);

	destroy_bary_point_array(bary_point_array);
}

// Rasterizes a single triangle
struct PointArray rasterize_triangle(struct Triangle tri) {
	// sort vertices in ascending y
	struct Vertex** sorted_verts = sort_vertices_by_y_asc(tri);

	struct Vertex vertices[3] = {
		*sorted_verts[0],
		*sorted_verts[1],
		*sorted_verts[2]
	};

	struct Bounds bounds = get_bounds(vertices, 3);
	struct PointArray point_array = rasterize_bounding_box(bounds);
	return point_array;
}

