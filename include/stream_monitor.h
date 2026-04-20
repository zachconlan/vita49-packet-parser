#ifndef STREAM_MONITOR_H
#define STREAM_MONITOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t total_bytes;
    uint64_t total_packets;
    uint64_t overruns;
    uint64_t underruns;
} monitor_t;

void monitor_init(monitor_t *m);
void monitor_packet(monitor_t *m, size_t bytes);
void monitor_buffer(monitor_t *m, size_t count, size_t size);

#endif