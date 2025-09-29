#include "render.h"

void place_pixel(int x, int y, uint32_t value, uint32_t* framebuffer) {
	framebuffer[x + WIDTH*y] = value;
}

struct Vertex {
	struct Vec4f position; // Vec4f to support Mat4 
	struct Vec2f uv;
	struct Vec3f normal;
};

struct MyTriangle {
	struct Vertex v0;
	struct Vertex v1;
	struct Vertex v2;
};

struct Vertex vertex_create(struct Vec3f position, struct Vec2f uv, struct Vec3f normal){
	struct Vec4f pos = vec4f_create(position.x, position.y, position.z, 1.0f);

	struct Vertex v = {
		.position = pos,
		.uv = uv,
		.normal = normal
	};

	return v;
}

struct MyTriangle triangle_create(struct Vertex v0, struct Vertex v1, struct Vertex v2){

	struct MyTriangle tri = {
		.v0 = v0,
		.v1 = v1,
		.v2 = v2
	};

	return tri;
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

struct Fragment {
	struct Vec4f position;
};

void render_game_object(uint32_t* framebuffer, float* zbuffer, struct Scene scene, struct GameObject go){

		if(go.mesh == NULL){
			//LOG_ERROR("No mesh to render");
			return;
		}

		// Vertex Data
		int num_vertices = go.mesh->num_vertices;
		struct Vec3f* vertices = go.mesh->vertices;
		struct Vec2f* uvs = go.mesh->uvs;
		struct Vec3f* normals = NULL; // FIX!!
		
		// Triangle Data
		int num_triangles = go.mesh->num_triangles;
		int* triangles = go.mesh->triangles;

		// Material Data
		struct Material mat = material_default();
		if(go.material != NULL) mat = *go.material;

		// Transform and rasterize each triangle
		for(int t = 0; t < num_triangles; t++) {
			// GPU WORK
			
			// Primitive Assembly
			struct Vec3f p0 = (vertices) ? vertices[triangles[3*t]] : VEC3F_0;
			struct Vec3f p1 = (vertices) ? vertices[triangles[3*t+1]]: VEC3F_0;
			struct Vec3f p2 = (vertices) ? vertices[triangles[3*t+2]] : VEC3F_0;

			struct Vec2f uv0 = (uvs) ? uvs[triangles[3*t]] : VEC2F_0;
			struct Vec2f uv1 = (uvs) ? uvs[triangles[3*t+1]]: VEC2F_0;
			struct Vec2f uv2 = (uvs) ? uvs[triangles[3*t+2]] : VEC2F_0;
			
			struct Vec3f n0 = (normals) ? normals[triangles[3*t]]: VEC3F_0;
			struct Vec3f n1 = (normals) ? normals[triangles[3*t+1]]: VEC3F_0;
			struct Vec3f n2 = (normals) ? normals[triangles[3*t+2]] : VEC3F_0;

			struct Vertex v0 = vertex_create(p0, uv0, n0);
			struct Vertex v1 = vertex_create(p1, uv1, n1);
			struct Vertex v2 = vertex_create(p2, uv2, n2);
			
			struct Mat4 model = get_model_matrix(go.transform);
			struct Mat4 view = get_view_matrix(*scene.cam);
			struct Mat4 projection = get_projection_matrix(*scene.cam);

			// ----- Vertex Shader -----
			/* apply_vertex_shader(&v0); */
			/* apply_vertex_shader(&v1); */
			/* apply_vertex_shader(&v2); */

			// ----- End Vertex Shader -----
			
			struct MyTriangle myTri = triangle_create(v0, v1, v2);

			// Rasterize
			// define a frag buffer?
				
			//struct Fragment frag = rasterize_triangle(myTri,&mat);
			// Apply Fragment Shader
			
			//merge_fragment(frag, framebuffer, zbuffer);

			struct Triangle tri = {0};
			struct Vec3f pos;
			struct Vec2f uv;
			struct Vec3f normal;	

			// positions
			tri.v0 = vertices[triangles[3*t]];
			tri.v1 = vertices[triangles[3*t+1]];
			tri.v2 = vertices[triangles[3*t+2]];
	
			// uvs
			if(uvs != NULL) {
				tri.uv0 = uvs[triangles[3*t]];
				tri.uv1 = uvs[triangles[3*t+1]];
				tri.uv2 = uvs[triangles[3*t+2]];
			}; 
	
			// Model to World 
			tri = apply_transformation(get_model_matrix(go.transform), tri);
			
			// World to Camera
			tri = apply_transformation(get_view_matrix(*scene.cam) ,tri);

			bool clipped = false;	
			tri = apply_perspective_projection(
					&clipped,
					get_projection_matrix(*scene.cam),
					tri
			);

			if(!clipped) return;

			// at this point the tri contains 'fragments' - points in Clip Space
			// i need to calculate the color of each vertex, then interpolate nicely across its surface
			// what's the difference between color and lighting tho? 

			// Clip to Viewport
			tri = apply_transformation(get_viewport_matrix(*scene.cam),tri);
			rasterize_triangle(tri, &mat, framebuffer, zbuffer);
		}

}

void render_scene(uint32_t* framebuffer, float* zbuffer, struct Scene scene) {

	if(scene.gameObjects == NULL){
		printf("src/render.c/render_scene: no gameObjects to render\n");
		return;
	}
	
	for(int i = 0; i < scene.num_gameObjects; i++) {

		struct GameObject go = *scene.gameObjects[i];
		struct Camera cam = *scene.cam;

		render_game_object(framebuffer, zbuffer, scene, go);
	}
}

