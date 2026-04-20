#include "buffer.h"
#include "packet_verify.h"
#include "stream_monitor.h"
#include "input_source.h"
#include "input_file.h"
#include "input_dma.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#define BUF_SIZE   (64 * 1024 * 1024)
#define READ_CHUNK (64 * 1024)

int main(int argc, char **argv)
{
    ring_buffer_t buf;
    verify_state_t verify = {0};
    monitor_t monitor;

    input_source_t src;
    file_input_ctx_t file_ctx;
    dma_input_ctx_t dma_ctx;

    const char *input_name = "stdin";
    bool read_error = false;
    bool input_eof = false;

    if (argc > 3) {
        fprintf(stderr, "Usage: %s [dump.bin] | --device <devpath>\n", argv[0]);
        return 1;
    }

    memset(&file_ctx, 0, sizeof(file_ctx));
    memset(&dma_ctx, 0, sizeof(dma_ctx));

    if (argc == 1) {
        input_name = "stdin";

        file_ctx.filename = NULL;
        file_ctx.fp = NULL;

        src.open  = file_input_open;
        src.read  = file_input_read;
        src.close = file_input_close;
        src.ctx   = &file_ctx;
    }
    else if (argc == 2) {
        input_name = argv[1];

        file_ctx.filename = argv[1];
        file_ctx.fp = NULL;

        src.open  = file_input_open;
        src.read  = file_input_read;
        src.close = file_input_close;
        src.ctx   = &file_ctx;
    }
    else if (argc == 3 && strcmp(argv[1], "--device") == 0) {
        input_name = argv[2];

        dma_ctx.device_path = argv[2];
        dma_ctx.fd = -1;

        src.open  = dma_input_open;
        src.read  = dma_input_read;
        src.close = dma_input_close;
        src.ctx   = &dma_ctx;
    }
    else {
        fprintf(stderr, "Usage: %s [dump.bin] | --device <devpath>\n", argv[0]);
        return 1;
    }

    if (src.open(&src) != 0) {
        perror(input_name);
        return 1;
    }

    if (buffer_init(&buf, BUF_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize buffer\n");
        src.close(&src);
        return 1;
    }

    monitor_init(&monitor);

    size_t max_buf = 0;
    int print_counter = 0;
    int buf_counter = 0;

    while (1)
    {
        if (!input_eof) {
            uint8_t *wptr;
            size_t space = buffer_write_ptr(&buf, &wptr);

            if (buf.count == buf.size) {
                printf("WARNING: BUFFER FULL\n");
            }

            if (space > 0) {
                size_t to_read = (space > READ_CHUNK) ? READ_CHUNK : space;
                ssize_t n = src.read(&src, wptr, to_read);

                if (n > 0) {
                    buffer_produce(&buf, (size_t)n);
                } else if (n == 0) {
                    input_eof = true;
                } else {
                    fprintf(stderr, "%s read error\n", input_name);
                    read_error = true;
                    break;
                }
            }
        }

        if (buf.count > max_buf)
            max_buf = buf.count;

        if (++buf_counter % 1000 == 0) {
            printf("Buffer: %zu/%zu (max=%zu)\n", buf.count, buf.size, max_buf);
        }

        uint8_t *ptr;
        size_t available = buffer_peek(&buf, &ptr);

        if (available == 0) {
            if (input_eof)
                break;
            continue;
        }

        int pkt_size = verify_packet(ptr, available, &verify);

        if (pkt_size == 0) {
            if (input_eof)
                break;
            continue;
        }
        else if (pkt_size < 0) {
            buffer_consume(&buf, 1);
            continue;
        }

        int16_t *iq = (int16_t *)(ptr + sizeof(vita_hdr_t));
        int16_t I = ntohs(iq[0]);
        int16_t Q = ntohs(iq[1]);

        if (++print_counter % 1000 == 0) {
            printf("I=%d Q=%d loss=%" PRIu64 "\n", I, Q, verify.packet_loss);
        }

        monitor_packet(&monitor, pkt_size);
        buffer_consume(&buf, pkt_size);
    }

    src.close(&src);

    if (read_error) {
        return 1;
    }

    printf(
        "Summary: packets=%" PRIu64 " bytes=%" PRIu64 " loss=%" PRIu64
        " max_buf=%zu remaining=%zu\n",
        monitor.total_packets,
        monitor.total_bytes,
        verify.packet_loss,
        max_buf,
        buf.count
    );

    if (buf.count > 0) {
        printf("Note: %zu trailing bytes left unparsed at end of input\n", buf.count);
    }

    return 0;
}
// To compile: gcc -O3 main.c buffer.c packet_verify.c stream_monitor.c input_file.c input_dma.c -o main
// To run: ./main dump.bin or ./main < dump.bin
