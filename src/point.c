#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "triangle.h"

void destroy_point_array(struct PointArray point_array){
        free(point_array.points);
}

void destroy_bary_point_array(struct BaryPointArray bary_point_array){
	free(bary_point_array.bary_points);
}

struct BaryPoint convert_point_to_bary_point(struct Point P, struct Triangle tri){

	struct Vertex** sorted_verts = sort_vertices_by_y_asc(tri);
	struct Vertex A = *sorted_verts[0];
	struct Vertex B = *sorted_verts[1];
	struct Vertex C = *sorted_verts[2];

	free(sorted_verts);

	float alpha = (A.x*(C.y-A.y)+(P.y-A.y)*(C.x-A.x)-P.x*(C.y-A.y))
			/((B.y-A.y)*(C.x-A.x)-(B.x-A.x)*(C.y-A.y));;
	float beta = ((P.y-A.y) - alpha*(B.y-A.y))/(C.y-A.y);
	float gamma = 1 - alpha - beta;

	struct BaryPoint bary_point = {
		.alpha = alpha,
		.beta = beta,
		.gamma = gamma
	};

	return bary_point;

}

struct BaryPointArray convert_point_arr_to_bary_point_arr(
	struct PointArray point_array,
	struct Triangle tri
){
	int num_points = point_array.num_points;

	struct BaryPoint* bary_points = malloc(sizeof(struct BaryPoint)*num_points);

	for(int i = 0; i < num_points; i++){
		bary_points[i] = convert_point_to_bary_point(point_array.points[i], tri);
	}

	struct BaryPointArray bary_point_array = {
		.bary_points = bary_points,
		.num_bary_points = num_points
	};

	return bary_point_array;
}
