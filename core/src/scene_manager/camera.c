#include <stdlib.h>
#include "scene_manager/transform.h"
#include "scene_manager/camera.h"

#define PI (3.14159265359)
void camera_set_screen_width(Camera* cam, float screen_width) {
	if(!cam) return;
	cam->screen_width = screen_width;
}	

void camera_set_screen_height(Camera* cam, float screen_height) {
	if(!cam) return;
	cam->screen_height = screen_height;
}	

void camera_set_fov_degrees(Camera* cam, float fov_degrees){

	if(fov_degrees < 0.0f) fov_degrees = 0.0f;
	if(fov_degrees > 180.0f) fov_degrees = 180.0f;

	float fov_radians = PI*fov_degrees/180.0f;	
	cam->fov = fov_radians;
}

void camera_set_near(Camera* cam, float near){
	if(near < 0.0f) near = 0.0f;
	cam->near = near;
}

void camera_set_far(Camera* cam, float far){
	if(far < cam->near) far = cam->near;
	cam->far = far;
}

void camera_set_transform(Camera* cam, Transform* tr){
	if(!cam || !tr) return;
	cam->transform = tr;
}

Camera* camera_create(Transform* tr, int screen_width, int screen_height) {

	if(!tr) return NULL;
	Camera* cam = malloc(sizeof(Camera));

	if(!cam) {
		printf("camera malloc failed");
		return NULL;
	}

	float default_fov = 60.0f;
	float default_near = 1.0f;
	float default_far = 20.0f;

	camera_set_transform(cam, tr);
	camera_set_screen_width(cam, screen_width);
	camera_set_screen_height(cam, screen_height);
	camera_set_fov_degrees(cam, default_fov);
	camera_set_near(cam, 1.0f);
	camera_set_far(cam, 20.0f);
	
	return cam;
}

float camera_get_fov(const Camera* cam) {
	 return cam->fov;
}
float camera_get_near(const Camera* cam) {
	return cam->near;
}
float camera_get_far(const Camera* cam) {
	return cam->far;
}
Transform* camera_get_transform(const Camera* cam) {
	return cam->transform;
}

int camera_get_screen_width(const Camera* cam) {
	return cam->screen_width;
}

int camera_get_screen_height(const Camera* cam) {
	return cam->screen_height;
}


void camera_destroy(Camera* cam) {
	free(cam);
}

