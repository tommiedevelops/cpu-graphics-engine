#include "render.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
	framebuffer[x + WIDTH*y] = value;
}

struct RenderData {
	int num_vertices;
	struct Vec3f* vertices;
	struct Vec2f* uvs;
	struct Vec3f* normals;
	int num_triangles;
	int* triangles;	
	struct Material mat;
};

struct RenderData prepare_render_data(struct GameObject go) {

	struct RenderData r = {0};

	if(go.mesh == NULL){
		//LOG_ERROR("No mesh to render");
		return r;
	}

	// Vertex Data
	r.num_vertices = go.mesh->num_vertices;
	r.vertices = go.mesh->vertices;
	r.uvs = go.mesh->uvs;
	r.normals = NULL; // FIX!!

	// Triangle Data
	r.num_triangles = go.mesh->num_triangles;
	r.triangles = go.mesh->triangles;

	// Material Data
	struct Material mat = material_default();
	if(go.material != NULL) r.mat = *go.material;

	return r;
}

void render_game_object(uint32_t* framebuffer, float* zbuffer, struct Scene scene, struct GameObject go){
		
		struct RenderData data = prepare_render_data(go);
		if(NULL == data.vertices) return; // required

		for(int t = 0; t < data.num_triangles; t++) {

			struct Triangle tri = {0};

			int tri_idx = 3*t;

			int v0_idx = data.triangles[tri_idx];
			int v1_idx = data.triangles[tri_idx + 1];
			int v2_idx = data.triangles[tri_idx + 2];

			// convert to homogenous coordinates
			tri.v0 = vec3f_to_vec4f(data.vertices[v0_idx], 1.0f);
			tri.v1 = vec3f_to_vec4f(data.vertices[v1_idx], 1.0f);
			tri.v2 = vec3f_to_vec4f(data.vertices[v2_idx], 1.0f);
	
			if(data.uvs != NULL) { // optional 
				tri.uv0 = data.uvs[v0_idx];
				tri.uv1 = data.uvs[v1_idx];
				tri.uv2 = data.uvs[v2_idx];
			} 

			if(data.normals != NULL) { //optional
				tri.n0 = data.normals[v0_idx];
				tri.n1 = data.normals[v1_idx];
				tri.n2 = data.normals[v2_idx];
			}

			// --- Vertex Shader ---
			struct Mat4 model, view, projection, view_port;

			model = get_model_matrix(go.transform);
			view = get_view_matrix(*scene.cam);
			projection = get_projection_matrix(*scene.cam);
			view_port = get_viewport_matrix(*scene.cam);

			apply_transformation(model, &tri);
			apply_transformation(view, &tri);
			apply_transformation(projection, &tri);
	
			// Clipping Algorithm	

			apply_perspective_divide(&tri); // divide (x,y,z,w) by w
			apply_transformation(view_port, &tri);

			// Fragment Shader
			rasterize_triangle(tri, &data.mat, framebuffer, zbuffer);
		}

}

void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene) {

	if(scene.gameObjects == NULL){
		printf("src/render.c/render_scene: no gameObjects to render\n");
		return;
	}
	
	for(int i = 0; i < scene.num_gameObjects; i++) {
		struct GameObject go = *scene.gameObjects[i];
		render_game_object(framebuffer, zbuffer, scene, go);
	}
}

