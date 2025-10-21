#include <stdlib.h>
#include <string.h>

#include "material.h"
#include "clip.h"
#include "triangle.h"
#include "vector.h"
#include "transformation.h"
#include "scene_manager.h"

#include "rasterize.h"
#include "pipeline.h"
#include "framebuffer.h"

typedef struct Renderer { 
	VSUniforms* vs_u;
	FSUniforms* fs_u;
	FrameBuffer* fb;
} Renderer;

Renderer* renderer_init(FrameBuffer* fb) {

	if(NULL == fb) {
		printf("Framebuffer is requried\n");
		return NULL;
	}

	Renderer* r = malloc(sizeof(Renderer));
	r->fb = fb;

	return r;
}

void renderer_uninit(Renderer* r){
	// pipeline and framebuffer life cycle not managed by renderer
	free(r);
}

static inline bool inside_triangle(BaryCoords b){
	return (b.alpha > 0) && (b.beta > 0) && (b.gamma > 0) && (b.alpha <= 1) && (b.beta <= 1) && (b.gamma <= 1);
}

static Frag rasterize_pixel(int x, int y, Triangle* tri) {

	Vec2f P = vec2f_create(x,y);
	BaryCoords b = cartesian_to_bary(A,B,C,P);

	if(!inside_triangle(b)) return;

	/* INTERPOLATING VALUES */ 
	float depth     = bary_interpolate_float(b, v[0].pos.z, v[1].pos.z, v[2].pos.z);	
	Vec2f uv_over_w = bary_interpolate_vec2f(b, v[0].uv_over_w, v[1].uv_over_w, v[2].uv_over_w);
	Vec3f frag_pos  = bary_inerpolate_vec3f(b, v[0].frag_pos, v[1].frag_pos, v[2].frag_pos);
	float w_inv     = bary_interpolate_float(b, v[0].w_inv, v[1].w_inv, v[2].w_inv);
	Vec2f uv        = vec2f_scale(uv_over_w, 1.0f/w_inv);
	Vec3f n         = bary_interpolate_vec3f(b,v[0].n, v[1].n, v[2].n);

	Frag frag = {
		.frag_pos = frag_pos,
		.normal = n,
		.uv = uv,
		.depth = depth
	};

	return frag;
}

static void rasterize_triangle(Renderer* r, Triangle* tri, FragShaderF fs) {

	Vertex (*v)[3] = tri->v;

	FSin fs_in  = {0}; 
	FSout fs_out = {0};

	Vec2f A = vec2f_create(v[0].pos.x, v[0].pos.y);
	Vec2f B = vec2f_create(v[1].pos.x, v[1].pos.y);
	Vec2f C = vec2f_create(v[2].pos.x, v[2].pos.y);

	Bounds bounds = tri_get_bounds(tri);
	int width = (int)(bounds.xmax - bounds.xmin);
	int height = (int)(bounds.ymax - bounds.ymin);	

	for(int i = 0; i < width * height; ++i) {
		int x = (int)bounds.xmin + (i % width);
		int y = (int)bounds.ymin + (i / width);

		Fragment frag = rasterize_pixel(x,y,tri);	
		fs(&frag, &fs_out, &(r->fs_u));

		frame_buffer_draw_pixel(r->fb, x, y, fs_out.color, fs_out.depth);
	}
}

void renderer_draw_triangle(Renderer* r, Mesh* mesh, Material* mat, size_t tri_idx) {

	VSin in[3]; Vertex out[3];
	VertShaderF vs = mat->pipeline->vs;
	FragShaderF fs = mat->pipeline->fs;

	Triangle clip_result[6] = {0};

	for(int i = 0; i < 3; i++){
		size_t pos_idx  = mesh->triangles[tri_idx + i];
		size_t uv_idx   = mesh->triangle_uvs[tri_idx + i];
		size_t n_idx    = mesh->triangle_normals[tri_idx + i];

		Vec3f pos = mesh->vertices[pos_idx];
		Vec2f uv  = mesh->uvs ? mesh->uvs[uv_idx] : VEC2F_0;
		Vec3f n   = mesh->normals ? mesh->normals[n_idx] : VEC3F_0;

		vs_assemble_input(&in[i], pos, uv, n);
		vs(&in[i], &out[i], &(r->vs_u));
	}

	Triangle tri = {.v = out};

	int num_tris = clip_tri(&tri, clip_result); //TODO

	for(int k = 0; k < num_tris; k++){
		tri_apply_perspective_divide(&clip_result[k]); // divide (x,y,z,w) by w
		tri_apply_transformation(view_port, &clip_result[k]);
		rasterize_triangle(r, &clip_result[k], fs);
	}

}

void renderer_draw_mesh(Renderer* r, Mesh* mesh, Material* mat) {
	for(size_t t = 0; t < mesh->num_triangles; t++) {
		size_t tri_idx = 3*t;
		renderer_draw_triangle(r,mesh,mat,tri_idx);
	}	
}

void renderer_draw_scene(Renderer* r, Scene* scene) {

	int width  = r->fb->width;
	int height = r->fb->height;
	float aspect = (float)height/width;
	int  num_gos = scene_get_num_gos(scene);

	Transform cam_tr     = scene->cam->transform;
	Mat4      view       = get_view_matrix(cam_tr.position, cam_tr.rotation, cam_tr.scale);
	Mat4      proj       = get_projection_matrix(cam->fov, cam->near, cam->far, aspect);
	Mat4      view_port  = get_viewport_matrix(cam->near, cam->far, width, height);

	for(int i = 0; i < num_gos; i++) {
		GameObject* go    = scene_get_game_object(scene, i);
		Transform   tr    = go->transform;
		Mat4        model = get_model_matrix(tr.position, tr.rotation, tr.scale);

		vs_assemble_uniforms(&r.vs_u, model, view, proj, view_port); //TODO
		fs_assemble_uniforms(&r.fs_u, scene->lighting, go->material);  //TODO

		renderer_draw_mesh(r, go->mesh, go->mat);
	}
}

