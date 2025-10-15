#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"
#include "shading.h"
#include "vector.h" 
#include "lighting.h"
#include "scene_manager.h"

struct Triangle {

	// Vertex 0
	Vec4f v0;
	Vec2f uv0;
	Vec3f n0;
	Vec2f uv0_over_w;
	float w0_inv;

	// Vertex 1
	Vec4f v1;
	Vec2f uv1;
	Vec3f n1;
	Vec2f uv1_over_w;
	float w1_inv;

	// Vertex 2
	Vec4f v2;
	Vec2f uv2;
	Vec3f n2;
	Vec2f uv2_over_w;	
	float w2_inv;
};

struct Bounds get_bounds_from_tri(struct Triangle tri);

Vec3f calculate_normal(struct Triangle tri);
struct Triangle tri_perspective_divide(struct Triangle tri); 

// array size 3 of Vec3f ptrs
void rasterize_triangle(struct Triangle tri, Camera* cam, struct LightSource* ls, struct Material* mat, uint32_t* framebuffer, float* zbuffer);
#endif
