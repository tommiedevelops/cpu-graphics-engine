#include <stdlib.h>
#include <string.h>

#include "material.h"
#include "clip.h"
#include "primitive.h"
#include "vector.h"
#include "transformation.h"
#include "rasterize.h"

typedef struct RenderData {
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
} RenderData;

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

	return r;
}

void assemble_triangle(Triangle* tri, int tri_idx, const RenderData* data){

	int v0_idx = data->triangles[tri_idx];
	int v1_idx = data->triangles[tri_idx + 1];
	int v2_idx = data->triangles[tri_idx + 2];

	if( (data->uvs != NULL) && (data->triangle_uvs != NULL) ) {
		int uv0_idx = data->triangle_uvs[tri_idx];
		int uv1_idx = data->triangle_uvs[tri_idx + 1];
		int uv2_idx = data->triangle_uvs[tri_idx + 2];
		tri->v[0].uv = data->uvs[uv0_idx];
		tri->v[1].uv = data->uvs[uv1_idx];
		tri->v[2].uv = data->uvs[uv2_idx];
	}

	// convert to homogenous coordinates
	tri->v[0].pos = vec3f_to_vec4f(data->vertices[v0_idx], 1.0f);
	tri->v[1].pos = vec3f_to_vec4f(data->vertices[v1_idx], 1.0f);
	tri->v[2].pos = vec3f_to_vec4f(data->vertices[v2_idx], 1.0f);

	if(data->normals != NULL) { //optional
		tri->v[0].n = data->normals[v0_idx];
		tri->v[1].n = data->normals[v1_idx];
		tri->v[2].n = data->normals[v2_idx];
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

void render_game_object(uint32_t* framebuffer, float* zbuffer, Light* lights, int num_lights, Camera* cam, GameObject* go){
		
		struct RenderData data = prepare_render_data(go, cam);
		
		if(NULL == data.vertices) return; // required
						  
		// pre-compute matrices
		Mat4 model, view, projection, view_port;
		model = get_model_matrix(go->transform);
		view = get_view_matrix(*cam);
		projection = get_projection_matrix(*cam, (float)HEIGHT/WIDTH);
		view_port = get_viewport_matrix(*cam);
		
		Triangle clip_result[6] = {0};
		for(int t = 0; t < data.num_triangles; t++) {
			int tri_idx = 3*t;
			Triangle tri = {0};
			assemble_triangle(&tri, tri_idx, &data);
			apply_transformation(model,&tri);
			apply_transformation(view,&tri);
			apply_transformation(projection,&tri);

			int num_tris = clip_tri(&tri, clip_result);

			for(int k = 0; k < num_tris; k++){
				precompute_interpolated_values(&clip_result[k]);			
				apply_perspective_divide(&clip_result[k]); // divide (x,y,z,w) by w
				apply_transformation(view_port, &clip_result[k]);
				rasterize_triangle(&clip_result[k], lights, data.mat, framebuffer, zbuffer);
			}

		}

}

void render_scene(uint32_t* framebuffer, float* zbuffer, Scene* scene) {
	
	for(int i = 0; i < scene->num_gos; i++) {
		GameObject* go = scene->gos[i];
		Camera* cam = scene->cam;
		Light* lights = &scene->light;
		int num_lights = 1; // CHANGE
		render_game_object(framebuffer, zbuffer, lights, num_lights, cam, go);
	}
}

