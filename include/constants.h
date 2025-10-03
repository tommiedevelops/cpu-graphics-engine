#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WIDTH (1280)
#define HEIGHT (720)
#define COLOR_RED (0xFF0000FF)
#define LENGTH_SCALE (1.0f)
#define PI (3.14159265359)
#define COLOR_BLUE (0x0000FFFF)

#define LOG_ERROR(fmt, ...) \
    do { \
        int err = errno; \
        fprintf(stderr, "[ERROR] (%s:%d in %s) " fmt "%s%s\n", \
                __FILE__, __LINE__, __func__, \
                ##__VA_ARGS__, \
                (err ? ": " : ""), (err ? strerror(err) : "")); \
    } while (0)

#endif
