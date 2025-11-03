#ifndef APP_H
#define APP_H

typedef struct App App;

App* app_create();
void app_init(void);
void app_run(App* app);
void app_shutdown(void);
void app_destroy(App* app);
#endif
