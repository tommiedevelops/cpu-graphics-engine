#ifndef VERT_SHADER_H
#define VERT_SHADER_H

typedef struct VSin {
	// model space
	Vec4f pos;
	Vec3f n;
	Vec2f uv;
} VSin;

typedef struct VSout {
	Vec4f clip_pos;
	Vec3f view_pos; // view space for ligthing
	Vec3f normal;
	Vec2f uv_over_w;
	float w_inv;
} VSout;

typedef struct VertShader {
	VSin* in;
	VSout* out;
	void (*shade)(const VSin* in, VSout* out, const void* uniforms);
} VertShader;

#endif
