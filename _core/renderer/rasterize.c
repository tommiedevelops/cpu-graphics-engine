#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "lighting.h"
#include "material.h"
#include "triangle.h"
#include "barycentric.h"
#include "bounds.h"
#include "framebuffer.h"
#include "frag_shader.h"

// FROM CHAT-GPT
static inline uint32_t vec4f_to_rgba32(Vec4f c) {
    // Clamp
    float r = c.x < 0 ? 0 : (c.x > 1 ? 1 : c.x);
    float g = c.y < 0 ? 0 : (c.y > 1 ? 1 : c.y);
    float b = c.z < 0 ? 0 : (c.z > 1 ? 1 : c.z);
    float a = c.w < 0 ? 0 : (c.w > 1 ? 1 : c.w);

    // Scale to 0-255
    uint8_t R = (uint8_t)(r * 255.0f + 0.5f);
    uint8_t G = (uint8_t)(g * 255.0f + 0.5f);
    uint8_t B = (uint8_t)(b * 255.0f + 0.5f);
    uint8_t A = (uint8_t)(a * 255.0f + 0.5f);

    // Pack: RGBA in memory (lowest byte = R, highest = A)
    return ((uint32_t)R << 24) |
           ((uint32_t)G << 16) |
           ((uint32_t)B << 8)  |
           ((uint32_t)A);
}

static inline bool inside_triangle(BaryCoords b){
	return (b.alpha > 0) && (b.beta > 0) && (b.gamma > 0) && (b.alpha <= 1) && (b.beta <= 1) && (b.gamma <= 1);
}

void rasterize_triangle(Triangle* tri, Lighting* lgt, Material* mat, FrameBuffer* fb) {

	Vertex v0 = tri->v[0];
	Vertex v1 = tri->v[1];
	Vertex v2 = tri->v[2];

	Vec2f A = vec2f_create(v0.pos.x, v0.pos.y);
	Vec2f B = vec2f_create(v1.pos.x, v1.pos.y);
	Vec2f C = vec2f_create(v2.pos.x, v2.pos.y);

	Bounds bounds = tri_get_bounds(tri);
	for(int y = (int)bounds.ymin; y <= (int)bounds.ymax; y++){
		for(int x = (int)bounds.xmin; x <= (int)bounds.xmax; x++) {

			Vec2f P = vec2f_create(x,y);
			BaryCoords b = cartesian_to_bary(A,B,C,P);

			if(inside_triangle(b)) {
			
				// Fragment Shader	
				float depth = bary_interpolate_float(b, v0.pos.z, v1.pos.z, v2.pos.z);	
				Vec2f uv_over_w = bary_interpolate_vec2f(b, v0.uv_over_w, v1.uv_over_w, v2.uv_over_w);
				float w_inv = bary_interpolate_float(b, v0.w_inv, v1.w_inv, v2.w_inv);
				Vec2f uv = vec2f_scale(uv_over_w, 1.0f/w_inv);
				Vec3f n = bary_interpolate_vec3f(b,v0.n, v1.n, v2.n);

				Light* light = lighting_get_light(lgt, 0); 
				Vec4f diffuse = 
				compute_diffuse(material_get_albedo(mat,uv), light->direction, light->color, n);

				frame_buffer_draw_pixel(fb, x, y, vec4f_to_rgba32(diffuse), depth);
			} 

		}

	}
}
