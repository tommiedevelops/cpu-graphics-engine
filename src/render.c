#include <stdio.h>
#include <stdint.h>
#include "constants.h"
#include "line.h"

int render_lines(uint32_t *framebuffer, int* coords, int num_coords) {
	// array organisation:
	// [[start_coord_x_0],[start_coord_y_0],[end_coord_x_0], [end_coord_y_0] ...]
	// where [start_coord_X_X] and [end_coord_X_X] are integers.
	for(int i = 0; i < num_coords; i++){
		int* coord = coords + 4*i;
		printf("drawing line: {%d,%d} -> {%d,%d}\n", *coord, *(coord+1),*(coord+2),*(coord+3));
		draw_line_general(framebuffer, coords + 4*i, coords + 4*i + 2);
	}
	return 0;
}



