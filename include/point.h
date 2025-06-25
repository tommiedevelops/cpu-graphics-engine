#ifndef POINT_H
#define POINT_H

#include "triangle.h"
struct Triangle;
struct Point {
	int x;
	int y;
};

struct PointArray {
	struct Point* points;
	int num_points;
};

struct BaryPoint {
	float alpha;
	float beta;
	float gamma;
};

struct BaryPointArray {
	struct BaryPoint* bary_points;
	int num_bary_points;
};

struct BaryPoint convert_point_to_bary_point(struct Point point, struct Triangle tri);

struct BaryPointArray convert_point_arr_to_bary_point_arr(struct PointArray point_array, struct Triangle tri);

// in terms of a triangle

void destroy_point_array(struct PointArray point_array);

void destroy_bary_point_array(struct BaryPointArray bary_point_array);
#endif
