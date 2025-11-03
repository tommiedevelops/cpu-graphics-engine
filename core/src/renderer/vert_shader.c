#include "renderer/vert_shader.h"

void vs_default(const VSin* in, VSout* out, const VSUniforms* u) {						
	Mat4 mvp = mat4_mul_mat4(u->proj, mat4_mul_mat4(u->view, u->model));

	Vec4f pos = vec3f_to_vec4f(in->pos, 1.0f);

	Vec4f clip_pos = mat4_mul_vec4(mvp, pos);
	out->pos = clip_pos;
	out->world_pos = vec4f_to_vec3f(mat4_mul_vec4(u->model, pos));
	Vec4f norm = vec3f_to_vec4f(in->n, 0.0f); // 0f in 4th element represents directional vecs
	out->normal = vec4f_to_vec3f(mat4_mul_vec4(u->model, norm)); // convert norm to world pos

	float w_inv = (float)1.0f/clip_pos.w;
	out->uv_over_w = vec2f_scale(in->uv, w_inv);
	out->w_inv = w_inv;
}
