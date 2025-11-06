#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include <stdio.h>

#define LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR] (%s:%d) " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define NULL_PARAM_ERROR "Parameter(s) were NULL"
#endif
