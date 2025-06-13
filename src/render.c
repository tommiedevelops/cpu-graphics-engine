#include <stdio.h>
#include <stdint.h>
#include "headerfiles/constants.h"
#include "headerfiles/line.h"

int draw_lines_to_screen(uint32_t *framebuffer, int* coords, int num_coords) {
	// array organisation:
	// [[start_coord_0],[end_coord_0],[start_coord_1], [end_coord_1] ...]
	// where [start_coord_X] and [end_coord_X] are integers.
	for(int i = 0; i < num_coords; i++){
		printf("draw_lines: %d\n", i);
	}

}



