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
	struct Vec4f n; //normal
	struct Vec4f p; // point on the plane
};

struct Vec4f intersect(struct Plane P, struct Vec4f u, struct Vec4f v);
bool inside(struct Plane P, struct Vec4f x, float eps);
float sdf(struct Plane P, struct Vec4f x);
struct Vec4f lerp(struct Vec4f u, struct Vec4f v, float t);
int clip_against_plane(struct Vec4f* in,struct Vec2f* in_uv, int in_n, struct Plane P, struct Vec4f* out, struct Vec2f* out_uv);

struct ClipResult clip_tri(struct Triangle tri, struct Plane * planes, int num_planes);

#endif
