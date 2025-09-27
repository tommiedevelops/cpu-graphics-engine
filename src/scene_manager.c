#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "scene_manager.h"
#include "matrix.h"
#include "constants.h"
#include "construct_plane.h"

// custom logic
struct Transform transform_create(struct Vec3f pos, struct Quaternion rot, struct Vec3f scale){
	struct Transform tr;
	tr.position = pos;
	tr.rotation = rot;
	tr.scale = scale;
	return tr;
}

struct Camera* camera_create(struct Transform tr){
	struct Camera* cam = malloc(sizeof(struct Camera));
	cam->transform = tr;
	cam->fov = PI*60.0f/180.0f;
	cam->near = 1.0f;
	cam->far = 20.0f;
	return cam;
}

void camera_set_fov_degrees(struct Camera* cam, float fov_degrees){

	if(fov_degrees < 0.0f) fov_degrees = 0.0f;
	if(fov_degrees > 180.0f) fov_degrees = 180.0f;

	float fov_radians = PI*fov_degrees/180.0f;	
	cam->fov = fov_radians;
}


void camera_set_near(struct Camera* cam, float near){
	if(near < 0.0f) near = 0.0f;
	cam->near = near;
}

void camera_set_far(struct Camera* cam, float far){
	cam->far = far;
}

struct GameObject* game_object_create(struct Transform tr, struct Mesh* mesh, struct Material* mat){
	struct GameObject* go = malloc(sizeof(struct GameObject));
	go->transform = tr;
	go->mesh = mesh;
	go->material = mat;
	return go;
}

struct Scene create_scene(){
		
	// Loading Assets
	
	struct Mesh* mesh = malloc(sizeof(struct Mesh));
        *mesh 	= parse_obj("./assets/models/bunny.obj");

	struct Mesh* teapot_mesh = malloc(sizeof(struct Mesh));
	*teapot_mesh = parse_obj("./assets/models/teapot.obj"); 

	struct Mesh* ground_mesh = malloc(sizeof(struct Mesh));
	*ground_mesh = create_square_plane();

	struct Texture* tex = texture_load("./assets/textures/brickwall.png");

	// creating materials
	struct Vec4f pink  = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f green = vec4f_create(0.2f, 0.8f, 0.2f, 1.0f);
	struct Vec4f blue  = vec4f_create(0.2f, 0.2f, 0.8f, 1.0f);

	struct Material* bunny_material  = material_create(pink, NULL);
	struct Material* teapot_material = material_create(green, NULL);
	struct Material* dragon_material = material_create(blue, NULL);
	struct Material* ground_material = material_create(VEC4F_1, tex); 	

	// creating game_objects
	struct Vec3f      ground_scale = {.x = 5.0f, .y = 1.0f, .z = 5.0f};
	struct Transform  ground_tr    = transform_create(VEC3F_0, QUAT_IDENTITY, ground_scale);
	struct GameObject* ground_go    = game_object_create(ground_tr, ground_mesh, ground_material);
		
	struct Vec3f      pos0      = {.x = 0.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  transform = transform_create(pos0, QUAT_IDENTITY, VEC3F_1);
	struct GameObject* go        = game_object_create(transform, mesh, bunny_material);

	struct Vec3f      pos1 = {.x = 2.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  tr1  = transform_create(pos1, QUAT_IDENTITY, VEC3F_1);
	struct GameObject* go1  = game_object_create(tr1, teapot_mesh, teapot_material);

	struct Vec3f      pos2 = {.x = -2.0f, .y = 1.0f, .z = 0.0f};
	struct Transform  tr2  = transform_create(pos2, QUAT_IDENTITY, VEC3F_1);
	struct GameObject* go2  = game_object_create(tr2, mesh, dragon_material);
	
	// game_object array
	int num_gameObjects = 4;
	int size = sizeof(struct GameObject*)*num_gameObjects;
	struct GameObject** gameObjects = malloc(size);

	gameObjects[0] = go;
	gameObjects[1] = go1;
	gameObjects[2] = go2;
	gameObjects[3] = ground_go;

	// creating & configuring camera
	struct Transform camera_transform = transform_create(vec3f_create(0.0f, 0.5f, 3.0f), QUAT_IDENTITY, VEC3F_1);
	struct Camera* cam = camera_create(camera_transform);
	camera_set_fov_degrees(cam, 60.0f); //remember to convert to radians in function 
	camera_set_near(cam, 5.0f);
	camera_set_far(cam, 20.0f);

	struct Scene scene = {
		.cam = cam,
	       	.gameObjects = gameObjects, 
		.num_gameObjects =num_gameObjects
	};

	return scene;	
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


/* vector array methods */

/* Shifts coordinates on all three axes to [0, imax - imin] */
static void shift_to_origin(struct Bounds bounds, struct Vec3f* vectors, int num_vectors) {
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x -= bounds.xmin;
                vectors[i].y -= bounds.ymin;
                vectors[i].z -= bounds.zmin;
        }
}

/* Normalizes values to between [-1,1] - assumes min = 0 for all axes */
static void normalize_lengths(struct Bounds bounds, struct Vec3f* vectors, int num_vectors) {

        // normalizes between [0,1]
	float max = fmax(fmax(bounds.xmax, bounds.ymax), bounds.zmax);
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x = (float)vectors[i].x / max;
                vectors[i].y = (float)vectors[i].y / max;
                vectors[i].z = (float)vectors[i].z / max;
        }

        // scale and translate to [-1,1]
        for(int i = 0; i < num_vectors; i++){
                scale_vector(vectors + i, 2);
                translate_vector(vectors + i, -1.0f,-1.0f,-1.0f);
        }
}

/* Takes normalized vectors between [-1,1] and scales them to [-target_length/2, target_length/2] */
static void scale_lengths(float target_length, struct Bounds bounds, struct Vec3f* vectors, int num_vectors){
        for(int i = 0; i < num_vectors; i++){
                scale_vector(vectors + i, (float)target_length/2.0f);
        }
}

/*
   normalize vectors to fit in box, side length s that is centered
   on the origin all axes scaled equally.
*/
void normalize_vertices(
			float sidelength,
			struct Vec3f* vertices,
			int num_vertices
){
        struct Bounds bounds = get_bounds(vertices, num_vertices);

        shift_to_origin(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        normalize_lengths(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        scale_lengths(sidelength, bounds, vertices, num_vertices);
}
