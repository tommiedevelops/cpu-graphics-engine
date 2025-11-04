#include <stdlib.h>
#include <string.h>

#include "material.h"
#include "clip.h"
#include "triangle.h"
#include "vector.h"
#include "transformation.h"
#include "scene_manager.h"
#include "barycentric.h"
#include "vert_shader.h"
#include "frag_shader.h"
#include "framebuffer.h"
#include "render.h"

typedef struct Renderer { 
	VSUniforms* vs_u;
	FSUniforms* fs_u;
	Pipeline* p;
	FrameBuffer* fb;
} Renderer;

Renderer* renderer_init(Pipeline* p, FrameBuffer* fb) {
	Renderer* r = malloc(sizeof(Renderer));
	VSUniforms* vs_u = malloc(sizeof(VSUniforms));
	FSUniforms* fs_u = malloc(sizeof(FSUniforms));
	r->p = p;	
	r->vs_u = vs_u;
	r->fs_u = fs_u;
	r->fb = fb;
	return r;
}

void renderer_uninit(Renderer* r) {
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

static inline bool inside_triangle(BaryCoords b){
	return (b.alpha > 0) && (b.beta > 0) && (b.gamma > 0) && (b.alpha <= 1) && (b.beta <= 1) && (b.gamma <= 1);
}

static bool rasterize_pixel(int x, int y, const Triangle* tri, FSin* out) {

	VSout* const* v = tri->v;

	Vec2f A = (Vec2f){v[0]->pos.x, v[0]->pos.y};
	Vec2f B = (Vec2f){v[1]->pos.x, v[1]->pos.y};
	Vec2f C = (Vec2f){v[2]->pos.x, v[2]->pos.y};
	Vec2f P = (Vec2f){ x, y };

	BaryCoords b = cartesian_to_bary(A,B,C,P);
	if(!inside_triangle(b)) return false;

	// Depth (Screen Space Z)
	const float depth = bary_mix1(b, v[0]->pos.z, v[1]->pos.z, v[2]->pos.z);	

	// Perspective Correct Attributes
	const float w_inv = bary_mix1(b, v[0]->w_inv, v[1]->w_inv, v[2]->w_inv);
	const Vec2f uv_over_w = bary_mix2(b, v[0]->uv_over_w, v[1]->uv_over_w, v[2]->uv_over_w);
	const Vec2f uv = vec2f_scale(uv_over_w, 1.0f/w_inv);

	// Other linearly interpolated values
	Vec3f normal = vec3f_normalize(bary_mix3(b,v[0]->normal, v[1]->normal, v[2]->normal));
	Vec3f world_pos  = bary_mix3(b, v[0]->world_pos, v[1]->world_pos, v[2]->world_pos);

	*out = (FSin) {
		.world_pos = world_pos,
		.normal   = normal,
		.uv       = uv,	
		.depth    = depth
	};

	return true;
}

static inline int max_i(int a, int b) {return a > b ? a : b; }
static inline int min_i(int a, int b) {return a < b ? a : b; }

static void rasterize_triangle(Renderer* r, Triangle* tri, FragShaderF frag_shader) {
	
	FSin  fs_in  = (FSin){0}; 
	FSout fs_out = (FSout){0};
	
	FrameBuffer* fb = r->fb;
	Bounds b = tri_get_bounds(tri);
	
	int xmin = max_i(0, (int)ceilf(b.xmin));
	int ymin = max_i(0, (int)ceilf(b.ymin));
	int xmax = min_i(fb->width - 1, (int)floorf(b.xmax));
	int ymax = min_i(fb->height - 1, (int)floorf(b.ymax)); 

	if(xmin > xmax || ymin > ymax) return;
	for(int y = ymin; y <= ymax; y++){
	    for(int x = xmin; x <= xmax; x++){
                  if( !rasterize_pixel(x,y,tri, &fs_in) ) continue;
		  frag_shader(&fs_in, &fs_out, r->fs_u);
		  frame_buffer_draw_pixel(fb,x,y,vec4f_to_rgba32(fs_out.color),fs_out.depth);
	    }
	}
	
}

static void process_clip_and_rasterize(Renderer* r, Triangle clip_result[6], size_t num_tris, FragShaderF fs){
	Mat4 viewport = r->vs_u->viewport;

	for(int k = 0; k < num_tris; k++){
		Triangle* tri = &clip_result[k];
		tri_apply_perspective_divide(tri); // divide (x,y,z,w) by w
		tri_apply_transformation(viewport, tri);
		rasterize_triangle(r, tri, fs);
	}
}

static inline void apply_vertex_shader(const VSin in[3], VSout out[3], const VSUniforms* vs_u, VertShaderF vertex_shader){
	for(size_t i = 0; i < 3; i++) vertex_shader(&in[i], &out[i], vs_u);
}

static void assemble_triangle_inputs(Mesh* mesh, size_t tri_idx, VSin in[3]) {

	printf("tri_idx = %ld\n", tri_idx);

	for(int i = 0; i < 3; i++){
		const int* tri_uvs   = mesh->triangle_uvs;
		const int* tri_norms = mesh->triangle_normals;

		size_t pos_idx  = mesh->triangles[tri_idx + i];
		size_t uv_idx   = tri_uvs ? mesh->triangle_uvs[tri_idx + i] : 0;
		size_t n_idx    = tri_norms ? mesh->triangle_normals[tri_idx + i] : 0;

		Vec3f pos = (pos_idx > 0) ? mesh->vertices[pos_idx] : VEC3F_0;
		Vec2f uv  = (mesh->uvs && tri_uvs)       ? mesh->uvs[uv_idx] : VEC2F_0;
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

void renderer_draw_triangle(Renderer* r, Mesh* mesh, Material* mat, size_t tri_idx) {

	VSin   in[3] = {0};
	VSout out[3] = {0};

	const Pipeline* mat_p = material_get_pipeline(mat);
	const Pipeline* p     = mat_p ? mat_p : r->p; 

	const VertShaderF vert_shader = p->vs;
	const FragShaderF frag_shader = p->fs;
	const VSUniforms* vs_u        = r->vs_u;

	assemble_triangle_inputs(mesh, tri_idx, in);
	apply_vertex_shader(in, out, vs_u, vert_shader);

	Triangle tri = {0};
	assemble_triangle(&tri, out, tri_idx);

	Triangle clip_result[6] = {0};
	int num_tris = clip_tri(&tri, clip_result);

	process_clip_and_rasterize(r, clip_result, num_tris, frag_shader);
}

static void renderer_draw_game_object(Renderer* r, GameObject* go) {

	size_t num_triangles = go->mesh->num_triangles;
	for(size_t t = 0; t < num_triangles; t++) {
		const size_t tri_idx = 3*t;
		renderer_draw_triangle(r,go->mesh,go->material,tri_idx);
	}	
}

static void prepare_per_scene_uniforms(Renderer* r, Scene* scene) {

	const int width    = r->fb->width;
	const int height   = r->fb->height;
	const float aspect = (float)height/(float)width;

	const Camera* cam = scene_get_camera(scene);
	const Transform* tr = &cam->transform;

	const Mat4 view = get_view_matrix(tr->position, tr->rotation, tr->scale);
	const Mat4 proj  = get_projection_matrix(cam->fov, cam->near, cam->far, aspect);
	const Mat4 viewport = get_viewport_matrix(cam->near, cam->far, width, height);

	r->vs_u->view       = view;
	r->vs_u->proj       = proj;
	r->vs_u->viewport   = viewport;

	r->fs_u->lights           = scene_get_lighting(scene)->lights;
	r->fs_u->num_lights       = scene_get_lighting(scene)->len;
	r->fs_u->cam_world_pos    = scene_get_camera(scene)->transform.position;
}

static void prepare_per_game_object_uniforms(Renderer* r, GameObject* go) {

	const Transform* tr = &go->transform;
	Mat4 model = get_model_matrix(tr->position, tr->rotation, tr->scale);
	r->vs_u->model      = model;
	r->fs_u->base_color = material_get_base_color(go->material);
	r->fs_u->tex        = material_get_texture(go->material);
}

void renderer_draw_scene(Renderer* r, Scene* scene) {

	if(!r || !r->fb || !scene || !scene_get_camera(scene)) return;
	
	prepare_per_scene_uniforms(r,scene);

	const size_t count = scene_get_num_gos(scene);

	for(size_t i = 0; i < count; i++) {
		GameObject* go = scene_get_game_object(scene, i);
		if(!go || !go->mesh || !go->material) continue;
		prepare_per_game_object_uniforms(r,go);
		renderer_draw_game_object(r, go);
	}

}

