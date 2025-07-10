#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include "vec3f.h"
#include "triangle.h"
#include "scene_manager.h"

struct Mesh parse_obj(char* filename);
#endif
