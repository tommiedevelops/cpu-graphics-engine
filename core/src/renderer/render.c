#include <stdlib.h>
#include <string.h>

#include "game_math/vector.h"
#include "game_math/transformation.h"
#include "game_math/barycentric.h"
#include "game_math/bounds.h"

#include "asset_manager/material.h"
#include "asset_manager/mesh.h"

#include "clip.h"
#include "triangle.h"
#include "vert_shader.h"
#include "frag_shader.h"
#include "framebuffer.h"
#include "render.h"
#include "rasterize.h"

#include "scene_manager/game_object.h"
#include "scene_manager/camera.h"
#include "scene_manager/scene.h"
#include "scene_manager/transform.h"

Renderer* renderer_create(Pipeline* default_pl) {
	Renderer* r = malloc(sizeof(Renderer));
	VSUniforms* vs_u = malloc(sizeof(VSUniforms));
	FSUniforms* fs_u = malloc(sizeof(FSUniforms));
	r->p = default_pl;	
	r->vs_u = vs_u;
	r->fs_u = fs_u;
	return r;
}

void renderer_destroy(Renderer* r) {
	if(!r) return;
	free(r->vs_u);
	free(r->fs_u);
	free(r);
}

Pipeline* pipeline_create(VertShaderF vert_shader, FragShaderF frag_shader) {
	Pipeline* p = malloc(sizeof(Pipeline));
	p->vs = vert_shader;
	p->fs = frag_shader;
	return p;
}

void pipeline_destroy(Pipeline* p) {
	free(p);
}

static void process_clip_and_rasterize(Renderer* r, FrameBuffer* fb, Triangle clip_result[6], size_t num_tris, FragShaderF fs){
	Mat4 viewport = r->vs_u->viewport;

	for(int k = 0; k < num_tris; k++){
		Triangle* tri = &clip_result[k];
		tri_apply_perspective_divide(tri); // divide (x,y,z,w) by w
		tri_apply_transformation(viewport, tri);
		rasterize_triangle(r, fb, tri, fs);
	}
}

static inline void apply_vertex_shader(const VSin in[3], VSout out[3], const VSUniforms* vs_u, VertShaderF vertex_shader){
	for(size_t i = 0; i < 3; i++) vertex_shader(&in[i], &out[i], vs_u);
}

static void assemble_triangle_inputs(Mesh* mesh, size_t tri_idx, VSin in[3]) {

	for(int i = 0; i < 3; i++){

		const int* tri_uvs = mesh->triangle_uvs;
		const int* tri_norms = mesh->triangle_normals;

		size_t pos_idx  = mesh->triangles[tri_idx + i];
		size_t uv_idx   = tri_uvs ? mesh->triangle_uvs[tri_idx + i] : 0;
		size_t n_idx    = tri_norms ? mesh->triangle_normals[tri_idx + i] : 0;

		Vec3f pos = mesh->vertices[pos_idx];
		Vec2f uv  = (mesh->uvs && tri_uvs) ? mesh->uvs[uv_idx] : VEC2F_0;
		Vec3f n   = (mesh->normals && tri_norms) ? mesh->normals[n_idx] : VEC3F_0;
		
		in[i].pos = pos;
		in[i].n = n;
		in[i].uv = uv;
	}
}

static void assemble_triangle(Triangle* tri, VSout* out, int tri_idx) {
	for(size_t i = 0; i <= 2; i++) tri->v[i] = &out[i];
	tri->id = tri_idx;	
}

static void renderer_draw_triangle(Renderer* r, FrameBuffer* fb, Mesh* mesh, Material* mat, size_t tri_idx) {

	Triangle clip_result[6];
	Triangle tri;
	VSin     in[3];
	VSout    out[3];

	const Pipeline* mat_p = material_get_pipeline(mat);
	const Pipeline* p     = mat_p ? mat_p : r->p; 

	assemble_triangle_inputs(mesh, tri_idx, in);
	apply_vertex_shader(in, out, r->vs_u, p->vs);

	assemble_triangle(&tri, out, tri_idx);
	int num_tris = clip_tri(&tri, clip_result);
	process_clip_and_rasterize(r,fb,clip_result, num_tris, p->fs);
}

static void renderer_draw_game_object(Renderer* r, FrameBuffer* fb, GameObj* go) {

	size_t num_triangles = go->mesh->num_triangles;
	for(size_t t = 0; t < num_triangles; t++) {
		const size_t tri_idx = 3*t;
		renderer_draw_triangle(r,fb,go->mesh,go->mat,tri_idx);
	}	
}

static void prepare_per_scene_uniforms(Renderer* r, Scene* scene) {

	const Camera* cam = scene_get_camera(scene);
	const Transform* tr  = camera_get_transform(cam);
	float near = camera_get_near(cam); 
	float far  = camera_get_far(cam);
	float fov  = camera_get_fov(cam);

	const int   width    = camera_get_screen_width(cam);
	const int   height   = camera_get_screen_height(cam);
	const float aspect   = (float)height/(float)width;

	const Mat4  view     = get_view_matrix(tr->position, tr->rotation, tr->scale);
	const Mat4  proj     = get_projection_matrix(fov, near, far, aspect);
	const Mat4  viewport = get_viewport_matrix(near, far, width, height);

	r->vs_u->view       = view;
	r->vs_u->proj       = proj;
	r->vs_u->viewport   = viewport;

	r->fs_u->light            = scene_get_light(scene);
	r->fs_u->cam_world_pos    = tr->position;
}

static void prepare_per_game_object_uniforms(Renderer* r, GameObj* go) {
	const Transform* tr = go->tr;
	r->vs_u->model      = get_model_matrix(tr->position, tr->rotation, tr->scale);
	r->fs_u->base_color = material_get_base_color(go->mat);
	r->fs_u->tex        = material_get_texture(go->mat);
}

void renderer_draw_scene(Renderer* r, FrameBuffer* fb, Scene* scene) {

	if(!r || !scene || !scene_get_camera(scene)) return;
	
	prepare_per_scene_uniforms(r,scene);

	const size_t count = scene_get_num_gos(scene);
	GameObj** gos = scene_get_game_obj_arr(scene);

	for(size_t i = 0; i < count; i++) {
		GameObj* go = gos[i];
		if(!go || !go->mesh || !go->mat) continue;
		prepare_per_game_object_uniforms(r,go);
		renderer_draw_game_object(r, fb, go);
	}

}

