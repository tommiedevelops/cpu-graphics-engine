#ifndef CLIP_H
#define CLIP_H

typedef struct Triangle Triangle;
typedef struct Plane4 Plane4;
int clip_tri(const Triangle* tri, Triangle* clip_result);

#endif
