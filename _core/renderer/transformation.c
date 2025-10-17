#include "transformation.h"

Mat4 get_rotation_matrix(Transform tr) {
	return quat_to_mat4(quat_normalize(tr.rotation));
}

Mat4 get_scale_matrix(Transform tr) {
	Vec3f scale = tr.scale;
	Mat4 scale_matrix = {{
		{scale.x, 0,       0,       0},
		{0,       scale.y, 0,       0},
		{0,       0,       scale.z, 0},
		{0,       0,       0,       1.0f},
	}};
	return scale_matrix;
}

Mat4 get_translation_matrix(Transform tr) {
	Vec3f pos = tr.position;
    	Mat4 translation_matrix = {{
       		{1.0f, 0, 0, pos.x},
        	{0, 1.0f, 0, pos.y},
        	{0, 0, 1.0f, pos.z},
        	{0, 0, 0, 1.0f    },
    	}};
    	return translation_matrix;	
}

Mat4 get_model_matrix(Transform tr){
	Mat4 result;
	result = get_scale_matrix(tr);
	result = mat4_mul_mat4(get_rotation_matrix(tr), result);
	result = mat4_mul_mat4(get_translation_matrix(tr), result);
	return result;
}

Mat4 mat4_affine_orthonormal_inverse(Mat4 mat) {
	// special case of Mat4 being an affine, orthonormal transformation	
	float (*m)[4] = mat.m; // alias the existing storage
	
	Mat3 sub = {{
		{m[0][0], m[0][1], m[0][2]},
		{m[1][0], m[1][1], m[1][2]},
		{m[2][0], m[2][1], m[2][2]}
	}};

	Vec3f t = {.x = m[0][3], .y = m[1][3], .z = m[2][3]};
	Mat3 r_T = mat3_transpose(sub);
	Mat3 mR_T = scal_mul_mat3(-1.0f, r_T);

	Vec3f final_vec = mat3_mul_vec3(mR_T, t);

	Mat4 result = {0};
	
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

Mat4 get_view_matrix(Camera cam){
	return mat4_affine_orthonormal_inverse(get_model_matrix(cam.transform));
}

Mat4 get_projection_matrix(Camera cam, float aspect) {

	float fov = cam.fov;
	float zn = cam.near;
	float zf = cam.far;

	Mat4 P = {0};

	P.m[0][0] = (float)aspect/tanf(0.5f*fov);
	P.m[1][1] = (float)1.0f/tanf(0.5f*fov);
	P.m[2][2] = (float)-1.0f*zf/ (zn - zf);
	P.m[2][3] = (float)(zn)*zf/(zn-zf); 
	P.m[3][2] = 1.0f;

	return P;
}

Vec4f perspective_divide(Vec4f v){
	Vec4f result = {0};
	
	if(v.w != 0){
		result.x = v.x/v.w;
		result.y = v.y/v.w;
		result.z = v.z/v.w;		
		result.w = 1.0f;
	}
	
	return result;
}

Mat4 get_viewport_matrix(Camera cam){

	float near = cam.near;
	float far = cam.far;

	Mat4 P = {0};
	P.m[0][0] = WIDTH/2;
	P.m[1][1] = -HEIGHT/2; // positive y is downwards rel to screen
	P.m[0][3] = WIDTH/2;
	P.m[1][3] = HEIGHT/2;
	P.m[2][2] = (far - near);
	P.m[2][3] = near;
	P.m[3][3] = 1.0f;
	return P;
}

void apply_perspective_divide(struct Triangle* tri) {
	tri->v[0].pos = perspective_divide(tri->v[0].pos);
	tri->v[1].pos = perspective_divide(tri->v[1].pos);
	tri->v[2].pos = perspective_divide(tri->v[2].pos);
}

void apply_transformation(Mat4 tr, struct Triangle* tri) {
	tri->v[0].pos = mat4_mul_vec4(tr, tri->v[0].pos);
	tri->v[1].pos = mat4_mul_vec4(tr, tri->v[1].pos);
	tri->v[2].pos = mat4_mul_vec4(tr, tri->v[2].pos);
}	
