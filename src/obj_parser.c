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

void update_bounds(float* bounds, float* vertices, int num_vertices) {
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

	bounds[0] = xmin; bounds[1] = xmax;
	bounds[2] = ymin; bounds[3] = ymax;
	bounds[4] = zmin; bounds[5] = zmax;

}


/* Shifts coordinates on al three axes to [0, imax - imin] */
void shift_to_origin(float* bounds, float* vertices, int num_vertices) {
	for(int i = 0; i < num_vertices; i++){
		vertices[3*i] -= bounds[0];
		vertices[3*i+1] -= bounds[2];
		vertices[3*i+2] -= bounds[4];
	}

	update_bounds(bounds, vertices, num_vertices);
}

/* Normalizes values to between [-1,1] - assumes min = 0 for all axes */
void normalize_lengths(float* bounds, float* vertices, int num_vertices) {

	for(int i = 0; i < num_vertices; i++){
		vertices[3*i] = (float)vertices[3*i] / bounds[1];
		vertices[3*i + 1] = (float)vertices[3*i + 1] / bounds[3];
		vertices[3*i + 2] = (float)vertices[3*i + 2] / bounds[5];
	}


	for(int i = 0; i <3* num_vertices; i++){
		vertices[i] *= 2;
		vertices[i] -= 1;
	}
	update_bounds(bounds, vertices, num_vertices);

}

/* Takes normalized vertices between [-1,1] and scales them to [-target_length/2, target_length/2] */
void scale_lengths(float target_length, float* bounds, float* vertices, int num_vertices){
	for(int i = 0; i < num_vertices*3; i++){
		vertices[i] *= (float)target_length/2.0f;
	}
	update_bounds(bounds, vertices, num_vertices);
}

// normalize vertices to fit in box, side length s that is centred on the origin all axes scaled equally.
void normalize_vertices(float sidelength, float* vertices, int num_vertices){
	// find the maximum vertex value along x, y AND z.
	// divide all vertices by that value (now btn 0 and 1)
	float* bounds = get_bounds(vertices, num_vertices);

	shift_to_origin(bounds, vertices, num_vertices);
	normalize_lengths(bounds, vertices, num_vertices);
	scale_lengths(sidelength, bounds, vertices, num_vertices);

	free(bounds);
}

/*In memory, an edge will be an ordered pair of vertices (a triplet of floats) */
//TODO: might be better to store a reference to the vertex instead of the actual value. for future me.
/* edges = [e0_x_from, e0_y_from, e0_z_from, e0_x_to, e0_y_to, e0_z_to, e1_x_from ... ]*/
void parse_edges(FILE* fp, float* edges, int num_edges, float* vertices) {

	if(edges==NULL){
		perror("provided edges float* is null");
		return;
	}
	if(vertices==NULL){
		perror("provided vertices float * is null");
		return;
	}

	char buf[256] = {0};
	int v0,v1,v2;
	int edge_index = 0;

	while( (fgets(buf, sizeof(buf), fp) != NULL) ) {
		if ( buf[0] == 'f' ) {
			if( sscanf(buf, "f %d %d %d\n",&v0,&v1,&v2) == 3) {

				// .obj files start from index 1 so have to account for that
				v0--; v1--; v2--;

				// v0 -> v1
				edges[6*edge_index + 0] = vertices[3*v0 + 0];
				edges[6*edge_index + 1] = vertices[3*v0 + 1];
				edges[6*edge_index + 2] = vertices[3*v0 + 2];
				edges[6*edge_index + 3] = vertices[3*v1 + 0];
				edges[6*edge_index + 4] = vertices[3*v1 + 1];
				edges[6*edge_index + 5] = vertices[3*v1 + 2];

				edge_index++;

				// v1 -> v2
				edges[6*edge_index + 0] = vertices[3*v1 + 0];
				edges[6*edge_index + 1] = vertices[3*v1 + 1];
				edges[6*edge_index + 2] = vertices[3*v1 + 2];
				edges[6*edge_index + 3] = vertices[3*v2 + 0];
				edges[6*edge_index + 4] = vertices[3*v2 + 1];
				edges[6*edge_index + 5] = vertices[3*v2 + 2];

				edge_index++;

				//v2 -> v0
				edges[6*edge_index + 0] = vertices[3*v2 + 0];
				edges[6*edge_index + 1] = vertices[3*v2 + 1];
				edges[6*edge_index + 2] = vertices[3*v2 + 2];
				edges[6*edge_index + 3] = vertices[3*v0 + 0];
				edges[6*edge_index + 4] = vertices[3*v0 + 1];
				edges[6*edge_index + 5] = vertices[3*v0 + 2];

				edge_index++;
			}
		}
	}

}

int parse_num_edges(FILE* fp) {
	// extract face count
	char buf[256] = {0};
	const char * target = "# face count =";
	int face_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, sizeof(target))){
			sscanf(buf, "# face count = %d", &face_count);
			break;
		}
	}

	// for every 'face' (i.e. triangle) there are 3 edges
	int edge_count = 3 * face_count;
	return edge_count;
}

int* parse_obj_to_2D_coord_array(char* filename){
	FILE* fp = open_obj(filename);

	// vertices array: [v0x,v0y,v0z,v1x,v1y,v1z,...]
	int num_vertices = parse_num_vertices(fp);
	rewind(fp);

	float* vertices = malloc(sizeof(float)*num_vertices*3);
	memset(vertices,0x0,sizeof(float)*num_vertices*3);

	const float scale = 600.0f;
	parse_vertices(fp, num_vertices, vertices);
	rewind(fp);

	normalize_vertices(scale, vertices, num_vertices);

	int num_edges = parse_num_edges(fp);
	rewind(fp);

	// edges array: edges = [e0_x_from, e0_y_from, e0_z_from, e0_x_to, e0_y_to, e0_z_to, e1_x_from ... ]
	// i.e. every edge needs 6 floats to describe it
	int edges_array_size = sizeof(float)*num_edges*6;
	float* edges = malloc(edges_array_size);
	memset(edges,0x0,edges_array_size);

	parse_edges(fp, edges, num_edges, vertices);
	rewind(fp);

	int* coords = malloc(sizeof(int)*4*num_edges);
	memset(coords,0x0,sizeof(int)*4*num_edges);

	for(int i = 0; i < num_edges; i++){
		coords[4*i] = edges[6*i];
		coords[4*i+1]= edges[6*i+1];
		coords[4*i+2] = edges[6*i+3];
		coords[4*i+3] = edges[6*i+4];
	}

	
	free(edges);
	free(vertices);
	close_obj(fp);
	return coords;
}
