#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "asset_manager/texture.h"
#include "asset_manager/mesh.h"
#include "asset_manager/material.h"

typedef struct {
	Mesh** meshes;
	uint8_t m_len, m_cap;

	Material** materials;
	uint8_t mt_len, mt_cap;
} Assets;

Assets* assets_create() {

	Assets*  a = malloc(sizeof(Assets));

	uint8_t mt_len = 0, mt_cap = 2;
	Material** mats = malloc(mt_cap*sizeof(Material*));

	uint8_t m_len = 0, m_cap = 2;
	Mesh** m = malloc(m_cap*sizeof(Mesh*));

	a->materials =  mats;
	a->meshes = m;
	a->mt_len = mt_len;
	a->mt_cap = mt_cap;
	a->m_len = m_len;
	a->m_cap = m_cap;

	return a;
};

uint8_t assets_add_material(Assets* a, Material* mat, const char * handle) {
	bool full = (a->mt_cap == a->mt_len);
	
	if(!full) {
		mat->handle = handle;
		a->materials[a->mt_len++] = mat;
		return a->mt_len;
	}

	size_t new_cap = a->mt_cap + 2;
	Material** new = realloc(a->materials, new_cap*sizeof(Material*));	
	for(int i = 0; i < a->mt_len; i++) new[i] = a->materials[i];

	free(a->materials);
	a->materials = new;
	a->mt_cap = new_cap;

	mat->handle = handle;
	a->materials[a->mt_len++] = mat;
	return a->mt_len;
}

uint8_t assets_add_mesh(Assets* a, Mesh* mesh, const char * handle) {
	bool full = (a->m_cap == a->m_len);
	
	if(!full) {
		mesh->handle = handle;
		a->meshes[a->m_len++] = mesh;
		return a->m_len;
	}

	size_t new_cap = a->m_cap + 2;
	Mesh** new = realloc(a->meshes, new_cap*sizeof(Texture*));	
	for(int i = 0; i < a->m_len; i++) new[i] = a->meshes[i];

	free(a->meshes);
	a->meshes = new;
	a->m_cap = new_cap;

	mesh->handle = handle;
	a->meshes[a->m_len++] = mesh;
	return a->m_len;
}

Mesh* assets_get_mesh(Assets* a, const char* handle) {
	int n = a->m_len;

	for(int i = 0; i < n; i++) {
		Mesh* mesh = a->meshes[i];
		if(!strcmp(handle,mesh->handle)) return mesh;	
	}

	return NULL;
}

Material* assets_get_material(Assets* a, const char* handle) {
	int n = a->mt_len;

	for(int i = 0; i < n; i++) {
		Material* mat = a->materials[i];
		if(!strcmp(handle,mat->handle)) return mat;	
	}

	return NULL;
}


void assets_destroy(Assets* a) {
	if(NULL == a) return;
	
	if(a->materials) {
		for(int i = 0; i < a->mt_len; i++) {
	       		material_destroy(a->materials[i]);
		}

		free(a->materials);
	}

	if(a->meshes) {
		for(int i = 0; i < a->m_len; i++) {
			mesh_destroy(a->meshes[i]);
		}
		
		free(a->meshes);
	}

	free(a);
}

