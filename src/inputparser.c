#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * Expects *.input files with the following format: * "{x0,y0},{x1,y1}" */
/* please supply a ptr to and int[2] for start and end to store the result */


int extract_num_coords(FILE* fp) {
	if(fp == NULL) {perror("extract_num_coords:fp is null"); return -1;}

	char buf[256] = {0};
	fgets(buf, sizeof(buf), fp);

	return atoi(buf);
}


// convert "{x0,y0} {x1,y1}" into int and store into coords as
// coords[i] = x0 coords[i+1] = y0 coords[i+2] = x1 coords[i+3] = y1
void parse_coords(FILE* fp, int* coords, int num_coords) {
	char buf[256] = {0};
	int coord_index = 0;
	while( (fgets(buf, sizeof(buf), fp)) && coord_index < num_coords) {
		const char * delim = "{}";
		char* start_coords = strtok(buf,delim);
		strtok(NULL,delim);
		char* end_coords = strtok(NULL, delim);

		// start_coords & end_coords of the form int,int
		int x0 = atoi(strtok(start_coords, ","));
		int y0 = atoi(strtok(NULL, ","));
		int x1 = atoi(strtok(end_coords, ","));
		int y1 = atoi(strtok(NULL, ","));
		
		printf("coord_index = %d\n", coord_index);
		coords[4*coord_index] = x0; printf("value at %p is %d\n", &(coords[4*coord_index]), x0);
		coords[4*coord_index+1] = y0; printf("value at %p is %d\n", &(coords[4*coord_index+1]), y0);
		coords[4*coord_index+2] = x1; printf("value at %p is %d\n", &(coords[4*coord_index+2]), x1);
		coords[4*coord_index+3] = y1; printf("value at %p is %d\n", &(coords[4*coord_index+3]), y1);

		coord_index++;
		memset(buf, 0x0, sizeof(buf));
	}
}

void free_coords_array(int** coords, int num_coords){
	if(coords == NULL){
		perror("coords is null");
		return;
	}
	for(int i = 0; i < num_coords; i++){
		free(coords[i]);
	}
	free(coords);
}


int parse_line(char* line, int* start, int* end) {
	return 0;
}

FILE* open_input_file(char* filename) { 
	FILE* fp = fopen(filename, "r");
	if(fp == NULL) {
		perror("error opening file");
		return NULL;	
	}
	return fp;
}

void close_input_file(FILE* fp) {
	fclose(fp);
}


