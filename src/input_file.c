#include "input_file.h"
#include <string.h>

int file_input_open(input_source_t *src)
{
    file_input_ctx_t *ctx = (file_input_ctx_t *)src->ctx;

    if (ctx->filename == NULL) {
        ctx->fp = stdin;
        return 0;
    }

    ctx->fp = fopen(ctx->filename, "rb");
    return (ctx->fp == NULL) ? -1 : 0;
}

ssize_t file_input_read(input_source_t *src, uint8_t *dst, size_t max_len)
{
    file_input_ctx_t *ctx = (file_input_ctx_t *)src->ctx;
    size_t n = fread(dst, 1, max_len, ctx->fp);

    if (n > 0)
        return (ssize_t)n;

    if (feof(ctx->fp))
        return 0;

    return -1;
}

void file_input_close(input_source_t *src)
{
    file_input_ctx_t *ctx = (file_input_ctx_t *)src->ctx;

    if (ctx->fp != NULL && ctx->fp != stdin) {
        fclose(ctx->fp);
    }
}