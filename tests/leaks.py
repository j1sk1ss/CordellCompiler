import re
import argparse

parser: argparse.ArgumentParser = argparse.ArgumentParser(description='Compiler leak finder!')
parser.add_argument('--log-path', required=True, help='Path to log file with memory logs')
args = parser.parse_args()

alloc_re = re.compile(
    r'\[MEM\]\s*\((?P<src_file>.*?):(?P<src_line>\d+)\)\s*'
    r'Allocation in (?P<file>.*?) on line=(?P<line>\d+), '
    r'size=(?P<size>\d+), ptr=(?P<ptr>0x[0-9A-Fa-f]+)'
)

free_re = re.compile(
    r'\[MEM\]\s*\((?P<src_file>.*?):(?P<src_line>\d+)\)\s*'
    r'Trying to free ptr=(?P<ptr>0x[0-9A-Fa-f]+)\s*from file=(?P<file>.*?), line=(?P<line>\d+)'
)

allocations: dict = {}
leaks: dict = {}

with open(args.log_path, "r", encoding="utf-8") as f:
    print("Reading file...")
    for line_number, line in enumerate(f, start=1):
        alloc_match = alloc_re.search(line)
        if alloc_match:
            d = alloc_match.groupdict()
            address = d["ptr"]
            size = int(d["size"])
            file = d["file"]
            line_no = d["line"]

            allocations[address] = (size, file, line_no, line_number)
            leaks[address] = (size, file, line_no, line_number)
            continue

        free_match = free_re.search(line)
        if free_match:
            d = free_match.groupdict()
            address = d["ptr"]
            if address in leaks:
                del leaks[address]
            continue

if leaks:
    print("Found next memory leaks:")
    for addr, (size, file, line_no, log_line_no) in leaks.items():
        print(
            f"Addr: {addr}, Size: {size}, "
            f"Allocated at: {file}:{line_no}, Log line: {log_line_no}"
        )
else:
    print("There is no leaks in log!")
