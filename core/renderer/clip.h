#ifndef CLIP_H
#define CLIP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "triangle.h"
#include "constants.h"
#include "vector.h"
typedef struct Plane4 Plane4;

int clip_tri(const Triangle* tri, Triangle* clip_result);

#endif
