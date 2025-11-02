#ifndef BARYCENTRIC_H
#define BARYCENTRIC_H

#include "vector.h"
typedef struct BaryCoords {
	float alpha, beta, gamma;
} BaryCoords;

BaryCoords cartesian_to_bary(Vec2f A, Vec2f B, Vec2f C, Vec2f P);
float bary_mix1(BaryCoords b, float v0, float v1, float v2);
Vec2f bary_mix2(BaryCoords b, Vec2f v0, Vec2f v1, Vec2f v2);
Vec3f bary_mix3(BaryCoords b, Vec3f v0, Vec3f v1, Vec3f v2);

#endif
