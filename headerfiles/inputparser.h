#ifndef INPUTPARSER_H
#define INPUTPARSER_H
	void parse_coords(FILE* fp, int* coords, int num_coords);
	int free_coords_array(int** coords, int num_coords);
	int extract_num_coords(FILE* fp);
	FILE* open_input_file(char* filename);
	void close_input_file(FILE* fp);
#endif

