#ifndef RENDER_H
#define RENDER_H

typedef struct Triangle Triangle;
typedef struct Scene Scene;
typedef struct Pipeline Pipeline;
typedef struct FrameBuffer FrameBuffer;
typedef struct Renderer Renderer;

Renderer* renderer_init(Pipeline* pl, FrameBuffer* fb);
void renderer_uninit(Renderer* r);
void render_wireframe(FrameBuffer* fb, Triangle* triangles, int num_triangles);
void renderer_draw_scene(Renderer* r, Scene* scene);

#endif
