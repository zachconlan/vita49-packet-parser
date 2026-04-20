import sys
import struct
import random
import time
import argparse

def make_packet(seq):
    return struct.pack("!BBHIIIIhh",
        0, 0, 6,
        2,          # stream ID
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
    sent = 0

    while args.num_packets == 0 or sent < args.num_packets:
        # Randomly drop packets
        if random.random() < 0.1:
            seq += 1
            continue

        # Occasionally jump sequence (simulate loss burst)
        if random.random() < 0.05:
            seq += 5

        pkt = make_packet(seq)
        sys.stdout.buffer.write(pkt)
        sys.stdout.buffer.flush()

        seq += 1
        sent += 1
        if args.sleep > 0:
            time.sleep(args.sleep)

if __name__ == "__main__":
    main()

# To run: python3 gen_packet_loss.py --num-packets 10000 | ./main
