#ifndef RENDER_H
#define RENDER_H

typedef struct Triangle Triangle;
typedef struct Scene Scene;
typedef struct Pipeline Pipeline;
typedef struct FrameBuffer FrameBuffer;

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer); 
int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(FrameBuffer* fb, Triangle* triangles, int num_triangles);
void render_scene(FrameBuffer* fb, Scene* scene, Pipeline* pl);

#endif
