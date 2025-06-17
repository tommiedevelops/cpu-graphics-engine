#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "compute_line.h"
#include "constants.h"
#include "transform_line.h"

bool PointsAreEqual(struct Point a, struct Point b){
        if( (a.x == b.x) && (a.y == b.y) ) {
                return true;
        }
        return false;
}

int compute_num_points(int x0, int y0, int x1, int y1){
	return (int)fmax(abs(x1-x0),abs(y1-y0));
}

struct Point* draw_line_easy(int x0, int y0, int x1, int y1) {

	int dx = x1 - x0;
	int dy = y1 - y0;

	int num_pixels = compute_num_points(x0,y0,x1,y1);

	struct Point* points = malloc(num_pixels*sizeof(struct Point));

	float step = fmax(abs(dx), abs(dy));
	if(step != 0){
		float stepX = (float)dx / step;
		float stepY = (float)dy / step;
		for(int i = 0; i < step; i++){
			points[i].x = round(x0+i*stepX);
			points[i].y = round(y0+i*stepY);
		}
	}
	return points;
}




