#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
} ring_buffer_t;

int buffer_init(ring_buffer_t *buf, size_t size);
size_t buffer_write_ptr(ring_buffer_t *buf, uint8_t **ptr);
void buffer_produce(ring_buffer_t *buf, size_t bytes);
void buffer_consume(ring_buffer_t *buf, size_t bytes);
size_t buffer_peek(ring_buffer_t *buf, uint8_t **ptr);

#endif