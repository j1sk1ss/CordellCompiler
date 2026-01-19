import difflib

def _make_diff(expected: str, actual: str) -> str:
    expected_lines = expected.splitlines()
    actual_lines = actual.splitlines()

    diff = difflib.unified_diff(
        expected_lines,
        actual_lines,
        fromfile="expected",
        tofile="actual",
        lineterm=""
    )

    return "\n".join(diff)

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

def _normalize_output(text: str) -> str:
    lines = []
    for line in text.splitlines():
        line = line.rstrip()
        if line:
            lines.append(line)
    return "\n".join(lines)

def _parse_test_file(path: Path) -> tuple[str, str]:
    text = path.read_text(encoding="utf-8")
    marker = ": OUTPUT"
    if marker not in text:
        raise ValueError(f"{path}: OUTPUT block not found")

    before, after = text.split(marker, 1)
    after = after.lstrip()
    if not after.endswith(":"):
        raise ValueError(f"{path}: OUTPUT block must end with ':'")

    expected = after[:-1].rstrip()
    return before.rstrip(), expected

def _run_test(binary: str, test_file: Path) -> dict:
    _, expected = _parse_test_file(test_file)
    proc = subprocess.run(
        [binary, str(test_file), str(test_file.parent)],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    expected_n = _normalize_output(expected)
    actual_n = _normalize_output(proc.stdout)
    ok = expected_n == actual_n

    return {
        "file": str(test_file),
        "expected": expected_n,
        "actual": actual_n,
        "ok": ok,
        "diff": None if ok else _make_diff(expected_n, actual_n)
    }

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
    base = test_dir / "base.c"

    if not deps.exists() or not base.exists():
        print(f"Error: Test directory '{args.path}' isn't correct", file=sys.stderr)
        sys.exit(1)

    with deps.open("r", encoding="utf-8") as f:
        bconf_raw = json.load(f)

    bconf = CCBuilderConfig(
        include=args.base,
        target="unit_test",
        config=bconf_raw,
        compiler=BuildCompiler(args.compiler)
    )

    builder = CCBuilder(settings=bconf)
    binary = builder.build(
        test_file=str(base),
        output_dir=args.output_dir
    )

    if not binary:
        sys.exit(1)

    results = []
    for cpl_file in test_dir.glob("*.cpl"):
        results.append(_run_test(binary, cpl_file))

    failed = 0
    for r in results:
        if r["ok"]:
            print(f"Succeed: {r['file']}")
        else:
            failed += 1
            print(f"\nFailed: {r['file']}")
            print(r["diff"])

if __name__ == "__main__":
    _entry()
