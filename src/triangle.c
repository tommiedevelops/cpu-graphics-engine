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
