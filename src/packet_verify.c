#include "packet_verify.h"
#include <string.h>
#include <arpa/inet.h>

int verify_packet(uint8_t *data, size_t len, verify_state_t *state)
{
    if(len < sizeof(vita_hdr_t))
        return 0;

    vita_hdr_t hdr;
    memcpy(&hdr, data, sizeof(hdr));

    uint16_t words = ntohs(hdr.words);
    uint32_t seq   = ntohl(hdr.seq);

    size_t pkt_bytes = words * 4;
    if(pkt_bytes < sizeof(vita_hdr_t))
        return -1;

    if(len < pkt_bytes)
        return 0; // incomplete packet

    // Sequence check
    if(seq != state->expected_seq)
    {
        state->packet_loss++;
        state->expected_seq = seq + 1;
    }
    else
    {
        state->expected_seq++;
    }

    return pkt_bytes;
}
