#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>

#include "vertex.h"
#include "edge.h"
#include "bounds.h"

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

void parse_vertices(FILE* fp, int num_vertices, struct Vertex* vertices){

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
				vertices[vertex_index].x = x;
				vertices[vertex_index].y = y;
				vertices[vertex_index].z = z;
				vertex_index++;
			}
		}
	}

}

/*In memory, an edge will be an ordered pair of vertices (a triplet of floats) */
//TODO: might be better to store a reference to the vertex instead of the actual value. for future me.
/* edges = [e0_x_from, e0_y_from, e0_z_from, e0_x_to, e0_y_to, e0_z_to, e1_x_from ... ]*/
void parse_edges(FILE* fp, float* edges, int num_edges, struct Vertex* vertices) {

	if(edges==NULL){
		perror("provided edges float* is null");
		exit(EXIT_FAILURE); // gonna cause mem leaks pls fix
	}
	if(vertices==NULL){
		perror("provided vertices float * is null");
		exit(EXIT_FAILURE); // gonna cause mem leaks pls fix
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
				edges[6*edge_index + 0] = vertices[v0].x;
				edges[6*edge_index + 1] = vertices[v0].y;
				edges[6*edge_index + 2] = vertices[v0].z;
				edges[6*edge_index + 3] = vertices[v0].x;
				edges[6*edge_index + 4] = vertices[v1].y;
				edges[6*edge_index + 5] = vertices[v1].z;

				edge_index++;

				// v1 -> v2
				edges[6*edge_index + 0] = vertices[v1].x;
				edges[6*edge_index + 1] = vertices[v1].y;
				edges[6*edge_index + 2] = vertices[v1].z;
				edges[6*edge_index + 3] = vertices[v2].x;
				edges[6*edge_index + 4] = vertices[v2].y;
				edges[6*edge_index + 5] = vertices[v2].z;

				edge_index++;

				//v2 -> v0
				edges[6*edge_index + 0] = vertices[v2].x;
				edges[6*edge_index + 1] = vertices[v2].y;
				edges[6*edge_index + 2] = vertices[v2].z;
				edges[6*edge_index + 3] = vertices[v0].x;
				edges[6*edge_index + 4] = vertices[v0].y;
				edges[6*edge_index + 5] = vertices[v0].z;

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

	struct Vertex* vertices = malloc(sizeof(struct Vertex)*num_vertices);
	memset(vertices,0x0,sizeof(struct Vertex)*num_vertices);

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
