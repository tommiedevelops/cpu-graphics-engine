#include "vector.h"
#include "scene_manager.h"
#include "lighting.h"

Light* light_default() {
	Light* light = malloc(sizeof(Light));
	light->type = LIGHT_DIRECTIONAL;
	light->direction = vec3f_scale(VEC3F_1, -1.0f);
	light->color = VEC4F_1;
	return light;
}

Light* light_create(LightType type, Vec3f direction, Vec4f color) {
	Light* light = malloc(sizeof(Light));
	light->type = type;
	light->direction = direction;
	light->color = color;
}

Lighting* lighting_init(){
	Lighting* l = malloc(sizeof(Lighting));
	
	int len = 0; int cap = 2;
	Light** lights = malloc(cap*sizeof(Light*)); 
	
	l->len = len;
	l->cap = cap;
	l->lights = lights;

	return l;
}

void lighting_uninit(Lighting* lighting){
	for(int i = 0; i < lighting->len; i++) {
		free(lighting->lights[i]);
	}
	free(lighting->lights);
	free(lighting);
}

int lighting_add_light(Lighting* lighting, Light* light) {

	bool at_capacity = (lighting->cap == lighting->len);

	if(!at_capacity) {
		lighting->lights[lighting->len++] = light;
		return lighting->len;
	}

	int new_cap = lighting->cap + 2;
	Light** new_array = realloc(lighting->lights, new_cap*sizeof(Light*));	

	for(int i = 0; i < lighting->len; i++) new_array[i] = lighting->lights[i];

	free(lighting->lights);
	lighting->lights = new_array;
	lighting->cap = new_cap;

	int light_idx = lighting->len;
	lighting->lights[light_idx] = light;
	lighting->len++;

	return light_idx;
}

Light* lighting_get_light(Lighting* lighting, int idx){
	if(idx < 0 || idx > lighting->len) {
		printf("invalid light idx\n");
		return NULL;
	}

	return lighting->lights[idx];
}


