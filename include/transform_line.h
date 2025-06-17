#ifndef TRANSFORM_LINE_H
#define TRANSFORM_LINE_H

#include "compute_line.h"
	int identify_octant(int dx, int dy);
	void reflect_on_x(struct Point* points, int num_points);
	void reflect_on_yx(struct Point* points, int num_points);
	void transform_from_octant_2(int curr_octant, struct Point* points, int num_points);
	void reflect_on_y(struct Point* points, int num_points);
#endif
