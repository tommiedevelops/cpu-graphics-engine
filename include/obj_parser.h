#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
	int parse_num_edges(FILE* fp);
	int parse_num_vertices(FILE* fp);
	void scale_lengths(float target_length, float* bounds, float* vertices, int num_vertices);
	void normalize_lengths(float* bounds, float* vertices, int num_vertices);
	int* parse_obj_to_2D_coord_array(char* file_path);
	float* get_bounds(float* vertices, int num_vertices);
	void shift_to_origin(float* bounds, float* vertices, int num_vertices);
#endif
