#include <stdlib.h>
#include "light.h"
Light* light_create(Vec3f direction, Vec4f color) {
	Light* l = malloc(sizeof(Light));
	l->direction = direction;
	l->color = color;
	return l;
}
void light_destroy(Light* light) {
	free(light);
}


