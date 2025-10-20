#ifndef PIPELINE_H
#define PIPELINE_H

typedef struct Pipeline {
	VertShader* vs;
	FragShader* fs;
}

Pipeline* pipeline_init();
void pipeline_bind_vert_shader(Pipeline* p, VertShader* vs);
void pipeline_bind_frag_shader(Pipeline* p, FragShader* fs);

#endif
