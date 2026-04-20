CC := gcc
CFLAGS := -O3 -Wall -Wextra -Iinclude

BIN := main_test

SRC := src/main.c \
	src/buffer.c \
	src/packet_verify.c \
	src/stream_monitor.c \
	src/input_file.c \
	src/input_dma.c

PYTHON := python3
PACKETS ?= 5000
SLEEP ?= 0
OUT_DIR ?= /tmp/vita49_parser_tests

.PHONY: all build test-all test-clean test-loss test-corrupt test-burst test-highrate test-rx0dump test-rx1dump clean

all: build

build: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

# ========================
# TESTS
# ========================

test-all: test-clean test-loss test-corrupt test-burst test-highrate test-rx0dump test-rx1dump

test-clean: build
	mkdir -p $(OUT_DIR)
	$(PYTHON) tools/gen_clean_stream.py --num-packets $(PACKETS) --sleep $(SLEEP) > $(OUT_DIR)/clean.bin
	./$(BIN) < $(OUT_DIR)/clean.bin | tee $(OUT_DIR)/clean.out

test-loss: build
	mkdir -p $(OUT_DIR)
	$(PYTHON) tools/gen_packet_loss.py --num-packets $(PACKETS) --sleep $(SLEEP) > $(OUT_DIR)/loss.bin
	./$(BIN) < $(OUT_DIR)/loss.bin | tee $(OUT_DIR)/loss.out

test-corrupt: build
	mkdir -p $(OUT_DIR)
	$(PYTHON) tools/gen_corrupt.py --num-packets $(PACKETS) --sleep $(SLEEP) > $(OUT_DIR)/corrupt.bin
	./$(BIN) < $(OUT_DIR)/corrupt.bin | tee $(OUT_DIR)/corrupt.out

test-burst: build
	mkdir -p $(OUT_DIR)
	$(PYTHON) tools/gen_burst.py --num-packets $(PACKETS) > $(OUT_DIR)/burst.bin
	./$(BIN) < $(OUT_DIR)/burst.bin | tee $(OUT_DIR)/burst.out

test-highrate: build
	mkdir -p $(OUT_DIR)
	$(PYTHON) tools/gen_highrate.py --num-packets $(PACKETS) > $(OUT_DIR)/highrate.bin
	./$(BIN) < $(OUT_DIR)/highrate.bin | tee $(OUT_DIR)/highrate.out

test-rx0dump: build
	mkdir -p $(OUT_DIR)
	./$(BIN) test_runs/rx0_dump.bin | tee $(OUT_DIR)/rx0_dump.out

test-rx1dump: build
	mkdir -p $(OUT_DIR)
	./$(BIN) test_runs/rx1_dump.bin | tee $(OUT_DIR)/rx1_dump.out

# ========================
# CLEAN
# ========================

clean:
	rm -f $(BIN)
	rm -rf $(OUT_DIR)