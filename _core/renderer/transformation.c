#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

Mat4 get_rotation_matrix(Quat rot) {
	return quat_to_mat4(quat_normalize(rot));
}

Mat4 get_scale_matrix(Vec3f scale) {
	Mat4 scale_matrix = {{
		{scale.x, 0,       0,       0},
		{0,       scale.y, 0,       0},
		{0,       0,       scale.z, 0},
		{0,       0,       0,       1.0f},
	}};
	return scale_matrix;
}

Mat4 get_translation_matrix(Vec3f pos) {
    	Mat4 translation_matrix = {{
       		{1.0f, 0, 0, pos.x},
        	{0, 1.0f, 0, pos.y},
        	{0, 0, 1.0f, pos.z},
        	{0, 0, 0, 1.0f    },
    	}};
    	return translation_matrix;	
}

Mat4 get_model_matrix(Vec3f pos, Quat rot, Vec3f scale){
	Mat4 result;
	result = get_scale_matrix(scale);
	result = mat4_mul_mat4(get_rotation_matrix(rot), result);
	result = mat4_mul_mat4(get_translation_matrix(pos), result);
	return result;
}

Mat4 get_view_matrix(Vec3f cam_pos, Quat cam_rot, Vec3f cam_scale){
	return mat4_affine_orthonormal_inverse(get_model_matrix(cam_pos, cam_rot, cam_scale));
}

Mat4 get_projection_matrix(float fov, float near, float far, float aspect) {

	Mat4 P = {0};

	P.m[0][0] = (float)aspect/tanf(0.5f*fov);
	P.m[1][1] = (float)1.0f/tanf(0.5f*fov);
	P.m[2][2] = (float)-1.0f*far/ (near - far);
	P.m[2][3] = (float)(near)*far/(near-far); 
	P.m[3][2] = 1.0f;

	return P;
}

Mat4 get_viewport_matrix(float near, float far, float vp_width, float vp_height){

	Mat4 P = {0};
	P.m[0][0] = vp_width/2;
	P.m[1][1] = -vp_height/2;
	P.m[0][3] = vp_width/2;
	P.m[1][3] = vp_height/2;
	P.m[2][2] = (far - near);
	P.m[2][3] = near;
	P.m[3][3] = 1.0f;
	return P;
}

