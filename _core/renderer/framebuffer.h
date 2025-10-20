#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

typedef struct FrameBuffer {
	uint32_t* framebuffer;
	float* zbuffer;
	int width, height;
} FrameBuffer;

void frame_buffer_clear(uint32_t clear_color);
void frame_buffer_draw_pixel(int x, int y, uint32_t color);

#endif
