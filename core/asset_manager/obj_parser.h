#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
typedef struct Vec3f Vec3f;
typedef struct Vec2f Vec2f;
typedef struct MeshData MeshData;

FILE*  obj_open(char* filename);
void   obj_close(FILE* fp);
int    obj_parse_num_vertices(FILE* fp);
int    obj_parse_num_uvs(FILE* fp);
Vec3f* obj_parse_vertices(FILE* fp, int num_vertices);
Vec2f* obj_parse_uvs(FILE* fp, int num_uvs);
Vec3f* obj_parse_normals(FILE* fp, int num_normals);
int    obj_parse_num_normals(FILE* fp);
int    obj_parse_num_triangles(FILE* fp);
void   obj_parse_triangles(FILE* fp, int n_tris, int n_verts, int n_norms, int n_uvs, MeshData* data);
#endif
