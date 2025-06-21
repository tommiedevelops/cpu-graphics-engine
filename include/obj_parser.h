#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "bounds.h"
	int parse_num_edges(FILE* fp);
	int parse_num_vertices(FILE* fp);
	int* parse_obj_to_2D_coord_array(char* file_path);
	struct Bounds get_bounds(struct Vertex* vertices, int num_vertices);
#endif
