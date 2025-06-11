#ifndef INPUTPARSER_H
#define INPUTPARSER_H
	int parse_file(char* filename, int** res_start_coords,int** res_end_coords, int* num_coords_ptr);
	int free_coords_array(int** coords, int num_coords);
#endif

