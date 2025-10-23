#ifndef MATERIAL_H
#define MATERIAL_H
typedef struct Vec4f Vec4f;
typedef struct Vec2f Vec2f;
typedef struct Material Material;
typedef struct Texture Texture;
typedef struct Pipeline Pipeline;

struct Material* material_default(); 
Material* material_create(Vec4f color, Texture* tex);
Vec4f material_get_base_color(Material* mat);
Texture* material_get_texture(Material* mat);
Vec4f material_get_albedo(Material* mat, Vec2f uv);
Pipeline* material_get_pipeline(Material* mat);

#endif
