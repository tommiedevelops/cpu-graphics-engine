#include <stdint.h>
#include <stdlib.h>
#include "texture.h"
#include "mesh.h"
#include "material.h"

typedef struct {
	Texture** textures;
	uint8_t t_len, t_cap;

	Mesh** meshes;
	uint8_t m_len, m_cap;

	Material** materials;
	uint8_t mt_len, mt_cap;
} Assets;

Assets* assets_init() {

	Assets*  a = malloc(sizeof(Assets));

	uint8_t t_len = 0, t_cap = 2;
	Texture** t = malloc(t_cap*sizeof(Texture*));

	uint8_t m_len = 0, m_cap = 2;
	Mesh** m = malloc(m_cap*sizeof(Mesh*));

	a->textures = t;
	a->meshes = m;
	a->t_len = t_len;
	a->t_cap = t_cap;
	a->m_len = m_len;
	a->m_cap = m_cap;


	return a;
};

uint8_t assets_add_tex(Assets* a, Texture* tex) {
	bool full = (a->t_cap == a->t_len);
	
	if(!full) {
		a->textures[a->t_len] = tex;
		return a->t_len++;
	}

	size_t new_cap = a->t_cap + 2;
	Texture** new = realloc(a->textures, new_cap*sizeof(Texture*));	
	for(int i = 0; i < a->t_len; i++) new[i] = a->textures[i];

	free(a->textures);
	a->textures = new;
	a->t_cap = new_cap;

	a->textures[a->t_len] = tex;
	return a->t_len++;
}

uint8_t assets_add_mesh(Assets* a, Mesh* mesh) {
	bool full = (a->m_cap == a->m_len);
	
	if(!full) {
		a->meshes[a->m_len] = mesh;
		return a->m_len++;
	}

	size_t new_cap = a->m_cap + 2;
	Mesh** new = realloc(a->meshes, new_cap*sizeof(Texture*));	
	for(int i = 0; i < a->m_len; i++) new[i] = a->meshes[i];

	free(a->meshes);
	a->meshes = new;
	a->m_cap = new_cap;

	a->meshes[a->m_len] = mesh;
	return a->m_len++;
}

void assets_uninit(Assets* a) {
	for(int i = 0; i < a->t_len; i++) free(a->textures[i]);
	for(int i = 0; i < a->m_len; i++) free(a->meshes[i]);
	free(a->textures);
	free(a->meshes);
	free(a);
}

