#include <stdio.h>
#include "cpu_engine.h"

#define CLEAR_COLOR (0x87CEEBFF)
#define W_WIDTH (512)
#define W_HEIGHT (392)
#define W_NAME ("cpu graphics engine")

int main(void) {

	Window*       win = window_create(W_WIDTH, W_HEIGHT, W_NAME);
	Pipeline*     p   = pipeline_create(vs_default, fs_phong);
	FrameBuffer* fb   = frame_buffer_create(W_WIDTH, W_HEIGHT, CLEAR_COLOR);
	Renderer*     r   = renderer_create(p);

	// Inititalising scene
	Transform* cam_tr = transform_create((Vec3f){0.0f,0.0f,-5.0f}, QUAT_IDENTITY, VEC3F_1);
	Camera*       cam = camera_create(cam_tr, W_WIDTH, W_HEIGHT);
	Light*      light = light_create(VEC3F_1, VEC4F_1);
	Scene*      scene = scene_create(cam, light);

	/* Building and adding GameObjects */
	
	// Preparing mesh, material and initial transform
	Mesh*      mesh  = mesh_parse_from_obj("assets/models/bunny.obj");
	mesh_recalculate_normals(mesh);
	Vec4f      green = (Vec4f){0.1f, 1.0f,0.1f, 1.0f}; // Maybe define RGBA struct
	Material*  mat   = material_create(green, NULL, NULL);
	Transform* tr    = transform_create(VEC3F_0, QUAT_IDENTITY, VEC3F_1);

	GameObj* bunny = game_obj_create(tr, mesh, mat);
	scene_add_game_object(scene, bunny);

	// Render Loop
	int num_frames = 300;
	int i = 0;
	while(i < num_frames) {
		frame_buffer_clear(fb);
		renderer_draw_scene(r,fb,scene);
		window_update(win, fb->framebuffer);
		printf("frame = %d\n", i);
		i++;
	}
	
        return 0;
}
