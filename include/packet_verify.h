#ifndef PACKET_VERIFY_H
#define PACKET_VERIFY_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t pkt_type;
    uint8_t flags;
    uint16_t words;
    uint32_t stream_id;
    uint32_t seq;
    uint32_t ts_sec;
    uint32_t ts_frac;
} __attribute__((packed)) vita_hdr_t;

typedef struct {
    uint32_t expected_seq;
    uint64_t packet_loss;
} verify_state_t;

int verify_packet(uint8_t *data, size_t len, verify_state_t *state);

#endif