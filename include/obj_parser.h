#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "vertex.h"
#include "triangle.h"

struct ObjData {
        int num_vertices;
        int num_triangles;
        struct Vertex* vertices;
        struct Triangle* triangles;
};

struct ObjData parse_obj(char* filename);
void destroy_obj_data(struct ObjData data);
#endif
