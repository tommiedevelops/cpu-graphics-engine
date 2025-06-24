#include <stdio.h>
#include <stdlib.h>
#include "point.h"

void destroy_point_array(struct PointArray point_array){
	free(point_array.points);
}
