#include <stdio.h>

#include "vector.h"
#include "bounds.h"

#include "mesh.h"
#include "obj_parser.h"

void shift_to_origin(Bounds3 bounds, Vec3f* vectors, int num_vectors) {
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x -= bounds.xmin;
                vectors[i].y -= bounds.ymin;
                vectors[i].z -= bounds.zmin;
        }
}

void normalize_lengths(Bounds3 bounds, Vec3f* vectors, int num_vectors) {
	float max = fmax(fmax(bounds.xmax, bounds.ymax), bounds.zmax);
        for(int i = 0; i < num_vectors; i++){
                vectors[i].x = (float)vectors[i].x / max;
                vectors[i].y = (float)vectors[i].y / max;
                vectors[i].z = (float)vectors[i].z / max;
		vectors[i] = vec3f_scale(vectors[i], 2.0f);
		vectors[i] = vec3f_sub(vectors[i], VEC3F_1);
        }
}

void normalize_vertices(Vec3f* vertices, int num_vertices) {
        Bounds3 bounds = get_bounds(vertices, num_vertices);

        shift_to_origin(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);

        normalize_lengths(bounds, vertices, num_vertices);
	bounds = get_bounds(vertices, num_vertices);
}

Mesh mesh_parse_from_obj(char* filename){
	FILE* fp = obj_open(filename);

	int num_verts = obj_parse_num_vertices(fp);
	Vec3f* verts = obj_parse_vertices(fp, num_verts);
	normalize_vertices(verts, num_verts);

	int num_uvs    = obj_parse_num_uvs(fp);
	Vec2f* uvs     = obj_parse_uvs(fp, num_uvs);
	int num_norms  = obj_parse_num_normals(fp);
	Vec3f* norms   = obj_parse_normals(fp, num_norms);
	int num_tris   = obj_parse_num_triangles(fp);

	int* tris;
	int* tri_norms;
	int* tri_uvs;

	MeshData data;
	obj_parse_triangles(fp, num_tris, num_verts, num_norms, num_uvs, &data);
	obj_close(fp);

	return (Mesh) {
		.num_normals      = num_norms,
		.normals          = norms,
		.num_uvs          = num_uvs,
		.uvs              = uvs,
		.num_vertices     = num_verts,
		.vertices         = verts,
		.num_triangles    = num_tris,
		.triangles        = data.tris,
		.triangle_uvs     = data.tri_uvs,
		.triangle_normals = data.tri_norms
	};

}

int mesh_recalculate_normals(Mesh* m){
	if(NULL == m) return -1;
	
	int n = m->num_vertices;
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
		normals[v1_idx] = vec3f_add(normals[v1_idx], n);
		normals[v2_idx] = vec3f_add(normals[v2_idx], n);
	}	

	for(int i = 0; i < n; i++){
		normals[i] = vec3f_normalize(normals[i]);
	}

	m->normals = normals;
}

