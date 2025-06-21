#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

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


int parse_num_vertices(char* filename){
	FILE* fp = open_obj(filename);
	char buf[256] = {0};
	const char * target = "# vertex count =";
	int vertex_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, sizeof(target))){
			sscanf(buf, "# vertex count = %d", &vertex_count);
			break;
		}
	}
	close_obj(fp);
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
void parse_edges(FILE* fp, struct Edge* edges, int num_edges, struct Vertex* vertices) {

	if(edges==NULL){
		perror("provided edges float* is null");
		exit(EXIT_FAILURE); // gonna cause mem leaks pls fix
	}
	if(vertices==NULL){
		perror("obj_parser.c/parse_edges:provided vertices float * is null");
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
				edges[edge_index].from = vertices + v0;
				edges[edge_index].to = vertices + v1;

				edge_index++;

				// v1 -> v2
				edges[edge_index].from = vertices + v1;
				edges[edge_index].to = vertices + v2;

				edge_index++;

				//v2 -> v0
				edges[edge_index].from = vertices + v2;
				edges[edge_index].to = vertices + v0;

				edge_index++;
			}
		}
	}

}

int parse_num_edges(char* filename) {
	// extract face count
	FILE* fp = open_obj(filename);
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
	close_obj(fp);
	return edge_count;
}

struct Vertex* parse_vertices_from_obj(char* filename) {
	// vertices array: [v0x,v0y,v0z,v1x,v1y,v1z,...]
	int num_vertices = parse_num_vertices(filename);

	struct Vertex* vertices = malloc(sizeof(struct Vertex)*num_vertices);
	memset(vertices,0x0,sizeof(struct Vertex)*num_vertices);

	FILE* fp = open_obj(filename);
	const float scale = 200.0f; // hard coded
	parse_vertices(fp, num_vertices, vertices);
	close_obj(fp);

	normalize_vertices(scale, vertices, num_vertices);

	return vertices;
}

struct Edge* parse_edges_from_obj(char* filename, struct Vertex* parsed_vertices){
	FILE* fp = open_obj(filename);

	if(parsed_vertices == NULL){
		perror("src/obj_parser.c/parse_edges_from_obj:provided struct Vertex* is null");
		exit(EXIT_FAILURE);
	}

	int num_edges = parse_num_edges(filename);
	rewind(fp);

	struct Edge* edges = malloc(sizeof(struct Edge)*num_edges);
	memset(edges,0x0,sizeof(struct Edge)*num_edges);

	parse_edges(fp, edges, num_edges, parsed_vertices);
	rewind(fp);

	close_obj(fp);
	return edges;
}
