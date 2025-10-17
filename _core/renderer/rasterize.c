#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"

Bounds get_bounds_from_tri(Triangle tri){
        Bounds bounds = BOUNDS_DEFAULT;
	for(int i = 0; i <= 2; i++){
		if(tri.v[i].pos.x > bounds.xmax) {bounds.xmax = tri.v[i].pos.x;}
		if(tri.v[i].pos.y > bounds.ymax) {bounds.ymax = tri.v[i].pos.y;}
		if(tri.v[i].pos.z > bounds.zmax) {bounds.zmax = tri.v[i].pos.z;}
		if(tri.v[i].pos.x < bounds.xmin) {bounds.xmin = tri.v[i].pos.x;}
		if(tri.v[i].pos.y < bounds.ymin) {bounds.ymin = tri.v[i].pos.y;}
		if(tri.v[i].pos.z < bounds.zmin) {bounds.zmin = tri.v[i].pos.z;}

	}
        return bounds;
}

bool inside_triangle(float alpha, float beta, float gamma){
	return (alpha > 0) && (beta > 0) && (gamma > 0) && (alpha <= 1) && (beta <= 1) && (gamma <= 1);
}

float interpolate_depth(Triangle tri, float alpha, float beta, float gamma){
	// the basic idea is that each point x,y has a z value. i just need to calculate it
	float z0 = tri.v[0].pos.z;
	float z1 = tri.v[1].pos.z;
	float z2 = tri.v[2].pos.z;

	float depth = alpha*z0 + beta*z1 + gamma*z2;

	return depth;
}

Vec3f interpolate_normal(Triangle tri, float alpha, float beta, float gamma){
	float nx = alpha*tri.v[0].n.x + beta*tri.v[1].n.x + gamma*tri.v[2].n.x;
	float ny = alpha*tri.v[0].n.y + beta*tri.v[1].n.y + gamma*tri.v[2].n.y;
	float nz = alpha*tri.v[0].n.z + beta*tri.v[1].n.z + gamma*tri.v[2].n.z;

	Vec3f n = vec3f_normalize(vec3f_create(nx,ny,nz));
	return n;
}

Vec2f interpolate_uv(Triangle tri, float alpha, float beta, float gamma){
	float u0 = tri.v[0].uv_over_w.x;
	float u1 = tri.v[1].uv_over_w.x;
	float u2 = tri.v[2].uv_over_w.x;

	float v0 = tri.v[0].uv_over_w.y;
	float v1 = tri.v[1].uv_over_w.y;
	float v2 = tri.v[2].uv_over_w.y;
	
	float u = alpha*u0 + beta*u1 + gamma*u2;
	float v = alpha*v0 + beta*v1 + gamma*v2;

	float w_inv = alpha*tri.v[0].w_inv + beta*tri.v[1].w_inv + gamma*tri.v[2].w_inv;

	Vec2f uv = vec2f_scale(vec2f_create(u,v), (float)1.0f/w_inv);
	
	return uv;
}

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
static inline float compute_alpha(int x, int y, Triangle tri) {

	// calculate barycentric coords
	float alpha = (tri.v[0].pos.x*(tri.v[2].pos.y-tri.v[0].pos.y)+(y-tri.v[0].pos.y)*(tri.v[2].pos.x-tri.v[0].pos.x)-x*(tri.v[2].pos.y-tri.v[0].pos.y))
		/((tri.v[1].pos.y-tri.v[0].pos.y)*(tri.v[2].pos.x-tri.v[0].pos.x)-(tri.v[1].pos.x-tri.v[0].pos.x)*(tri.v[2].pos.y-tri.v[0].pos.y));

	return alpha;
}

static inline float compute_beta(int x, int y, Triangle tri, float alpha) {
	float beta = ((y-tri.v[0].pos.y) - alpha*(tri.v[1].pos.y-tri.v[0].pos.y))/(tri.v[2].pos.y-tri.v[0].pos.y);
	return beta;
}


void draw_pixel(int x, int y, uint32_t* framebuffer, float* zbuffer, float depth, uint32_t color){
	if(x >= WIDTH || x <= 0.0f) return;
	if(y >= HEIGHT || y <= 0.0f) return;

	if(depth <= zbuffer[x + y*WIDTH]){
		framebuffer[x + y*WIDTH] = color;			
		zbuffer[x + y*WIDTH] = depth;
	}
}

static inline Vec4f vec4f_mul(Vec4f a, Vec4f b) {
    return (Vec4f){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}


void rasterize_triangle(Triangle tri, Camera* cam, struct LightSource* ls, struct Material* mat, uint32_t* framebuffer, float* zbuffer) {

	struct Bounds bounds = get_bounds_from_tri(tri);
	for(int y = (int)bounds.ymin; y <= (int)bounds.ymax; y++){
		for(int x = (int)bounds.xmin; x <= (int)bounds.xmax; x++) {

			float alpha = compute_alpha(x,y,tri);
			float beta = compute_beta(x,y,tri, alpha);
			float gamma = 1 - alpha - beta;

			if( inside_triangle(alpha, beta, gamma) ) {
			
				// Fragment Shader	
				float depth = interpolate_depth(tri, alpha, beta, gamma);	
				Vec2f uv = interpolate_uv(tri, alpha, beta, gamma);
				Vec3f n = interpolate_normal(tri, alpha, beta, gamma);

				Vec4f diffuse = 
				compute_diffuse(material_get_albedo(mat,uv), ls->direction, ls->color, n);
				uint32_t color = vec4f_to_rgba32(diffuse);
				draw_pixel(x,y,framebuffer,zbuffer,depth,color);
			} 

		}

	}
}
