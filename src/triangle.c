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

struct Vertex** sort_vertices_by_y(struct Triangle tri) {
	struct Vertex** sorted  = malloc(sizeof(struct Triangle*)*3);

	if(sorted == NULL){
		perror("src/triangle.c/sort_vertices_by_y:malloc failed");
		exit(EXIT_FAILURE); //TODO handle gracefully
	}

	sorted[0] = tri.a; sorted[1] = tri.b; sorted[2] = tri.c;
	//TODO complete sorting algo (bubble sort)
	return NULL;
}

