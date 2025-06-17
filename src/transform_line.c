#include <stdio.h>
#include "compute_line.h"

/*Function reflects 2D points along the line y = x*/
void reflect_on_yx(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++) {
		int temp = points[i].x;
		points[i].x = points[i].y;
		points[i].y = temp;
	}
}

/* Function reflects 2D points along x axis */
void reflect_on_x(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++){
		points[i].x = -1 * points[i].x;
	}
}

/* Function reflects 2D points along y axis */
void reflect_on_y(struct Point* points, int num_points) {
	for(int i = 0; i < num_points; i++){
		points[i].y = -1 * points[i].y;
	}
}

void transform_from_2_to_target_octant(int target_octant, struct Point* points, int num_points) {
	switch(target_octant) {
		case 1:
			reflect_on_yx(points, num_points);
			break;
		case 2:
			break;
		case 3:
			reflect_on_y(points, num_points);
			break;
		case 4:
			reflect_on_y(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		case 5:
			reflect_on_x(points, num_points);
			reflect_on_y(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		case 6:
			reflect_on_x(points, num_points);
			reflect_on_y(points, num_points);
			break;
		case 7:
			reflect_on_x(points, num_points);
			break;
		case 8:
			reflect_on_x(points, num_points);
			reflect_on_yx(points, num_points);
			break;
		default:
			printf("line.c/transform_to_octant_2: invalid octant supplied: curr_octant={%d}\n",target_octant);
	}
}





