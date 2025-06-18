#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>

/*
Purpose: create .input files that can be understood by my line renderer from .obj file
*/


FILE* open_obj(char* filename){
	FILE* fp = fopen(filename, "r");

	if(fp == NULL){
		perror("Could not find filename");
		return NULL;
	}

	return fp;
}

void close_obj(FILE* fp){
	if(fp==NULL){
		perror("provided file pointer was null");
		return;
	}
	fclose(fp);
}


int parse_num_vertices(FILE* fp){
	char buf[256] = {0};
	const char * target = "# vertex count =";
	int vertex_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, sizeof(target))){
			sscanf(buf, "# vertex count = %d", &vertex_count);
			break;
		}
	}
	return vertex_count;
}

void parse_vertices(FILE* fp, int num_vertices, float* vertices){

	if(vertices==NULL){
		perror("provided float* was null");
		return;
	}


	char buf[256] = {0};
	float x,y,z;
	int vertex_index = 0;

	while( fgets(buf, sizeof(buf), fp) != NULL ) {
		if (buf[0] == 'v' ) {
			if( sscanf(buf, "v %f %f %f\n",&x,&y,&z) == 3) {
				vertices[3*vertex_index]     = x;
				vertices[3*vertex_index + 1] = y;
				vertices[3*vertex_index + 2] = z;
				vertex_index++;
				printf("parse_vertices:success. {%f,%f,%f}\n", x,y,z);
			}
		}
	}

}



float* get_bounds(float* vertices, int num_vertices) {
	/* return: [xmin, xmax, ymin, ymax, zmin, zmax] */
	float xmin =  FLT_MAX, ymin =  FLT_MAX, zmin =  FLT_MAX;
	float xmax = -FLT_MAX, ymax = -FLT_MAX, zmax = -FLT_MAX;

	for(int i = 0; i < num_vertices; i++){
		if( vertices[3*i    ] > xmax ){ xmax = vertices[3*i    ]; }
		if( vertices[3*i    ] < xmin ){ xmin = vertices[3*i    ]; }
		if( vertices[3*i + 1] > ymax ){ ymax = vertices[3*i + 1]; }
		if( vertices[3*i + 1] < ymin ){ ymin = vertices[3*i + 1]; }
		if( vertices[3*i + 2] > zmax ){ zmax = vertices[3*i + 2]; }
		if( vertices[3*i + 2] < zmin ){ zmin = vertices[3*i + 2]; }
	}

	float* bounds = malloc(sizeof(float)*6);

	bounds[0] = xmin; bounds[1] = xmax;
	bounds[2] = ymin; bounds[3] = ymax;
	bounds[4] = zmin; bounds[5] = zmax;

	return bounds;
}


// normalize vertices to fit in box, side length s that is centred on the origin all axes scaled equally.

void normalize_vertices(float sidelength, float* vertices, int num_vertices){
	// find the maximum vertex value along x, y AND z.
	// divide all vertices by that value (now btn 0 and 1)
	float* bounds = get_bounds(vertices, num_vertices);

	free(bounds);
}

void shift_to_origin(float* bounds, float* vertices, int num_vertices) {
	//TODO
}

void normalize_lengths(float* bounds, float* vertices, int num_vertices) {
	//TODO
	// normalize range to be between [-1,1]
}

void scale_lengths(float target_length, float* bounds, float* vertices, int num_vertices){
	//TODO
	// multiplying the range to be between [-target_length/2, target_length/2]
}

void parse_obj(char* filename){
	FILE* fp = open_obj(filename);
	int num_vertices = parse_num_vertices(fp);
	rewind(fp);

	// array: [v0x,v0y,v0z,v1x,v1y,v1z,...]
	float* vertices = malloc(sizeof(float)*num_vertices*3);
	memset(vertices,0x0,sizeof(float)*num_vertices*3);

	parse_vertices(fp, num_vertices, vertices);


	free(vertices);
	close_obj(fp);
}
