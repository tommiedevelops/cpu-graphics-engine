#include "render.h"
#include <math.h>

struct RenderData {
	int num_vertices;
	struct Vec3f* vertices;
	struct Vec2f* uvs;
	struct Vec3f* normals;
	int num_triangles;
	int* triangles;	
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
	r.uvs = go.mesh->uvs;
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

	// convert to homogenous coordinates
	tri->v0 = vec3f_to_vec4f(data.vertices[v0_idx], 1.0f);
	tri->v1 = vec3f_to_vec4f(data.vertices[v1_idx], 1.0f);
	tri->v2 = vec3f_to_vec4f(data.vertices[v2_idx], 1.0f);

	if(data.uvs != NULL) { // optional 
		tri->uv0 = data.uvs[v0_idx];
		tri->uv1 = data.uvs[v1_idx];
		tri->uv2 = data.uvs[v2_idx];
	} 

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
static inline struct Vec3f compute_tri_normal(struct Triangle tri) {

	struct Vec3f v0 = vec4f_to_vec3f(tri.v0);
	struct Vec3f v1 = vec4f_to_vec3f(tri.v1);
	struct Vec3f v2 = vec4f_to_vec3f(tri.v2);
	struct Vec3f x = vec3f_add(v0, vec3f_scale(v1, -1.0f));
	struct Vec3f y = vec3f_add(v0, vec3f_scale(v2, -1.0f));
	struct Vec3f n = vec3f_cross(x,y);
	return vec3f_normalize(n);
}

static inline struct Vec3f compute_reflection_vector(struct Vec3f l, struct Vec3f n){
	float dot = vec3f_dot(l,n);
	struct Vec3f r = vec3f_add(vec3f_scale(n, 2*dot),vec3f_scale(l, -1.0f));
	return vec3f_normalize(r);
}

static inline struct Vec3f compute_eyesight_vector(struct Vec3f cam_pos, struct Vec3f origin){
	return vec3f_normalize(vec3f_add(cam_pos, vec3f_scale(origin, -1.0f)));	
}

static inline struct Vec4f compute_specular(float exponent, struct Vec4f light_col, struct Vec3f norm, struct Vec3f cam_pos, struct Vec3f light_dir, struct Triangle tri){	
	// current heuristic: select random vertex as origin for eye vec
	
	struct Vec3f e = compute_eyesight_vector(cam_pos, vec4f_to_vec3f(tri.v0));
	struct Vec3f r = compute_reflection_vector(light_dir, norm);
	float r_dot_e = fmaxf(vec3f_dot(r,e), 0.0f);
	float specular = pow(r_dot_e,exponent); 

	struct Vec4f spec_vec = vec4f_create(light_col.x * specular, light_col.y * specular, light_col.z * specular, 1.0f);
	return spec_vec;
}

static inline struct Vec4f compute_diffuse(struct Vec3f light_dir, struct Vec4f light_col, struct Vec3f norm){
	float n_dot_l = fmaxf(vec3f_dot(norm, light_dir), 0.0f);
	struct Vec4f diffuse = vec4f_scale(light_col, n_dot_l);
	return diffuse;
}

void precompute_lighting(struct Material* mat, struct Triangle tri, struct LightSource ls, struct Camera cam){
	struct Vec3f norm = compute_tri_normal(tri);
	struct Vec3f light_dir = vec3f_normalize(ls.direction);

	struct Vec4f diffuse = compute_diffuse(ls.direction, ls.color, norm);
	float specular_intensity = 32.0f;
	struct Vec4f specular = compute_specular(specular_intensity, ls.color, norm, cam.transform.position, ls.direction, tri);
	mat->specular = specular;
	mat->diffuse = diffuse;
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

