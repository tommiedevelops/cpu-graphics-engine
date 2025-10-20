#ifndef VERT_SHADER_H
#define VERT_SHADER_H

typedef struct VSin {
} VSin;

typedef struct VSout {
} VSout;

typedef struct VertShader {
	VSin* in;
	VSout* (*shade)(const VSin* in);
};

#endif
