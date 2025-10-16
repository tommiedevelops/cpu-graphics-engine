#include "normals.h"


// have to detect adjacent faces
// average of the normals in adjacent faces
int recalculate_normals(Mesh* m){
	if(NULL == m) return -1;
	
	int n = m->num_vertices;

	int* tri_num_normals = malloc(sizeof(int)*n);
	memset(tri_num_normals,0x0,sizeof(int)*n);

	m->num_normals = n;
	Vec3f* normals = malloc(sizeof(Vec3f)*n);
	for(int i = 0; i < n; i++) normals[i] = VEC3F_0;
	
	int t = m->num_triangles;

	// same as triangles since we're manually calculating
	m->triangle_normals = malloc(sizeof(int)*3*t);	

	for(int i = 0; i < 3*t; i++){
		m->triangle_normals[i] = m->triangles[i];
	}

	for(int i = 0; i < t; i++){
		int tri_idx = 3*i;

		int v0_idx = m->triangles[tri_idx];
		int v1_idx = m->triangles[tri_idx + 1];
		int v2_idx = m->triangles[tri_idx + 2];

		Vec3f v0 = m->vertices[v0_idx];
		Vec3f v1 = m->vertices[v1_idx];
		Vec3f v2 = m->vertices[v2_idx];

		Vec3f x = vec3f_sub(v0,v1);
		Vec3f y = vec3f_sub(v0,v2);
		Vec3f n = vec3f_cross(y,x);
		
		normals[v0_idx] = vec3f_add(normals[v0_idx], n);
		tri_num_normals[v0_idx]++;	

		normals[v1_idx] = vec3f_add(normals[v1_idx], n);
		tri_num_normals[v1_idx]++;	

		normals[v2_idx] = vec3f_add(normals[v2_idx], n);
		tri_num_normals[v2_idx]++;	
	}	

	for(int i = 0; i < n; i++){
		normals[i] = vec3f_normalize(normals[i]);
	}

	m->normals = normals;
}

