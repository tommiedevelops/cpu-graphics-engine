#ifndef CLIP_H
#define CLIP_H

#include <stdio.h>
#include <stdbool.h>

#include "triangle.h"
#include "constants.h"
#include "vector.h"

struct ClipResult {
	int num_tris;
	struct Triangle tris[2];
};

struct Plane { 
	struct Vec3f n; //normal
	struct Vec3f p; // point on the plane
};

struct Vec3f intersect(struct Plane P, struct Vec3f u, struct Vec3f v);
bool inside(struct Plane P, struct Vec3f x, float eps);
float sdf(struct Plane P, struct Vec3f x);
struct Vec3f lerp(struct Vec3f u, struct Vec3f v, float t);
int clip_against_plane(struct Vec3f* verts, int num_verts, struct Plane P);

struct ClipResult clip(struct Triangle tri, struct Plane * planes, int num_planes);

#endif
