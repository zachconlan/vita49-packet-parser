# High-Throughput VITA-49 Packet Parser

A high-performance C-based packet parser and validation framework for VITA-49-style streaming data.  
This project simulates and validates real-time data pipelines using a ring buffer, packet sequence tracking, and modular input sources.

---

## Features

- High-throughput ring buffer for continuous data ingestion  
- Packet validation with sequence tracking and loss detection  
- Modular input abstraction (stdin, file, or device interface)  
- Python-based traffic generators for testing:
  - Clean streams  
  - Packet loss  
  - Corruption  
  - Burst loss  
  - High-rate stress testing  
- Performance monitoring of packets, bytes, and buffer usage  

---

## System Overview

```
Python Generators --> Input Source --> Ring Buffer --> Packet Parser --> Monitor
                         |
                  (stdin / file / device)
```

---

## Project Structure

```
vita49-parser/
├── src/
│   ├── main.c
│   ├── buffer.c
│   ├── packet_verify.c
│   ├── stream_monitor.c
│   ├── input_file.c
│   └── input_dma.c
├── include/
│   ├── buffer.h
│   ├── packet_verify.h
│   ├── stream_monitor.h
│   ├── input_file.h
│   ├── input_dma.h
│   └── input_source.h
├── tools/
│   ├── gen_clean_stream.py
│   ├── gen_packet_loss.py
│   ├── gen_burst.py
│   ├── gen_corrupt.py
│   └── gen_highrate.py
├── analysis/
│   └── plot_buffer.py
├── Makefile
└── README.md
```

---

## Build

Build the parser:

```bash
make build
```

This produces:

```
main_test
```

Or compile manually:

```bash
gcc -O3 -Wall -Wextra -Iinclude \
  src/main.c src/buffer.c src/packet_verify.c \
  src/stream_monitor.c src/input_file.c src/input_dma.c \
  -o main_test
```

---

## Run

### From stdin

```bash
python3 tools/gen_clean_stream.py --num-packets 5000 --sleep 0 | ./main_test
```

### From a file

```bash
./main_test stream.bin
```

### From a device

```bash
./main_test --device /dev/your_device
```

---

## Automated Tests

Run all tests:

```bash
make test-all
```

Run individual tests:

```bash
make test-clean
make test-loss
make test-corrupt
make test-burst
make test-highrate
```

Override defaults:

```bash
make test-all PACKETS=20000 SLEEP=0 OUT_DIR=/tmp/parser_run
```

Defaults:
- `PACKETS = 5000`  
- `SLEEP = 0`  
- `OUT_DIR = /tmp/vita49_parser_tests`  

---

## Example Output

```
Summary: packets=5000 bytes=120000 loss=0 max_buf=119976 remaining=0
```

- `packets` – successfully parsed packets  
- `bytes` – total processed data  
- `loss` – detected sequence discontinuities  
- `max_buf` – peak buffer occupancy  
- `remaining` – bytes left unparsed at end  

---

## Components

### Ring Buffer
Implements a circular buffer using head/tail indexing to support continuous high-throughput data ingestion.

### Packet Verification
Parses packet headers and validates packet size and sequence continuity to detect loss or corruption.

### Input Abstraction
Supports multiple input sources:
- standard input (piped streams)
- binary files
- device-backed reads

### Monitoring
Tracks total packets, bytes processed, and buffer behavior during execution.

### Traffic Generators
Python scripts simulate realistic streaming scenarios for testing system robustness.

---

## Design Highlights

- Efficient ring-buffer-based ingestion for streaming workloads  
- Sequence-aware packet validation for real-time loss detection  
- Modular architecture separating ingestion, parsing, and monitoring  
- Reproducible testing using Python-based generators  

---

## Note

This repository contains independently developed software components for packet parsing and validation.  
It is a simplified, non-proprietary implementation intended for academic and demonstration purposes.

---

## Use Cases

- streaming data pipeline validation  
- SDR and signal-processing software prototypes  
- packet integrity testing  
- embedded and systems software demonstrations  

---

## Author

Zachary Conlan  
UT Austin — Electrical & Computer Engineering
