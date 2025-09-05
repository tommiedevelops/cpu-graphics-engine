#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "bounds.h"
#include "obj_parser.h"
#include "constants.h"
#include "scene_manager.h"

/*
Purpose: create .input files that can be understood by my line renderer from .obj file
*/

FILE* open_obj(char* filename){
	FILE* fp = fopen(filename, "r");

	if(fp == NULL){
		perror("src/obj_parser.c/open_obj: could not find filename");
		exit(EXIT_FAILURE);
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


static int parse_num_vertices(FILE* fp){

	char buf[256] = {0};
	const char * target = "# vertex count =";
	int vertex_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, strlen(target))){
			sscanf(buf, "# vertex count = %d", &vertex_count);
			break;
		}
	}
	rewind(fp);
	return vertex_count;
}

static struct Vec3f* parse_vertices(FILE* fp, int num_vertices){

	// NULL CHECK
	if(fp==NULL){
		perror("src/obj_parser.c:parse_vertices: parameter(s) were NULL");
		exit(EXIT_FAILURE);
	}

	struct Vec3f* vertices = malloc(sizeof(struct Vec3f)*num_vertices);

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
	rewind(fp);
	return vertices;
}

static int parse_num_triangles(FILE* fp) {

	char buf[256] = {0};
	const char * target = "# face count =";
	int face_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, strlen(target))){
			sscanf(buf, "# face count = %d", &face_count);
			break;
		}
	}
	rewind(fp);
	return face_count;
}

static int* parse_triangles(FILE* fp, int num_triangles, int num_vertices, struct Vec3f* vertices){

	// NULL check
	if( (vertices == NULL) || (fp == NULL) ){
		perror("src/obj_parser.c/parse_triangle: parameter(s) are null");
		exit(EXIT_FAILURE);
	}

	// allocate memory
	int* triangles = malloc(sizeof(int)*3*num_triangles);

	char buf[256] = {0};
	int v0 = -1,v1 = -1,v2 = -1;
	int vt0 = -1, vt1 = -1, vt2 = -1;
	int tri_index = 0;
	int line_number = 0;

	while( (fgets(buf, sizeof(buf), fp) != NULL) ) {
		line_number++;
		if ( buf[0] == 'f' ) {
			if( sscanf(buf, "f %d/%d %d/%d %d/%d\n",&v0,&vt0,&v1,&vt1,&v2,&vt2) != 6 ) {
				if(sscanf(buf, "f %d %d %d\n",&v0,&v1,&v2) != 3) {
					perror("src/obj_parser.c/parse_triangle: parsing error");
					exit(EXIT_FAILURE);
				}
			}

			v0 = (v0 > 0) ? v0 - 1 : num_vertices + v0;
			v1 = (v1 > 0) ? v1 - 1 : num_vertices + v1;
			v2 = (v2 > 0) ? v2 - 1 : num_vertices + v2;

			if(v0 > num_vertices || v1 > num_vertices || v2 > num_vertices || v0 < 0 || v1 < 0 || v2 < 0){
				printf("Something went wrong. v0 = {%d}, v1 = {%d}, v2 = {%d}, num_vertices = {%d}\n", v0,v1,v2,num_vertices);
				exit(EXIT_FAILURE);
			}
		
			triangles[3*tri_index] = v0;
			triangles[3*tri_index+1] = v1; 
			triangles[3*tri_index+2] = v2;	
			tri_index++;	
		}
	}
	rewind(fp);
	return triangles;
}

struct Mesh parse_obj(char* filename){
	FILE* fp = open_obj(filename);

	int num_vertices = parse_num_vertices(fp);
	struct Vec3f* vertices = parse_vertices(fp, num_vertices);

	normalize_vertices(LENGTH_SCALE, vertices, num_vertices);

	int num_triangles = parse_num_triangles(fp);
	int* triangles = parse_triangles(fp, num_triangles, num_vertices, vertices);

	close_obj(fp);

	struct Mesh data = {
		.num_vertices = num_vertices,
		.vertices = vertices,
		.num_triangles = num_triangles,
		.triangles = triangles
	};

return data;
}
