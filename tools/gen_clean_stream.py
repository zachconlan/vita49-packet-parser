import sys
import struct
import time
import argparse

STREAM_ID_L2 = 0x00000002
STREAM_ID_L5 = 0x00000005

def make_packet(seq, stream_id, I, Q):
    pkt_type = 0
    flags = 0
    words = 6  # header + 1 IQ pair (in 32-bit words)

    header = struct.pack(
        "!BBHIIII",
        pkt_type,
        flags,
        words,
        stream_id,
        seq,
        int(time.time()),
        0
    )

    iq_data = struct.pack("!hh", I, Q)

    return header + iq_data

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--num-packets",
        type=int,
        default=0,
        help="Number of packets to emit. 0 means infinite stream.",
    )
    parser.add_argument(
        "--sleep",
        type=float,
        default=0.001,
        help="Seconds to sleep between packets.",
    )
    args = parser.parse_args()

    seq = 0
    stream_id = STREAM_ID_L2  # change to L5 for other test
    sent = 0

    while args.num_packets == 0 or sent < args.num_packets:
        pkt = make_packet(seq, stream_id, 1000, -1000)
        sys.stdout.buffer.write(pkt)
        sys.stdout.buffer.flush()

        seq += 1
        sent += 1
        if args.sleep > 0:
            time.sleep(args.sleep)

if __name__ == "__main__":
    main()
    
# To run: python3 gen_clean_stream.py | ./main
