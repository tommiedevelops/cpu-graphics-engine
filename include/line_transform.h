#ifndef LINE_TRANSFORM_H
#define LINE_TRANSFORM_H

	struct Point {
        	int x;
        	int y;
	};

	int identify_octant(int dx, int dy);
	void reflect_on_x(struct Point* points, int num_points);
	void reflect_on_yx(struct Point* points, int num_points);
	void transform_from_octant_2(int curr_octant, struct Point* points, int num_points);
	void reflect_on_y(struct Point* points, int num_points);
#endif
