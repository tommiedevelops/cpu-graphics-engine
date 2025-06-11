#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * Expects *.input files with the following format: * "{x0,y0},{x1,y1}" */
/* please supply a ptr to and int[2] for start and end to store the result */


int parse_line(char* line, int* start, int* end);


/* first line should be the number of coordinates */
int parse_file(char* filename, int** res_start_coords,int** res_end_coords, int* num_coords_ptr) {
	if(filename == NULL) {
		perror("filename input not valid");
		return -1;
	}

	if(num_coords_ptr == NULL){
		perror("num_coords_ptr is null");
		return -1;
	}

	FILE *fp = fopen(filename, "r");
	if(fp == NULL){ perror("error opening the file"); return -1;}
	char buf[256] = {0};
	
	// parse the first line
	fgets(buf, sizeof(buf), fp);
	for(int i = 0; i < sizeof(buf); i++) {if(buf[i] == '\n') buf[i] = '\0';}
	int num_coords = atoi(buf);
	*num_coords_ptr = num_coords;
	
	// allocate memory for coordinates
	res_start_coords = malloc(sizeof(int*)*num_coords); //NEEDS FREEING
	res_end_coords = malloc(sizeof(int*)*num_coords);
	
	if( (res_start_coords == NULL) || (res_end_coords == NULL)) {
		perror("malloc failure");
		return -1;
	}
	
	memset(buf, 0x0, sizeof(buf));
	
	int coord_index = 0;
	while(fgets(buf, sizeof(buf), fp)) {
		int* start_coord = malloc(sizeof(int)*2); // NEEDS FREEING
		int* end_coord = malloc(sizeof(int)*2);

		parse_line(buf, start_coord, end_coord);
		
		res_start_coords[coord_index] = start_coord;
		res_end_coords[coord_index] = end_coord;		

		memset(buf, 0x0, sizeof(buf));		
		coord_index++;
	}
	
	fclose(fp);
	return 0;
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
	if(line == NULL){perror("line is null"); return -1;}
	if(start == NULL){perror("start is null"); return -1;}
	if(end == NULL){perror("end is null"); return -1;}

	char *token = strtok(line, "{}");
	int coord_index = 0;
	while (token != NULL) {
		int x,y;
		if(coord_index == 0) {
			if (sscanf(token, "%d,%d", start, start+1) == 2) {
				printf("parsed start coords (%d,%d)\n", *(start),*(start+1));
			}
		} else {
			if (sscanf(token, "%d,%d", end, end+1) == 2) {
				printf("parsed end coords (%d,%d)\n", *(end),*(end+1));
			}
		}
		token = strtok(NULL, "{}");
		coord_index++;
	}
	return 0;
}

