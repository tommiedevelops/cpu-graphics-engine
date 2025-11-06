#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

typedef struct FrameBuffer {
	uint32_t* framebuffer;
	float* zbuffer;
	int width, height;
	uint32_t clear_color;
} FrameBuffer;

FrameBuffer* frame_buffer_create(int width, int height, uint32_t clear_color);
void frame_buffer_destroy(FrameBuffer* fb);
void frame_buffer_clear(FrameBuffer* fb);
void frame_buffer_draw_pixel(FrameBuffer* fb, int x, int y, uint32_t color, float depth);

#endif
