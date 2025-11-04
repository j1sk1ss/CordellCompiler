import os
import sys
import textwrap
import pyfiglet
import argparse
import subprocess

def _build_builder(include: str, builder: str, std: str, ltypes: str, compiler: str) -> str:
    build_seq: list = [
        compiler, f"-I{include}/include", builder, f"{std}/std/*", ltypes, "-DTARGINFO_BUILD", "-o", f"{builder}.out"
    ]
    
    try:
        subprocess.run(" ".join(build_seq), shell=True, check=True)
        print(f"Compilation succeeded: {builder}.out")
    except subprocess.CalledProcessError as e:
        print(f"Compilation failed: {builder}.out, e={e}")
        raise e
    
    return f"{builder}.out"

def _build_target_info(builder: str, arch: str, out: str) -> None:
    cmd: list = [
        builder, arch, f"{out}/{arch}.trgcpl"
    ]
    
    try:
        subprocess.run(" ".join(cmd), shell=True, check=True)
        print(f"Target file succeeded: {out}.trgcpl")
    except subprocess.CalledProcessError as e:
        print(f"Compilation failed: {out}.trgcpl, e={e}")
        raise e

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
    ascii_banner = pyfiglet.figlet_format("TargInfo builder tool")
    print(ascii_banner)
    
    parser = argparse.ArgumentParser(description="Target info file builder for CPL Compiler (CC)")
    
    parser.add_argument("--lir-types-location", type=str, default=".", help="Compiler LIR types location")
    parser.add_argument("--include-location", type=str, default=".", help="Compiler incldue location")
    parser.add_argument("--std-location", type=str, default=".", help="Compiler STD location")
    parser.add_argument("--builder-location", type=str, default=".", help="Compiler target file builder location")
    parser.add_argument("--save-location", type=str, default=".", help="Target file save location")
    parser.add_argument("--arch", type=str, default="Ivy_Bridge", help="Target ARCH, for example: Ivy_Bridge, Coffe_Lake")
    parser.add_argument("--compiler", type=str, default="gcc-14", help="Compiler for compilation.")
    
    args = parser.parse_args()
    if len(sys.argv) == 1:
        _print_welcome(parser=parser)
        exit(1)
        
    builder: str = _build_builder(
        include=args.include_location, builder=args.builder_location, std=args.std_location, 
        ltypes=args.lir_types_location, compiler=args.compiler
    )
    
    _build_target_info(builder=builder, arch=args.arch, out=args.save_location)
    os.remove(builder)
    