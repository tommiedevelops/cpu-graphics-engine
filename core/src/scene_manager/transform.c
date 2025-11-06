#include <stdlib.h>
#include "scene_manager/transform.h"
Transform* transform_create(Vec3f pos, Quat rot, Vec3f scale) {
	Transform* tr = malloc(sizeof(Transform));
	tr->position   = pos;
	tr->rotation   = rot;
	tr->scale = scale;
	return tr;
}

Transform* transform_default(){
	Transform* tr = malloc(sizeof(Transform));
	tr->position = VEC3F_0;
	tr->rotation = QUAT_IDENTITY;
	tr->scale = VEC3F_1;
	return tr;
}

void transform_apply_rotation(Transform* tr, Quat rot) {
	tr->rotation = quat_mul(tr->rotation, rot);
	quat_normalize(tr->rotation);
}

void transform_apply_translation(Transform* tr, Vec3f delta) {
	tr->position = vec3f_add(tr->position, delta);
}

void transform_destroy(Transform* tr) {
	free(tr);
}
