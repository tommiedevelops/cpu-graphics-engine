#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "bounds.h"
	int parse_num_edges(FILE* fp);
	int parse_num_vertices(FILE* fp);
	void scale_lengths(float target_length, struct Bounds bounds, struct Vertex* vertices, int num_vertices);
	void normalize_lengths(struct Bounds bounds, struct Vertex* vertices, int num_vertices);
	int* parse_obj_to_2D_coord_array(char* file_path);
	struct Bounds get_bounds(struct Vertex* vertices, int num_vertices);
	void shift_to_origin(struct Bounds bounds, struct Vertex* vertices, int num_vertices);
#endif
