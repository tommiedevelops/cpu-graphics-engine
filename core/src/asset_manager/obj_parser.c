#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "game_math/bounds.h"
#include "scene_manager/scene.h"
#include "error_log.h"

FILE* obj_open(char* filename){
	FILE* fp = fopen(filename, "r");

	if(fp == NULL){
		perror("src/obj_parser.c/open_obj: could not find filename");
		exit(EXIT_FAILURE);
	}

	return fp;
}

void obj_close(FILE* fp){
	if(fp==NULL){
		perror("provided file pointer was null");
		return;
	}
	fclose(fp);
}


int obj_parse_num_vertices(FILE* fp){
	// NULL CHECK
	if(fp==NULL){
		LOG_ERROR("file pointer was null");
		exit(EXIT_FAILURE);
	}

	char buf[30];
	int num_vertices = 0;

	// assuming that every line starting with v represents a VALID vertex
	while( fgets(buf, sizeof(buf), fp) != NULL ) {
		if (buf[0] == 'v' ) num_vertices++;
	}

	rewind(fp);
	return num_vertices;
}

int obj_parse_num_uvs(FILE* fp){

	char buf[256] = {0};
	const char * target = "# uv count =";
	int uv_count = 0;

	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(!strncmp(buf, target, strlen(target))){
			sscanf(buf, "# uv count = %d", &uv_count);
			break;
		}
	}
	rewind(fp);
	return uv_count;
}


Vec3f* obj_parse_vertices(FILE* fp, int num_vertices){

	if(num_vertices == 0) return NULL;

	// NULL CHECK
	if(fp==NULL){
		perror("src/obj_parser.c:parse_vertices: parameter(s) were NULL");
		exit(EXIT_FAILURE);
	}

	Vec3f* vertices = malloc(sizeof(Vec3f)*num_vertices);

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

Vec2f* obj_parse_uvs(FILE* fp, int num_uvs){

	if(num_uvs == 0) return NULL;

	// NULL CHECK
	if(fp==NULL){
		perror("src/obj_parser.c:parse_uvs: parameter(s) were NULL");
		exit(EXIT_FAILURE);
	}

	Vec2f* uvs = malloc(sizeof(Vec2f)*num_uvs);

	char buf[256] = {0};
	float x,y,z;
	int uv_index = 0;

	while( fgets(buf, sizeof(buf), fp) != NULL ) {
		if ( (buf[0] == 'v') && (buf[1] == 't')) {
			if( sscanf(buf, "vt %f %f %f\n",&x,&y,&z) == 3) {
				uvs[uv_index].x = x;
				uvs[uv_index].y = y;
				uv_index++;
			}
		}
	}

	rewind(fp);
	return uvs;
}

int obj_parse_num_triangles(FILE* fp) {

	char buf[256] = {0};

	int tri_count = 0;
	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(buf[0] == 'f') {
			tri_count++;
		}
	}
	rewind(fp);
	return tri_count;
}

int* obj_parse_triangle_uvs(FILE* fp, int num_triangles, int num_uvs, Vec2f* uvs){

	if(num_triangles == 0 || num_uvs == 0 || uvs == NULL) return NULL;

	// NULL check
	if( (uvs  == NULL) || (fp == NULL) ){
		perror("src/obj_parser.c/parse_triangle: parameter(s) are null");
		exit(EXIT_FAILURE);
	}

	if(num_uvs == 0) return NULL; // no uvs to parse

	// allocate memory
	int* triangle_uvs = malloc(sizeof(int)*3*num_triangles);

	char buf[256] = {0};
	int v0 = -1,v1 = -1,v2 = -1;
	int vt0 = -1, vt1 = -1, vt2 = -1;
	int tri_index = 0;
	int line_number = 0;

	while( (fgets(buf, sizeof(buf), fp) != NULL) ) {
		line_number++;
		if ( buf[0] == 'f' ) {
			if( sscanf(buf, "f %d/%d %d/%d %d/%d\n",&v0,&vt0,&v1,&vt1,&v2,&vt2) != 6 ) {
				// Texture vertices unavailable
				if(sscanf(buf, "f %d %d %d\n",&v0,&v1,&v2) != 3) {
					perror("src/obj_parser.c/parse_triangle: parsing error");
					exit(EXIT_FAILURE);
				}
			}

			// Wrap if negative
			vt0 = (vt0 > 0) ? vt0 - 1 : num_uvs + vt0;
			vt1 = (vt1 > 0) ? vt1 - 1 : num_uvs + vt1;
			vt2 = (vt2 > 0) ? vt2 - 1 : num_uvs + vt2;

			if(vt0 > num_uvs || vt1 > num_uvs || vt2 > num_uvs || vt0 < 0 || vt1 < 0 || vt2 < 0){
				printf("Something went wrong. vt0 = {%d}, vt1 = {%d}, vt2 = {%d}, num_uvs = {%d}\n", v0,v1,v2,num_uvs);
				exit(EXIT_FAILURE);
			}
		
			triangle_uvs[3*tri_index] = vt0;
			triangle_uvs[3*tri_index+1] = vt1; 
			triangle_uvs[3*tri_index+2] = vt2;	
			
			tri_index++;	
		}
	}

	rewind(fp);
	return triangle_uvs;
}


int* obj_parse_triangles(FILE* fp, int num_triangles, int num_vertices, Vec3f* vertices){

	if(num_triangles == 0 || num_vertices == 0) return NULL;

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
				// Texture vertices unavailable
				if(sscanf(buf, "f %d %d %d\n",&v0,&v1,&v2) != 3) {
					perror("src/obj_parser.c/parse_triangle: parsing error");
					exit(EXIT_FAILURE);
				}
			}

			// Wrap if negative
			v0 = (v0 > 0) ? v0 - 1 : num_vertices + v0;
			v1 = (v1 > 0) ? v1 - 1 : num_vertices + v1;
			v2 = (v2 > 0) ? v2 - 1 : num_vertices + v2;

			vt0 = (vt0 > 0) ? vt0 - 1 : num_vertices + v0;
			vt1 = (vt1 > 0) ? vt1 - 1 : num_vertices + v1;
			vt2 = (vt2 > 0) ? vt2 - 1 : num_vertices + v2;

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

