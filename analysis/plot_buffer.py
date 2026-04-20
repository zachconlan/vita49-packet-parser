# import csv
# import matplotlib.pyplot as plt

# samples = []
# counts = []
# percents = []

# with open("buffer_log.csv", "r", newline="") as f:
#     reader = csv.DictReader(f)
#     for row in reader:
#         samples.append(int(row["sample"]))
#         counts.append(int(row["count"]))
#         percents.append(float(row["percent_full"]))

# plt.figure(figsize=(8, 4))
# plt.plot(samples, counts)
# plt.xlabel("Monitoring Sample")
# plt.ylabel("Buffer Occupancy (bytes)")
# plt.title("Buffer Occupancy Over Time")
# plt.grid(True)
# plt.tight_layout()
# plt.savefig("buffer_occupancy.png", dpi=300)
# plt.show()
# import re
# import matplotlib.pyplot as plt

# samples = []
# losses = []

# pattern = re.compile(r"loss=(\d+)")

# with open("burst_run.txt", "r") as f:
#     idx = 0
#     for line in f:
#         match = pattern.search(line)
#         if match:
#             losses.append(int(match.group(1)))
#             samples.append(idx)
#             idx += 1

# plt.figure(figsize=(8, 4))
# plt.plot(samples, losses)
# plt.xlabel("Monitoring Sample")
# plt.ylabel("Cumulative Packet Loss")
# plt.title("Detected Packet Loss Over Time")
# plt.grid(True)
# plt.tight_layout()
# plt.savefig("packet_loss_over_time.png", dpi=300)
# plt.show()
import csv
import matplotlib.pyplot as plt

seq = []

with open("seq_log.csv", "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        seq.append(int(row["seq"]))

# Compute delta between consecutive sequence numbers
delta = [seq[i] - seq[i-1] for i in range(1, len(seq))]
samples = list(range(len(delta)))

plt.figure(figsize=(8,4))
plt.plot(samples[:1000], delta[:1000])  # zoom first 1000 points
plt.axhline(1)

plt.xlabel("Packet Index")
plt.ylabel("Sequence Increment")
plt.title("Packet Sequence Integrity (Zoomed View)")
plt.grid(True)

plt.tight_layout()
plt.savefig("sequence_integrity.png", dpi=300)
plt.show()
