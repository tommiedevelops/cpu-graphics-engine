#ifndef SHADING_H
#define SHADING_H

typedef struct Vec4f Vec4f;

typedef struct FragShaderInput {
} FragShaderInput;

typedef struct FragShader {
	FragShaderInput input;
	Vec4f (*shade)(FragShaderInput input);
} FragShader;

#endif 
