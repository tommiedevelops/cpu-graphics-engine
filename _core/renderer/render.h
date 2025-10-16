#ifndef RENDER_H
#define RENDER_H

typedef struct Triangle Triangle;
typedef struct Scene Scene;

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer); 
int render_lines(uint32_t* framebuffer, int* coords, int num_coords);
void render_wireframe(uint32_t* framebuffer, Triangle* triangles, int num_triangles);
void render_scene(uint32_t* framebuffer, float* zbuffer, Scene* scene);

#endif
