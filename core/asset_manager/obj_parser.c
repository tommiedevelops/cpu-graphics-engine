#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "bounds.h"
#include "constants.h"
#include "scene_manager.h"

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

int obj_parse_num_normals(FILE* fp){

	if(!fp) return -1;

	char buf[256] = {0};
	int n = 0;

	while(fgets(buf, sizeof(buf), fp) != NULL) {
	    if (buf[0] == 'v' && buf[1] == 'n') n++;
	}

	rewind(fp);
	return n;
}


int obj_parse_num_vertices(FILE* fp){

	if(!fp) return -1;

	char buf[256] = {0};
	int num_verts = 0;

	while(fgets(buf, sizeof(buf), fp) != NULL) {
	    if (buf[0] == 'v' && buf[1] == ' ') num_verts++;
	}

	rewind(fp);
	return num_verts;
}

int obj_parse_num_uvs(FILE* fp){

	if(!fp) return -1;

	char buf[256] = {0};
	int n = 0;

	while(fgets(buf, sizeof(buf), fp) != NULL) {
	    if (buf[0] == 'v' && buf[1] == 't') n++;
	}

	rewind(fp);
	return n;
}


int obj_parse_num_triangles(FILE* fp){

	if(!fp) return -1;

	char buf[256] = {0};
	int n = 0;

	while(fgets(buf, sizeof(buf), fp) != NULL) {
	    if (buf[0] == 'f') n++;
	}

	rewind(fp);
	return n;
}


Vec3f* obj_parse_vertices(FILE* fp, int num_vertices){

	if(!fp || num_vertices == 0) return NULL;

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

	if(!fp || num_uvs == 0) return NULL;
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

Vec3f* obj_parse_normals(FILE* fp, int num_normals){

	if(!fp || num_normals == 0) return NULL;

	Vec3f* normals = malloc(sizeof(Vec3f)*num_normals);

	char buf[256] = {0};
	float x,y,z;
	int n_index = 0;

	while( fgets(buf, sizeof(buf), fp) != NULL ) {
		if ( (buf[0] == 'v') && (buf[1] == 'n')) {
			if( sscanf(buf, "vn %f %f %f\n",&x,&y,&z) == 3) {
				normals[n_index].x = x;
				normals[n_index].y = y;
				normals[n_index].z = z;
				n_index++;
			}
		}
	}

	rewind(fp);
	return normals;
}

static inline void wrap_int(int* n, int max) { n = (n > 0) ? n - 1 : n + max; }

static void parse_token(char * tok, int* v, int n_tris, int* vt, int n_uvs, int* vn, int n_norms){
	if(sscanf(tok, "%d/%d/%d", v, vt, vn) == 3) {
		// v/vt/vn
		wrap_int(v, n_tris);
		wrap_int(vt, n_uvs);
		wrap_int(vn, n_norms);
	} else if(sscanf(tok, "%d//%d", v, vn) == 2) {
		// v//vn
		wrap_int(v, n_tris);
		wrap_int(vn, n_norms);
	} else if(sscanf(tok, "%d/%d", v, vt) == 2) {
		wrap_int(v, n_tris);
		wrap_int(vt, n_uvs);	
	} else if(sscanf(tok, "%d", v) == 1) {
		wrap_int(v, n_tris);
	} else {
		printf("Cannot recognise obj format\n");
		exit(EXIT_FAILURE);
	}
}

static void parse_line(char* buf, int* tris, int n_tris, int* uvs, int n_uvs, int* norms, int n_norms, int tri_idx) {

	int v[3], vt[3], vn[3]; 

	char* tok = strtok(buf, " ");
	if(!tok || tok[0] != 'f') return;

	// -1 => no value
	int v_idx = 0;
	while( (tok = strtok(NULL, " ")) && v_idx < 3) {

		v[v_idx]  = -1; 
		vt[v_idx] = -1;
		vn[v_idx] = -1;

		parse_token(tok, v, n_tris, vt, n_uvs, vn, n_norms);

		v_idx++;
	}

	for(int i = 0; i < 3; i++){
		tris[tri_idx + i] = v[i];
		if(uvs) uvs[tri_idx + i] = vt[i];
		if(norms) norms[tri_idx + i] = vn[i];
	}
}

void obj_parse_triangles(FILE* fp, int num_tris, int num_verts, int num_norms, int num_uvs, int* out_tris, int* out_uvs, int* out_norms) {

	int* tris  = malloc(sizeof(int)*3*num_tris);
	int* uvs   = (num_uvs > 0) ? malloc(sizeof(int)*3*num_uvs) : NULL;
	int* norms = (num_norms > 0) ? malloc(sizeof(int)*3*num_norms) : NULL;

	char buf[256] = {0};
	
	int tri_idx = 0;
	while( (fgets(buf, sizeof(buf), fp) != NULL) )
	{
		parse_line(buf,tris,num_tris,uvs,num_uvs,norms,num_norms,tri_idx);
		if(buf[0] == 'f') tri_idx++;
	}

	out_tris  = tris;
	out_uvs   = uvs;
	out_norms = norms;

	rewind(fp);

}

