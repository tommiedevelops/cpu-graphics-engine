#ifndef CLIP_H
#define CLIP_H

typedef struct Triangle Triangle;
typedef struct VSout VSout;

int clip_tri(const Triangle* tri, Triangle* clip_result);
int clip(VSout** in, VSout** out, int* out_n);

#endif
