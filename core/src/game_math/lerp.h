#ifndef LERP_H
#define LERP_H

typedef struct Vec2f Vec2f;
typedef struct Vec3f Vec3f;
typedef struct Vec4f Vec4f;

float lerp_float(float u, float v, float t);
Vec2f lerp_vec2f(Vec2f u, Vec2f v, float t);
Vec3f lerp_vec3f(Vec3f u, Vec3f v, float t);
Vec4f lerp_vec4f(Vec4f u, Vec4f v, float t);

#endif
