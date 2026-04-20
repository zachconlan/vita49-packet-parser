import sys
import struct
import argparse

def make_packet(seq):
    return struct.pack("!BBHIIIIhh",
        0, 0, 6,
        2,
        seq,
        0,
        0,
        seq % 32768,
        -(seq % 32768)
    )

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--num-packets",
        type=int,
        default=0,
        help="Number of packets to emit. 0 means infinite stream.",
    )
    args = parser.parse_args()

    seq = 0

    while args.num_packets == 0 or seq < args.num_packets:
        sys.stdout.buffer.write(make_packet(seq))
        seq += 1

    sys.stdout.buffer.flush()

if __name__ == "__main__":
    main()

# To run: python3 gen_highrate.py --num-packets 100000 | ./main
