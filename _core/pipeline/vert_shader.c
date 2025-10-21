#include "vert_shader.h"

void vs_default(const VSin* in, VSout* out, const VSUniforms* u) {						
	Mat4 mv = mat4_mul_mat4(u->view, u->model);
	Mat4 mvp = mat4_mul_mat4(u->proj, mv);

	Vec4f pos = vec3f_to_vec4f(in->pos, 1.0f);

	Vec4f clip_pos = mat4_mul_vec4(mvp, pos);
	out->pos = clip_pos;
	out->view_pos = vec4f_to_vec3f(mat4_mul_vec4(mv, pos));
	out->normal = in->n;

	float w_inv = (float)1.0f/clip_pos.w;
	out->uv_over_w = vec2f_scale(in->uv, w_inv);
	out->w_inv = w_inv;
}
