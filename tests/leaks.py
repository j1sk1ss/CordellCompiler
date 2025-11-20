import re

log_file: str = "test_lir.log"
alloc_re = re.compile(
    r'\[MEM\]\s*(?:\[(.*?)\])?\s*\((.*?):(\d+)\)\s*Allocated node\s*\[(0x[0-9a-fA-F]+)\]\s*with\s*\[(\d+)\]\s*size'
)

free_re = re.compile(
    r'\[MEM\]\s*(?:\[(.*?)\])?\s*\((.*?):(\d+)\)\s*Free\s*\[(0x[0-9a-fA-F]+)\](?:\s*with\s*\[(\d+)\]\s*size)?'
)

allocations = {}
leaks = {}

with open(log_file, "r", encoding="utf-8") as f:
    print("Reading file...")
    for line_number, line in enumerate(f, start=1):
        alloc_match = alloc_re.search(line)
        if alloc_match:
            tag, file, line_no, address, size = alloc_match.groups()
            allocations[address] = (int(size), file, line_no, line_number)
            leaks[address] = (int(size), file, line_no, line_number)
            continue
        
        free_match = free_re.search(line)
        if free_match:
            tag, file, line_no, address, size = free_match.groups()
            if address in leaks:
                del leaks[address]

if leaks:
    for addr, (size, file, line_no, log_line_no) in leaks.items():
        print(f"Addr: {addr}, Size: {size}, Allocated at: {file}:{line_no}, Log line: {log_line_no}")
else:
    print("There is no leaks in log!")
    