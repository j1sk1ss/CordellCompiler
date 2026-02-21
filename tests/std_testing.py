import os
import sys
import json
import argparse
import subprocess
from pathlib import Path

from misc.builder import (
    CCBuilder, 
    BuildCompiler, 
    CCBuilderConfig
)

def _run_test(binary: str) -> bool:
    proc = subprocess.run(
        [ binary ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    if proc.stderr:
        print(proc.stderr, end="")
    
    return proc.returncode == 0

def _entry() -> None:
    parser = argparse.ArgumentParser(description='Compiler simple unit testing script')
    parser.add_argument('--output-dir', default='./bin')
    parser.add_argument('--path', required=True, help='Unit test path')
    parser.add_argument('--base', default='../')
    parser.add_argument('--compiler', default='gcc')
    args = parser.parse_args()

    test_dir = Path(args.path)

    if not test_dir.is_dir():
        print(f"Error: Test directory '{args.path}' wasn't found", file=sys.stderr)
        sys.exit(1)

    os.makedirs(args.output_dir, exist_ok=True)

    deps = test_dir / "dependencies.json"
    if not deps.exists():
        print(f"Error: Test directory '{args.path}' isn't correct", file=sys.stderr)
        sys.exit(1)

    with deps.open("r", encoding="utf-8") as f:
        bconf_raw = json.load(f)

    bconf = CCBuilderConfig(
        include=args.base,
        target="std_test",
        config=bconf_raw,
        compiler=BuildCompiler(args.compiler)
    )

    builder = CCBuilder(settings=bconf)
    for binaries in test_dir.rglob("*.c"):
        binary = builder.build(
            test_file=str(binaries),
            output_dir=args.output_dir,
            show_logs=False
        )
        
        if not _run_test(binary):
            print(f"Test {binaries} run error!")
            exit(1)
        else:
            print(f"Test {binaries} success!")

if __name__ == "__main__":
    _entry()
