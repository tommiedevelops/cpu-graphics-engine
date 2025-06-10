#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * Expects *.input files with the following format: * "{x0,y0},{x1,y1}" */
/* please supply a ptr to and int[2] for start and end to store the result */

int parse_input(char* filename, int* start, int* end) {

	if(filename == NULL) {
		perror("filename input not valid");
		return -1;
	}

	FILE *fp = fopen(filename, "r");

	// read the first line
	char line[256] = {0};
	fgets(line, sizeof(line), fp);

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

	if(fp == NULL) {
		perror("error opening file");
		return -1;
	}

	fclose(fp);
}

