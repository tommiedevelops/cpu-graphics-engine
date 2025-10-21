#ifndef SHADING_H
#define SHADING_H

typedef struct Light Light;
typedef struct Vec4f Vec4f;

typedef struct {
	Vec3f frag_pos; // view space
	Vec3f normal; 
	Vec2f uv;
	float depth;
} FSin;

typedef struct {
	Vec4f color;
	float depth;
} FSout;

typedef struct {
	Light** lights;
	size_t num_lights;
} FSUniforms;

typedef struct Material Material;
typedef struct Triangle Triangle;

Vec3f compute_eyesight_vector(Vec3f cam_pos, Vec3f origin);

Vec4f compute_specular(float exponent, Vec4f light_col, Vec3f norm, Vec3f cam_pos, Vec3f light_dir, Triangle* tri);

Vec4f compute_diffuse(Vec4f albedo, Vec3f light_dir, Vec4f light_col, Vec3f norm);

#endif 
