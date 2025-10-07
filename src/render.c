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
	struct Plane* clipping_planes;
};

static inline void ensure_inside(struct Plane* P, struct Vec4f point_inside){

	struct Vec4f x = vec4f_add(point_inside, vec4f_scale(P->p, -1.0f));		

	if(vec4f_dot(P->n, x) > 0){
		// n needs to be flipped
		P->n = vec4f_scale(P->n, -1.0f);
	}

}

static inline void calculate_planes(struct Plane* planes, float near, float far){

	struct Vec4f point_inside = vec4f_create(0.0f, 0.0f, 0.0f, 1.0f);

	//top
	planes[0].n = vec4f_create(0.0f, 1.0f, 0.0f, -1.0f);
	planes[0].p = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);

	ensure_inside(&planes[0], point_inside);

	//bottom
	planes[1].n = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);
	planes[1].p = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);

	ensure_inside(&planes[1], point_inside);

	// left
	planes[2].n = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);
	planes[2].p = vec4f_create(1.0f, 0.0f, 0.0f, -1.0f);

	ensure_inside(&planes[2], point_inside);

	// right
	planes[3].n = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);
	planes[3].p = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);

	ensure_inside(&planes[3], point_inside);

	// near
	planes[4].n = vec4f_create(0.0f, 0.0f, -1.0f, 0.0f);
	planes[4].p = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);

	ensure_inside(&planes[4], point_inside);

	// far
	planes[5].n = vec4f_create(0.0f, 0.0f, 1.0f, -1.0f);
	planes[5].p = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);

	ensure_inside(&planes[5], point_inside);
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
	struct Plane planes[6] = {0};
	calculate_planes(planes, cam.near, cam.far);	
	r.clipping_planes = planes;

	return r;
}

static inline bool vert_clipped(struct Vec4f v) {

	if(v.z <= 0 || v.z >= v.w) return true;
	if(v.x >= v.w || v.x <= -v.w) return true;
	if(v.y >= v.w || v.y <= -v.w) return true;

	return false; 
}

static inline bool tri_clipped(struct Triangle tri){
	return vert_clipped(tri.v0) || vert_clipped(tri.v1) || vert_clipped(tri.v2);
}

void render_game_object(uint32_t* framebuffer, float* zbuffer, struct Scene scene, struct GameObject go){
		
		struct RenderData data = prepare_render_data(go, *scene.cam);

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
			projection = get_projection_matrix(*scene.cam, (float)HEIGHT/WIDTH);
			view_port = get_viewport_matrix(*scene.cam);

			printf("cam pos:");
			print_vec3f(scene.cam->transform.position);
			printf("camera forward:"); 
			print_vec3f(quat_get_forward(scene.cam->transform.rotation));

			printf("object space:");
			print_vec4f(tri.v0);
			apply_transformation(model, &tri);

			printf("far = %f\n", scene.cam->far);

			printf("world space:");
			print_vec4f(tri.v0);
			apply_transformation(view, &tri);

			printf("camera space:");
			print_vec4f(tri.v0);
			apply_transformation(projection, &tri);

			printf("clip space:");
			print_vec4f(tri.v0);

			if(tri_clipped(tri)) {
				// clipping algorithm
				return;
			}

			apply_perspective_divide(&tri); // divide (x,y,z,w) by w

			printf("after perspective divide");
			print_vec4f(tri.v0);

			apply_transformation(view_port, &tri);

			printf("screen space");
			print_vec4f(tri.v0);
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

