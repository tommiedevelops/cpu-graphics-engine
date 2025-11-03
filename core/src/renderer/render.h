#ifndef RENDER_H
#define RENDER_H

#include "renderer/vert_shader.h"
#include "renderer/frag_shader.h"

typedef struct Triangle Triangle;
typedef struct Scene Scene;
typedef struct FrameBuffer FrameBuffer;
typedef struct Renderer Renderer;

typedef struct Pipeline {
	VertShaderF vs;
	FragShaderF fs;
} Pipeline;

Pipeline* pipeline_create(VertShaderF vs, FragShaderF fs);
Renderer* renderer_init(Pipeline* default_pl);
void renderer_uninit(Renderer* r);
void render_wireframe(FrameBuffer* fb, Triangle* triangles, int num_triangles);
void renderer_draw_scene(Renderer* r, FrameBuffer* fb ,Scene* scene);

#endif
