#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "triangle.h"
#include "color.h"
#include "render.h"
#include "constants.h"

struct Triangle create_triangle(
	struct Vec3f v0,
	struct Vec3f v1,
	struct Vec3f v2 
){
	struct Triangle tri = {.v0=v0,.v1=v1,.v2=v2};
	return tri;
}

struct Vec3f calculate_normal(struct Triangle tri){
	
	// u = v1 - v0
	struct Vec3f u = vec3f_add(tri.v0, vec3f_scale(tri.v1, -1));
	// v = v2 - v0
	struct Vec3f v = vec3f_add(tri.v0, vec3f_scale(tri.v2, -1));
	
	struct Vec3f n = vec3f_normalize(vec3f_cross(u,v));
	
	return n;
}

void swap(struct Vec3f a, struct Vec3f b){
	struct Vec3f temp = a;
	a = b;
	b = temp;
}

struct Triangle sort_vertices_by_y_asc(struct Triangle tri) {
	// Mini 3-element bubble sort to order vertices by ascending y
	struct Triangle res;

	if (tri.v1.y < tri.v0.y) swap(tri.v0,tri.v1);
	if (tri.v2.y < tri.v1.y) swap(tri.v2,tri.v1);
	if (tri.v1.y < tri.v0.y) swap(tri.v0,tri.v1);

	res.v0 = tri.v0;
	res.v1 = tri.v1;
	res.v2 = tri.v2;

	return res;

}

bool inside_triangle(float alpha, float beta, float gamma){
	return (alpha > 0) && (beta > 0) && (gamma > 0) && (alpha <= 1) && (beta <= 1) && (gamma <= 1);
}

float interpolate_depth(struct Triangle tri, float alpha, float beta, float gamma){
	// the basic idea is that each point x,y has a z value. i just need to calculate it
	float z0 = tri.v0.z;
	float z1 = tri.v1.z;
	float z2 = tri.v2.z;

	float depth = alpha*z0 + beta*z1 + gamma*z2;

	return depth;
}

bool point_inside(struct Vec4f point){	
	float w = point.w;
	return (point.x >= -w) && (point.x <= w) && 
	       (point.y >= -w) && (point.y <= w); //&& 
               //(point.z >= 0) && (point.z <= w);	       
}

struct Triangle apply_perspective_projection(bool* clipped, struct Mat4 m, struct Triangle tri) {
	struct Triangle res = {0};

	struct Vec4f v4_0 = {.x = tri.v0.x, .y = tri.v0.y, .z = tri.v0.z, .w = 1.0f};
	struct Vec4f v4_1 = {.x = tri.v1.x, .y = tri.v1.y, .z = tri.v1.z, .w = 1.0f};
	struct Vec4f v4_2 = {.x = tri.v2.x, .y = tri.v2.y, .z = tri.v2.z, .w = 1.0f};
	
	v4_0 = mat4_mul_vec4(m,v4_0);
	v4_1 = mat4_mul_vec4(m,v4_1);
	v4_2 = mat4_mul_vec4(m,v4_2);

	*clipped = !point_inside(v4_0) || !point_inside(v4_1) || !point_inside(v4_2);

	v4_0 = perspective_divide(v4_0);
	v4_1 = perspective_divide(v4_1);
	v4_2 = perspective_divide(v4_2);

	struct Vec3f v3_0 = {.x = v4_0.x, .y = v4_0.y, .z = v4_0.z};
	struct Vec3f v3_1 = {.x = v4_1.x, .y = v4_1.y, .z = v4_1.z};
	struct Vec3f v3_2 = {.x = v4_2.x, .y = v4_2.y, .z = v4_2.z};

	res.v0 = v3_0;
	res.v1 = v3_1;
	res.v2 = v3_2;

	return res;
}


struct Triangle apply_transformation(struct Mat4 tr, struct Triangle tri) {
	struct Triangle res;

	struct Vec4f v4_0 = {.x = tri.v0.x, .y = tri.v0.y, .z = tri.v0.z, .w = 1.0f};
	struct Vec4f v4_1 = {.x = tri.v1.x, .y = tri.v1.y, .z = tri.v1.z, .w = 1.0f};
	struct Vec4f v4_2 = {.x = tri.v2.x, .y = tri.v2.y, .z = tri.v2.z, .w = 1.0f};

	v4_0 = mat4_mul_vec4(tr, v4_0);
	v4_1 = mat4_mul_vec4(tr, v4_1);
	v4_2 = mat4_mul_vec4(tr, v4_2);

	struct Vec3f v3_0 = {.x = v4_0.x, .y = v4_0.y, .z = v4_0.z};
	struct Vec3f v3_1 = {.x = v4_1.x, .y = v4_1.y, .z = v4_1.z};
	struct Vec3f v3_2 = {.x = v4_2.x, .y = v4_2.y, .z = v4_2.z};

	res.v0 = v3_0;
	res.v1 = v3_1;
	res.v2 = v3_2;

	return res;
}



void rasterize_triangle(struct Triangle tri_in, uint32_t* framebuffer, float* zbuffer, uint32_t color) {
	
	// Currently assuming camera fixed on z-axis and is orthographic
	struct Triangle tri = sort_vertices_by_y_asc(tri_in);

	struct Vec3f A = tri.v0;
	struct Vec3f B = tri.v1;
	struct Vec3f C = tri.v2;

	/* printf("rasterizing triangle:\n"); */
	/* print_vec3f(A); */
	/* print_vec3f(B); */
	/* print_vec3f(C); */
	
	struct Bounds bounds = get_bounds_from_tri(tri);

	int xmin = (int)bounds.xmin;
	int xmax = (int)bounds.xmax;
	int ymin = (int)bounds.ymin;
	int ymax = (int)bounds.ymax;

	for(int y = ymin; y <= ymax; y++){
		for(int x = xmin; x <= xmax; x++) {
			// check if within bounds
			if( x >= WIDTH || x <= 0) return;
			if( y >= HEIGHT || y <= 0) return;

			// calculate barycentric coords
			float alpha = (A.x*(C.y-A.y)+(y-A.y)*(C.x-A.x)-x*(C.y-A.y))
					/((B.y-A.y)*(C.x-A.x)-(B.x-A.x)*(C.y-A.y));

			float beta = ((y-A.y) - alpha*(B.y-A.y))/(C.y-A.y);
			float gamma = 1 - alpha - beta;
								
			if( inside_triangle(alpha, beta, gamma) ) {
				float depth = interpolate_depth(tri, alpha, beta, gamma);	
				
				// Normalize depth to only positive values
				depth = depth / 2 + LENGTH_SCALE;

				if(depth >= zbuffer[x + y*WIDTH]){
					place_pixel(x,y,color,framebuffer);			
					zbuffer[x + y*WIDTH] = depth;
				}
			}	

		}

	}
}

