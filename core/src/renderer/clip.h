#ifndef CLIP_H
#define CLIP_H

typedef struct Triangle Triangle;

int clip_tri(const Triangle* tri, Triangle* clip_result);
#endif
