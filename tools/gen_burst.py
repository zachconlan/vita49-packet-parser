import sys
import struct
import random
import argparse
import time

DEFAULT_NUM_PACKETS = 100000

def make_packet(seq):
    return struct.pack("!BBHIIIIhh",
        0, 0, 6,
        2,
        seq,
        int(time.time()),
        0,
        1000, -1000
    )

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--num-packets",
        type=int,
        default=DEFAULT_NUM_PACKETS,
        help="Number of packet opportunities to generate.",
    )
    args = parser.parse_args()

    seq = 0

    for _ in range(args.num_packets):

        # Simulate loss
        if random.random() < 0.1:
            seq += 1
            continue

        # Simulate burst loss
        if random.random() < 0.05:
            seq += 5

        pkt = make_packet(seq)
        sys.stdout.buffer.write(pkt)

        seq += 1

    sys.stdout.buffer.flush()

if __name__ == "__main__":
    main()

# To run: python3 gen_burst.py --num-packets 100000 > burst_test.bin
# ./main < burst_test.bin
