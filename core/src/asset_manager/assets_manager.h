#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

typedef struct Assets Assets;
typedef struct Material Material;
typedef struct Mesh Mesh;

Assets*   assets_create();
uint8_t   assets_add_material(Assets* a, Material* mat, const char* handle);
Material* assets_get_material(Assets* a, const char* handle);
Mesh*     assets_get_mesh(Assets* a, const char* handle);
uint8_t   assets_add_mesh(Assets* a, Mesh* mesh, const char* handle);
void      assets_destroy(Assets* a);

#endif
