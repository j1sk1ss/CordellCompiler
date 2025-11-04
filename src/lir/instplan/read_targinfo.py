import sys
import struct
import textwrap
import pyfiglet
import argparse

def _print_target_info(path: str):
    TARGET_INFO_HEADER_FMT = "<32si"
    OP_INFO_FMT = "<ibbbbi fi b"

    TARGET_INFO_HEADER_SIZE = struct.calcsize(TARGET_INFO_HEADER_FMT)
    OP_INFO_SIZE = struct.calcsize(OP_INFO_FMT)

    with open(path, "rb") as f:
        data = f.read()

    if len(data) < TARGET_INFO_HEADER_SIZE:
        raise ValueError("File too small for target_info_t")

    name_bytes, op_count = struct.unpack_from(TARGET_INFO_HEADER_FMT, data, 0)
    name = name_bytes.split(b"\x00", 1)[0].decode("utf-8", errors="ignore")
    print(f"Target name  : {name}")
    print(f"Operation cnt: {op_count}")
    print("-" * 60)

    offset = TARGET_INFO_HEADER_SIZE
    for i in range(op_count):
        if offset + OP_INFO_SIZE > len(data):
            print(f"[!] Too small data for {i}")
            break
        
        (
            op,
            reads_memory,
            writes_memory,
            sets_flags,
            uses_flags,
            latency,
            throughput,
            issue_cost,
            commutative,
        ) = struct.unpack_from(OP_INFO_FMT, data, offset)

        print(
            f"[{i:03}] op={op:3d}, rm={reads_memory}, wm={writes_memory}, "
            f"sf={sets_flags}, rf={uses_flags}, lat={latency:3d}, th={throughput:.2f}, "
            f"cost={issue_cost:2d}, cmt={commutative}"
        )
        
        offset += OP_INFO_SIZE
        

def _print_welcome(parser: argparse.ArgumentParser) -> None:
    print("Welcome! Avaliable arguments:\n")
    help_text = parser.format_help()
    options_start = help_text.find("optional arguments:")
    if options_start != -1:
        options_text = help_text[options_start:]
        print(textwrap.indent(options_text, "    "))
    else:
        print(textwrap.indent(help_text, "    "))

if __name__ == "__main__":
    ascii_banner = pyfiglet.figlet_format("TargInfo reader tool")
    print(ascii_banner)
    
    parser = argparse.ArgumentParser(description="Target info file reader for CPL Compiler (CC)")
    parser.add_argument("--target-file-location", type=str, default=".", help="Compiler target file location")
    
    args = parser.parse_args()
    if len(sys.argv) == 1:
        _print_welcome(parser=parser)
        exit(1)
    
    _print_target_info(args.target_file_location)
    