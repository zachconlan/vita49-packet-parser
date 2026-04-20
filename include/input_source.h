#ifndef INPUT_SOURCE_H
#define INPUT_SOURCE_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct input_source input_source_t;

struct input_source {
    int (*open)(input_source_t *src);
    ssize_t (*read)(input_source_t *src, uint8_t *dst, size_t max_len);
    void (*close)(input_source_t *src);
    void *ctx;
};

#endif