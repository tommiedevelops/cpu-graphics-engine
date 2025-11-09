#ifndef RASTERIZE_H
#define RASTERIZE_H

typedef struct Renderer Renderer;
typedef struct FrameBuffer FrameBuffer;
typedef struct Triangle Triangle;
typedef FSin FSin;
typedef FragShaderF FragShaderF;

bool rasterize_pixel(int x, int y, const Triangle* tri, FSin* out);
void rasterize_triangle(Renderer* r, FrameBuffer* fb, Triangle* tri, FragShaderF frag_shader);

#endif
