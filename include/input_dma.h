#ifndef INPUT_DMA_H
#define INPUT_DMA_H

#include "input_source.h"

typedef struct {
    const char *device_path;
    int fd;
} dma_input_ctx_t;

int dma_input_open(input_source_t *src);
ssize_t dma_input_read(input_source_t *src, uint8_t *dst, size_t max_len);
void dma_input_close(input_source_t *src);

#endif