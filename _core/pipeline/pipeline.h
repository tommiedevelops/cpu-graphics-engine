#ifndef PIPELINE_H
#define PIPELINE_H

#include "vert_shader.h"
#include "frag_shader.h"

typedef void (*VertShaderF)(const VSin* in, VSout* out, const VSUniforms* u);
typedef void (*FragShaderF)(const FSin* in, FSout* out, const FSUniforms* u);	

typedef struct Pipeline {
	VertShaderF vs;
	FragShaderF fs;
} Pipeline;

Pipeline* pipeline_init();
void pipeline_bind_vert_shader(Pipeline* p, VertShaderF vs);
void pipeline_bind_frag_shader(Pipeline* p, FragShaderF fs);

#endif
