#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

typedef struct Assets Assets;

Assets*   assets_init();
uint8_t   assets_add_tex(Assets* a, Texture* tex);
uint8_t   assets_add_mesh(Assets* a, Mesh* mesh);
void      assets_uninit(Assets* a);

#endif
