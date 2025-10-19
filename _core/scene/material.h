#ifndef MATERIAL_H
#define MATERIAL_H
typedef struct Vec4f Vec4f;
typedef struct Vec2f Vec2f;
typedef struct Material Material;
typedef struct Texture Texture;

struct Material* material_default(); 
Material* material_create(Vec4f color, struct Texture* tex);
Vec4f material_get_albedo(struct Material* mat, Vec2f uv);

#endif
