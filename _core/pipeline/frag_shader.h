#ifndef SHADING_H
#define SHADING_H

typedef struct Vec4f Vec4f;

typedef struct FSin {
	Vec3f frag_pos; // view space
	Vec3f normal; 
	Vec2f uv_over_w;
	float w_inv;
} FSin;

typedef struct FSout {
	Vec4f color;
} FSout;

typedef struct FragShader {
	FSin* in;
	void (*shade)(const FSin* in, FSout* out, const void* uniforms);
} FragShader;

#endif 
