#ifndef RENDER_H
#define RENDER_H

#include "vert_shader.h"
#include "frag_shader.h"

typedef struct Triangle Triangle;
typedef struct Scene Scene;
typedef struct FrameBuffer FrameBuffer;
typedef struct Renderer Renderer;
typedef struct Pipeline Pipeline;

Pipeline* pipeline_create(VertShaderF vs, FragShaderF fs);
Renderer* renderer_init(Pipeline* pl, FrameBuffer* fb);
void renderer_uninit(Renderer* r);
void render_wireframe(FrameBuffer* fb, Triangle* triangles, int num_triangles);
void renderer_draw_scene(Renderer* r, Scene* scene);

#endif
