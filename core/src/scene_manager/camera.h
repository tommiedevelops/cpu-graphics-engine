#ifndef CAMERA_H
#define CAMERA_H

typedef struct Transform Transform;

typedef struct Camera { 
	Transform* transform;
	float fov;
	float near;
	float far;
	float screen_width;
	float screen_height;
} Camera;

Camera*    camera_create(Transform* tr, int screen_width, int screen_height);
void       camera_set_fov_degrees(Camera* cam, float fov_degrees);
void       camera_set_near(Camera* cam, float near);
void       camera_set_far(Camera* cam, float far);
void       camera_set_transform(Camera* cam, Transform* tr);
float      camera_get_fov(const Camera* cam);
float      camera_get_near(const Camera* cam);
float      camera_get_far(const Camera* cam);
int        camera_get_screen_width(const Camera* cam);
int        camera_get_screen_height(const Camera* cam);
Transform* camera_get_transform(const Camera* cam); 
void       camera_destroy(Camera* cam);
#endif
