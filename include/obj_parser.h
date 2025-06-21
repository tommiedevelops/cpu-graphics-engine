#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "bounds.h"
#include "vertex.h"
	struct Vertex* parse_vertices_from_obj(char* filename);
	int parse_num_edges(char* filepath);
	int parse_num_vertices(char* filepath);
	struct Edge* parse_edges_from_obj(char* file_path, struct Vertex* vertices);
	struct Bounds get_bounds(struct Vertex* vertices, int num_vertices);
#endif
