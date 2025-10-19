#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
typedef struct Vec3f Vec3f;
typedef struct Vec2f Vec2f;

FILE* obj_open(char* filename);
void obj_close(FILE* fp);
int obj_parse_num_vertices(FILE* fp);
int obj_parse_num_uvs(FILE* fp);
Vec3f* obj_parse_vertices(FILE* fp, int num_vertices);
Vec2f* obj_parse_uvs(FILE* fp, int num_uvs);
int obj_parse_num_triangles(FILE* fp);
int* obj_parse_triangle_uvs(FILE* fp, int num_triangles, int num_uvs, Vec2f* uvs);
int* obj_parse_triangles(FILE* fp, int num_triangles, int num_vertices, Vec3f* vertices);
#endif
