#ifndef INPUT_FILE_H
#define INPUT_FILE_H

#include "input_source.h"
#include <stdio.h>

typedef struct {
    const char *filename;
    FILE *fp;
} file_input_ctx_t;

int file_input_open(input_source_t *src);
ssize_t file_input_read(input_source_t *src, uint8_t *dst, size_t max_len);
void file_input_close(input_source_t *src);

#endif