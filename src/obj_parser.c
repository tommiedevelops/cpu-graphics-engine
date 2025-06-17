#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

float** parse_vertices(FILE* fp){
	char buf[256] = {0};
	while(NULL != fgets(buf, sizeof(buf), fp) ) {
		if(buf[0] = 'v'){
			printf("%s\n",buf);
		}
	}

}


void parse_obj(char* filename){
	FILE* fp = open_obj(filename);
	parse_vertices(fp);
	close_obj(fp);
}
