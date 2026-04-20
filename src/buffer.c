#include "buffer.h"

#include <stdlib.h>
#include <string.h>

int buffer_init(ring_buffer_t *buf, size_t size)
{
    memset(buf, 0, sizeof(*buf));

    if (posix_memalign((void**)&buf->data, 64, size))
        return -1;

    buf->size = size;
    return 0;
}

size_t buffer_write_ptr(ring_buffer_t *buf, uint8_t **ptr)
{
    size_t free = buf->size - buf->count;

    if (!free)
        return 0;

    *ptr = buf->data + buf->head;

    size_t contig = buf->size - buf->head;

    if (contig > free)
        contig = free;

    return contig;
}

void buffer_produce(ring_buffer_t *buf, size_t bytes)
{
    buf->head = (buf->head + bytes) % buf->size;
    buf->count += bytes;
}

void buffer_consume(ring_buffer_t *buf, size_t bytes)
{
    buf->tail = (buf->tail + bytes) % buf->size;
    buf->count -= bytes;
}

size_t buffer_peek(ring_buffer_t *buf, uint8_t **ptr)
{
    if (!buf->count)
        return 0;

    *ptr = buf->data + buf->tail;

    size_t contig = buf->size - buf->tail;

    if (contig > buf->count)
        contig = buf->count;

    return contig;
}