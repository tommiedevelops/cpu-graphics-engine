#include "render.h"

struct RenderData {
	int num_vertices;
	struct Vec3f* vertices;
	struct Vec2f* uvs;
	int num_uvs;
	struct Vec3f* normals;
	int num_triangles;
	int* triangles;	
	int* triangle_uvs;
	struct Material mat;
	struct Plane clipping_planes[6];
};

static inline void calculate_planes(struct Plane* planes){
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

struct RenderData prepare_render_data(struct GameObject go, struct Camera cam) {

	struct RenderData r = {0};

	if(go.mesh == NULL){
		//LOG_ERROR("No mesh to render");
		return r;
	}

	// Vertex Data
	r.num_vertices = go.mesh->num_vertices;
	r.vertices = go.mesh->vertices;
	r.num_uvs = go.mesh->num_uvs;
	r.uvs = go.mesh->uvs;
	r.triangle_uvs = go.mesh->triangle_uvs;
	r.normals = NULL; // FIX!!

	// Triangle Data
	r.num_triangles = go.mesh->num_triangles;
	r.triangles = go.mesh->triangles;

	// Material Data
	struct Material mat = material_default();
	if(go.material != NULL) r.mat = *go.material;

	// Clipping Planes
	calculate_planes(r.clipping_planes);

	return r;
}

void assemble_triangle(struct Triangle* tri, int tri_idx, struct RenderData data){

	int v0_idx = data.triangles[tri_idx];
	int v1_idx = data.triangles[tri_idx + 1];
	int v2_idx = data.triangles[tri_idx + 2];

	if( (data.uvs != NULL) && (data.triangle_uvs != NULL) ) {
		int uv0_idx = data.triangle_uvs[tri_idx];
		int uv1_idx = data.triangle_uvs[tri_idx + 1];
		int uv2_idx = data.triangle_uvs[tri_idx + 2];
		tri->uv0 = data.uvs[uv0_idx];
		tri->uv1 = data.uvs[uv1_idx];
		tri->uv2 = data.uvs[uv2_idx];
	}

	// convert to homogenous coordinates
	tri->v0 = vec3f_to_vec4f(data.vertices[v0_idx], 1.0f);
	tri->v1 = vec3f_to_vec4f(data.vertices[v1_idx], 1.0f);
	tri->v2 = vec3f_to_vec4f(data.vertices[v2_idx], 1.0f);

	if(data.normals != NULL) { //optional
		tri->n0 = data.normals[v0_idx];
		tri->n1 = data.normals[v1_idx];
		tri->n2 = data.normals[v2_idx];
	}

}

void precompute_interpolated_values(struct Triangle* tri) {
	tri->w0_inv = (float)1.0f/tri->v0.w;
	tri->w1_inv = (float)1.0f/tri->v1.w;
	tri->w2_inv = (float)1.0f/tri->v2.w;

	tri->uv0_over_w = vec2f_scale(tri->uv0, tri->w0_inv);
	tri->uv1_over_w = vec2f_scale(tri->uv1, tri->w1_inv);
	tri->uv2_over_w = vec2f_scale(tri->uv2, tri->w2_inv);
}

void render_game_object(uint32_t* framebuffer, float* zbuffer, struct Scene scene, struct GameObject go){
		
		struct RenderData data = prepare_render_data(go, *scene.cam);
		
		if(NULL == data.vertices) return; // required
						  
		// pre-compute matrices
		struct Mat4 model, view, projection, view_port;
		model = get_model_matrix(go.transform);
		view = get_view_matrix(*scene.cam);
		projection = get_projection_matrix(*scene.cam, (float)HEIGHT/WIDTH);
		view_port = get_viewport_matrix(*scene.cam);

		for(int t = 0; t < data.num_triangles; t++) {

			struct Triangle tri = {0};
			int tri_idx = 3*t;

			assemble_triangle(&tri, tri_idx, data);
			apply_transformation(model,&tri);
			// computed in world space
			precompute_lighting(&data.mat, tri, scene.light, *scene.cam);
			apply_transformation(view,&tri);
			apply_transformation(projection,&tri);

			struct ClipResult r = clip_tri(tri, data.clipping_planes, 6);
			for(int k = 0; k < r.num_tris; k++){
				precompute_interpolated_values(&r.tris[k]);			
				apply_perspective_divide(&r.tris[k]); // divide (x,y,z,w) by w
				apply_transformation(view_port, &r.tris[k]);
				rasterize_triangle(r.tris[k], scene.light, &data.mat, framebuffer, zbuffer);
			}

		}

}

void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene) {
	
	for(int i = 0; i < scene.num_gos; i++) {
		struct GameObject go = *scene.gos[i];
		render_game_object(framebuffer, zbuffer, scene, go);
	}
}

