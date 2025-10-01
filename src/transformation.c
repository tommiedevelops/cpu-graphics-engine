#include "transformation.h"

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
	P.m[2][2] = (float)-1.0f/ (zn - zf);
	P.m[2][3] = (float)(zn)/(zn-zf); 
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

void apply_perspective_divide(struct Triangle* tri) {
	
	if(NULL == tri){
		//LOG_ERROR("tri is null")
		return;
	}

	tri->v0 = perspective_divide(tri->v0);
	tri->v1 = perspective_divide(tri->v1);
	tri->v2 = perspective_divide(tri->v2);
}


void apply_transformation(struct Mat4 tr, struct Triangle* tri) {
	if(NULL == tri){
		//LOG_ERROR("tri is null");
		return;
	}

	tri->v0 = mat4_mul_vec4(tr, tri->v0);
	tri->v1 = mat4_mul_vec4(tr, tri->v1);
	tri->v2 = mat4_mul_vec4(tr, tri->v2);
}	
