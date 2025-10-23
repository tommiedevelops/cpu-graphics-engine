#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <errno.h>

#define WIDTH (1280)
#define HEIGHT (720)
#define PI (3.14159265359)

#define LOG_ERROR(fmt, ...) \
    do { \
        int err = errno; \
        fprintf(stderr, "[ERROR] (%s:%d in %s) " fmt "%s%s\n", \
                __FILE__, __LINE__, __func__, \
                ##__VA_ARGS__, \
                (err ? ": " : ""), (err ? strerror(err) : "")); \
    } while (0)

#endif
