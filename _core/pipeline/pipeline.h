#ifndef PIPELINE_H
#define PIPELINE_H

#include "vert_shader.h"
#include "frag_shader.h"

typedef struct Pipeline {
	VertShader* vs;
	FragShader* fs;
} Pipeline;

Pipeline* pipeline_init();
void pipeline_bind_vert_shader(Pipeline* p, VertShader* vs);
void pipeline_bind_frag_shader(Pipeline* p, FragShader* fs);


#endif
