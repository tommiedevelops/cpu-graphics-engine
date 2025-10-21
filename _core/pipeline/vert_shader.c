#include "vert_shader.h"

VertShader* vert_shader_default() {
	VertShader* vs = malloc(sizeof(VertShader));
	
	return vs;
}

void vert_shader_destroy(VertShader* vs) {
	free(vs);
}

void vs_default(const VSin* in, VSout* out, VSUniforms* u) {						
	Mat4 mv = mat4_mul_mat4(u->view, u->model);
	Mat4 mvp = mat4_mul_mat4(u->proj, mv);

	Vec4f clip_pos = mat4_mul_vec4(mvp, in->pos);
	out->clip_pos = clip_pos;
	out->view_pos = vec4f_to_vec3f(mat4_mul_vec4(mv, in->pos));
	out->normal = in->n;

	float w_inv = (float)1.0f/clip_pos.w;
	out->uv_over_w = vec2f_scale(in->uv, w_inv);
	out->w_inv = w_inv;
}
