#ifndef SHADING_H
#define SHADING_H

typedef struct Vec4f Vec4f;

typedef struct FSin {
} FSin;

typedef struct FragShader {
	FSin* in;
	Vec4f (*shade)(const FSin* in);
} FragShader;

#endif 
