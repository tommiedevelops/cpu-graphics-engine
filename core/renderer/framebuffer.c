#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "framebuffer.h"

FrameBuffer* frame_buffer_create(int width, int height) {

	FrameBuffer* fb = malloc(sizeof(FrameBuffer));
	uint32_t* framebuffer = malloc(sizeof(uint32_t)*width*height);
	float* zbuffer = malloc(sizeof(float)*width*height);

	fb->framebuffer = framebuffer;
	fb->zbuffer = zbuffer;
	fb->width = width;
	fb->height = height;

	return fb;
}

void frame_buffer_destroy(FrameBuffer* fb) {
	free(fb->framebuffer);
	free(fb->zbuffer);
	free(fb);
}

void frame_buffer_clear(FrameBuffer* fb, uint32_t clear_color) {
	int n = fb->width * fb->height;
	for(int i = 0; i < n; i++) {
		fb->framebuffer[i] = clear_color;
		fb->zbuffer[i] = FLT_MAX;
	}	
}

void frame_buffer_draw_pixel(FrameBuffer* fb, int x, int y, uint32_t color, float depth) {
	if(x <= 0 || x >= fb->width) return;
	if(y <= 0 || y >= fb->height) return;

	if(depth <= fb->zbuffer[x + y*fb->width]) {
		fb->framebuffer[x + y*fb->width] = color;
		fb->zbuffer[x + y*fb->width] = depth;
	}

}


