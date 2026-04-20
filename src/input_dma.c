#include "input_dma.h"
#include <unistd.h>
#include <fcntl.h>

int dma_input_open(input_source_t *src)
{
    dma_input_ctx_t *ctx = (dma_input_ctx_t *)src->ctx;
    ctx->fd = open(ctx->device_path, O_RDONLY);
    return (ctx->fd < 0) ? -1 : 0;
}

ssize_t dma_input_read(input_source_t *src, uint8_t *dst, size_t max_len)
{
    dma_input_ctx_t *ctx = (dma_input_ctx_t *)src->ctx;
    return read(ctx->fd, dst, max_len);
}

void dma_input_close(input_source_t *src)
{
    dma_input_ctx_t *ctx = (dma_input_ctx_t *)src->ctx;
    if (ctx->fd >= 0)
        close(ctx->fd);
}