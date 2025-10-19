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

Mesh parse_obj(char* filename){
	FILE* fp = obj_open(filename);

	int num_vertices = obj_parse_num_vertices(fp);
	Vec3f* vertices = obj_parse_vertices(fp, num_vertices);

	normalize_vertices(vertices, num_vertices);

	int num_triangles = obj_parse_num_triangles(fp);
	int* triangles = obj_parse_triangles(fp, num_triangles, num_vertices, vertices);
	int num_uvs = obj_parse_num_uvs(fp);
	Vec2f* uvs = obj_parse_uvs(fp, num_uvs);
	int* triangle_uvs = obj_parse_triangle_uvs(fp, num_triangles, num_uvs, uvs);

	obj_close(fp);

	Mesh data = {
		.num_uvs = num_uvs,
		.uvs = uvs,
		.num_vertices = num_vertices,
		.vertices = vertices,
		.num_triangles = num_triangles,
		.triangles = triangles,
		.triangle_uvs = triangle_uvs
	};

	return data;
}
