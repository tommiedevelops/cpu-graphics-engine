#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
	int parse_num_vertices(FILE* fp);
	void parse_obj(char* file_path);
	float* get_bounds(float* vertices, int num_vertices);
	void shift_to_origin(float* bounds, float* vertices, int num_vertices);
#endif
