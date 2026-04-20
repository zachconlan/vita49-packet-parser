#include "stream_monitor.h"
#include <stdio.h>

void monitor_init(monitor_t *m)
{
    m->total_bytes = 0;
    m->total_packets = 0;
    m->overruns = 0;
    m->underruns = 0;
}

void monitor_packet(monitor_t *m, size_t bytes)
{
    m->total_packets++;
    m->total_bytes += bytes;
}

void monitor_buffer(monitor_t *m, size_t count, size_t size)
{
    if(count == size)
        m->overruns++;

    if(count == 0)
        m->underruns++;

    // Optional debug output
    printf("Buffer: %zu/%zu bytes\n", count, size);
}