#ifndef CLIP_H
#define CLIP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "triangle.h"
#include "constants.h"
#include "vector.h"

typedef struct ClipResult {
	int num_tris;
	Triangle tris[6];
} ClipResult;

typedef struct Plane { 
	Vec4f n; //normal
	Vec4f p; // point on the plane
} Plane;

Vec4f intersect(Plane P, Vec4f u, Vec4f v);
bool inside(Plane P, Vec4f x, float eps);
float sdf(Plane P, Vec4f x);
Vec4f lerp(Vec4f u, Vec4f v, float t);
int clip_against_plane(Vec4f* in,Vec2f* in_uv, int in_n, Plane P, Vec4f* out, Vec2f* out_uv);

ClipResult clip_tri(const Triangle* tri, Plane * planes, int num_planes);

#endif
