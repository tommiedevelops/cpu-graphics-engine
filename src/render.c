#include "render.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
	framebuffer[x + WIDTH*y] = value;
}

struct Mat4 get_rotation_matrix(struct Transform tr) {
	return quat_to_mat4(quat_normalize(tr.rotation));
}

struct Mat4 get_scale_matrix(struct Transform tr) {
	struct Vec3f scale = tr.scale;
	struct Mat4 scale_matrix = {{
		{scale.x, 0,       0,       0},
		{0,       scale.y, 0,       0},
		{0,       0,       scale.z, 0},
		{0,       0,       0,       1.0f},
	}};
	return scale_matrix;
}

struct Mat4 get_translation_matrix(struct Transform tr) {
	struct Vec3f pos = tr.position;
    	struct Mat4 translation_matrix = {{
       		{1.0f, 0, 0, pos.x},
        	{0, 1.0f, 0, pos.y},
        	{0, 0, 1.0f, pos.z},
        	{0, 0, 0, 1.0f    },
    	}};
    	return translation_matrix;	
}

struct Mat4 get_model_matrix(struct Transform tr){
	struct Mat4 result;
	result = get_scale_matrix(tr);
	result = mat4_mul_mat4(get_rotation_matrix(tr), result);
	result = mat4_mul_mat4(get_translation_matrix(tr), result);
	return result;
}

struct Mat4 mat4_affine_orthonormal_inverse(struct Mat4 mat) {
	// special case of Mat4 being an affine, orthonormal transformation	
	float (*m)[4] = mat.m; // alias the existing storage
	
	struct Mat3 sub = {{
		{m[0][0], m[0][1], m[0][2]},
		{m[1][0], m[1][1], m[1][2]},
		{m[2][0], m[2][1], m[2][2]}
	}};

	struct Vec3f t = {.x = m[0][3], .y = m[1][3], .z = m[2][3]};
	struct Mat3 r_T = mat3_transpose(sub);
	struct Mat3 mR_T = scal_mul_mat3(-1.0f, r_T);

	struct Vec3f final_vec = mat3_mul_vec3(mR_T, t);

	struct Mat4 result = {0};
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			result.m[i][j] = r_T.m[i][j];
		}
	} 
	
	result.m[0][3] = final_vec.x;
	result.m[1][3] = final_vec.y;
	result.m[2][3] = final_vec.z;
	result.m[3][3] = 1.0f;

	return result;
}

struct Mat4 get_view_matrix(struct Camera cam){
	// i guess you'd just apply the inverse model matrix of the camera
	return mat4_affine_orthonormal_inverse(get_model_matrix(cam.transform));
}

struct Mat4 get_projection_matrix(struct Camera cam) {

	float fov = cam.fov;
	float aspect = (float) HEIGHT/WIDTH;
	float zn = cam.near;
	float zf = cam.far;

	struct Mat4 P = {0};
	P.m[0][0] = aspect/tan(0.5f*fov);
	P.m[1][1] = 1/tan(0.5f*fov);
	P.m[2][2] = (float)1.0f/ (zf - zn);
	P.m[2][3] = (float)(-zn)/(zf-zn); 
	P.m[3][2] = 1.0f;
	return P;
}

struct Vec4f perspective_divide(struct Vec4f v){
	struct Vec4f result = {0};
	
	if(v.w != 0){
		result.x = v.x/v.w;
		result.y = v.y/v.w;
		result.z = v.z/v.w;		
		result.w = 1.0f;
	}
	
	return result;
}

struct Mat4 get_viewport_matrix(struct Camera cam){

	float near = cam.near;
	float far = cam.far;

	struct Mat4 P = {0};
	P.m[0][0] = WIDTH/2;
	P.m[1][1] = HEIGHT/2;
	P.m[0][3] = WIDTH/2;
	P.m[1][3] = HEIGHT/2;
	P.m[2][2] = (far - near);
	P.m[2][3] = near;
	P.m[3][3] = 1.0f;
	return P;
}

// CHAT-GPT
static inline struct Vec4f vec3f_to_vec4f(struct Vec3f v, float w) {
    return (struct Vec4f){ v.x, v.y, v.z, w };
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

			// clipping
			
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

