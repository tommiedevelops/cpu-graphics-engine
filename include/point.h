#ifndef POINT_H
#define POINT_H
struct Point {
	int x;
	int y;
};

struct PointArray {
	struct Point* points;
	int num_points;
};

// in terms of a triangle

void destroy_point_array(struct PointArray point_array);
#endif
