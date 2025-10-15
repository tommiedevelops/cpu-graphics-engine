#ifndef CLIP_H
#define CLIP_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "triangle.h"
#include "constants.h"
#include "vector.h"

struct ClipResult {
	int num_tris;
	struct Triangle tris[6];
};

struct Plane { 
	Vec4f n; //normal
	Vec4f p; // point on the plane
};

Vec4f intersect(struct Plane P, Vec4f u, Vec4f v);
bool inside(struct Plane P, Vec4f x, float eps);
float sdf(struct Plane P, Vec4f x);
Vec4f lerp(Vec4f u, Vec4f v, float t);
int clip_against_plane(Vec4f* in,Vec2f* in_uv, int in_n, struct Plane P, Vec4f* out, Vec2f* out_uv);

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes);

#endif
