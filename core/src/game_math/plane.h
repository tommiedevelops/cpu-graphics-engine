#include <stdbool.h>
#include "vector.h"

typedef struct Plane4 { 
	Vec4f n; //normal
	Vec4f p; // point on the plane
} Plane4;

float plane4_compute_intersect_t(Plane4 P, Vec4f u, Vec4f v);
bool  plane4_inside(Plane4 P, Vec4f x);
float plane4_sdf(Plane4 P, Vec4f x);

