#include <stdlib.h>
#include <string.h>

#include "shading.h"
#include "clip.h"
#include "triangle.h"
#include "vector.h"
#include "transformation.h"
#include "rasterize.h"

struct RenderData {
	int num_vertices;
	Vec3f* vertices;
	Vec2f* uvs;
	int num_uvs;
	Vec3f* normals;
	int num_triangles;
	int* triangles;	
	int* triangle_uvs;
	int* triangle_normals;
	Material* mat;
	struct Plane clipping_planes[6];
};

static inline void get_clipping_planes(struct Plane* planes){
	/* all normals facing 'inside'*/
	/* inside => -w<=x<=w, -w<=y<=w, 0<=z<=w*/
	//top (y = w)
	planes[0].n = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);
	planes[0].p = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);

	//bottom (y = -w)
	planes[1].n = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);
	planes[1].p = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);

	// left (x = -w)
	planes[2].n = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);
	planes[2].p = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);

	// right (x = w)
	planes[3].n = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);
	planes[3].p = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);

	// near (z = 0)
	planes[4].n = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	planes[4].p = VEC4F_0;

	// far (z = w)
	planes[5].n = vec4f_create(0.0f, 0.0f, -1.0f, 1.0f);
	planes[5].p = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
}

struct RenderData prepare_render_data(GameObject* go, Camera* cam) {

	struct RenderData r = {0};

	if(go->mesh == NULL){
		perror("mesh was null");
		return r;
	}

	// Vertex Data
	r.num_vertices = go->mesh->num_vertices;
	r.vertices = go->mesh->vertices;
	r.num_uvs = go->mesh->num_uvs;
	r.uvs = go->mesh->uvs;
	r.triangle_uvs = go->mesh->triangle_uvs;
	r.normals = go->mesh->normals;
	r.triangle_normals = go->mesh->triangle_normals;

	// Triangle Data
	r.num_triangles = go->mesh->num_triangles;
	r.triangles = go->mesh->triangles;

	// Material Data
	r.mat = material_default();
	if(go->material != NULL) r.mat = go->material;

	// Clipping Planes
	get_clipping_planes(r.clipping_planes);

	return r;
}


void assemble_triangle(Triangle* tri, int tri_idx, struct RenderData data){

	int v0_idx = data.triangles[tri_idx];
	int v1_idx = data.triangles[tri_idx + 1];
	int v2_idx = data.triangles[tri_idx + 2];

	if( (data.uvs != NULL) && (data.triangle_uvs != NULL) ) {
		int uv0_idx = data.triangle_uvs[tri_idx];
		int uv1_idx = data.triangle_uvs[tri_idx + 1];
		int uv2_idx = data.triangle_uvs[tri_idx + 2];
		tri->v[0].uv = data.uvs[uv0_idx];
		tri->v[1].uv = data.uvs[uv1_idx];
		tri->v[2].uv = data.uvs[uv2_idx];
	}

	// convert to homogenous coordinates
	tri->v[0].pos = vec3f_to_vec4f(data.vertices[v0_idx], 1.0f);
	tri->v[1].pos = vec3f_to_vec4f(data.vertices[v1_idx], 1.0f);
	tri->v[2].pos = vec3f_to_vec4f(data.vertices[v2_idx], 1.0f);

	if(data.normals != NULL) { //optional
		tri->v[0].n = data.normals[v0_idx];
		tri->v[1].n = data.normals[v1_idx];
		tri->v[2].n = data.normals[v2_idx];
	}

}

void precompute_interpolated_values(Triangle* tri) {
	tri->v[0].w_inv = (float)1.0f/tri->v[0].pos.w;
	tri->v[1].w_inv = (float)1.0f/tri->v[1].pos.w;
	tri->v[2].w_inv = (float)1.0f/tri->v[2].pos.w;

	tri->v[0].uv_over_w = vec2f_scale(tri->v[0].uv, tri->v[0].w_inv);
	tri->v[1].uv_over_w = vec2f_scale(tri->v[1].uv, tri->v[0].w_inv);
	tri->v[2].uv_over_w = vec2f_scale(tri->v[2].uv, tri->v[0].w_inv);
}

void render_game_object(uint32_t* framebuffer, float* zbuffer, Scene* scene, GameObject* go){
		
		struct RenderData data = prepare_render_data(go, scene->cam);
		
		if(NULL == data.vertices) return; // required
						  
		// pre-compute matrices
		Mat4 model, view, projection, view_port;
		model = get_model_matrix(go->transform);
		view = get_view_matrix(*scene->cam);
		projection = get_projection_matrix(*scene->cam, (float)HEIGHT/WIDTH);
		view_port = get_viewport_matrix(*scene->cam);

		for(int t = 0; t < data.num_triangles; t++) {

			Triangle tri = {0};
			int tri_idx = 3*t;

			assemble_triangle(&tri, tri_idx, data);
			apply_transformation(model,&tri);
			apply_transformation(view,&tri);
			precompute_lighting(data.mat, tri, scene);
			apply_transformation(projection,&tri);

			struct ClipResult r = clip_tri(tri, data.clipping_planes, 6);

			for(int k = 0; k < r.num_tris; k++){
				precompute_interpolated_values(&r.tris[k]);			
				apply_perspective_divide(&r.tris[k]); // divide (x,y,z,w) by w
				apply_transformation(view_port, &r.tris[k]);
				rasterize_triangle(r.tris[k], scene->cam, &scene->light, data.mat, framebuffer, zbuffer);
			}

		}

}

void render_scene(uint32_t* framebuffer, float* zbuffer, Scene* scene) {
	
	for(int i = 0; i < scene->num_gos; i++) {
		GameObject* go = scene->gos[i];
		render_game_object(framebuffer, zbuffer, scene, go);
	}
}

