import re
import os
import sys
import json
import difflib
import argparse
import tempfile
import subprocess

from tqdm import tqdm
from pathlib import Path

from misc.builder import (
    CCBuilder,
    BuildCompiler,
    CCBuilderConfig
)

def _line_matches(expected_line: str, actual_line: str) -> bool:
    if expected_line.strip() == "{X}":
        return True

    if "{X}" in expected_line:
        parts = expected_line.split("{X}")
        pattern = "^" + ".*".join(re.escape(p) for p in parts) + "$"
        return re.match(pattern, actual_line) is not None

    return expected_line == actual_line

def _is_anywhere_line(line: str) -> bool:
    stripped = line.strip()
    return stripped.startswith('{') and stripped.endswith('}') and stripped != "{X}"

def _extract_anywhere_pattern(line: str) -> str:
    stripped = line.strip()
    return stripped[1:-1]

def _line_contains_pattern(pattern: str, actual_line: str) -> bool:
    if "{X}" in pattern:
        parts = pattern.split("{X}")
        escaped_parts = [re.escape(p) for p in parts]
        inner_re = ".*".join(escaped_parts)
        pattern_re = ".*" + inner_re + ".*"
        return re.search(pattern_re, actual_line) is not None
    else:
        return pattern in actual_line

def _matches_expected(expected_text: str, actual_text: str) -> tuple[bool, str | None]:
    exp_lines = expected_text.splitlines()
    act_lines = actual_text.splitlines()

    anywhere_patterns = []
    positional_expected = []
    for e in exp_lines:
        if _is_anywhere_line(e):
            anywhere_patterns.append(_extract_anywhere_pattern(e))
        else:
            positional_expected.append(e)

    if anywhere_patterns:
        for pattern in anywhere_patterns:
            if not any(_line_contains_pattern(pattern, a) for a in act_lines):
                return False, f"Anywhere pattern not found: {pattern}"

        act_idx = 0
        for pos_line in positional_expected:
            while act_idx < len(act_lines) and not _line_matches(pos_line, act_lines[act_idx]):
                act_idx += 1
            if act_idx >= len(act_lines):
                return False, f"Positional line not matched: {pos_line}"
            act_idx += 1
        return True, None
    else:
        if len(exp_lines) != len(act_lines):
            return False, f"Different number of lines: expected {len(exp_lines)}, actual {len(act_lines)}"

        for i, (e, a) in enumerate(zip(exp_lines, act_lines), start=1):
            if not _line_matches(e, a):
                return False, f"Line {i} mismatch:\n  expected: {e}\n  actual:   {a}"

        return True, None

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

def _normalize_output(text: str) -> str:
    lines = []
    for line in text.splitlines():
        line = line.rstrip()
        if line:
            lines.append(line)
    return "\n".join(lines)

def _parse_test_file(path: Path) -> tuple[str, str, dict]:
    text = path.read_text(encoding="utf-8")
    flags = {
        "test_debug": False,
        "block_test": False,
        "bug":        False,
    }

    lines = text.splitlines()
    header_processed = []
    for line in lines:
        stripped = line.strip()
        if stripped == ": TEST_DEBUG :":
            flags["test_debug"] = True
            continue
        elif stripped == ": BLOCK_TEST :":
            flags["block_test"] = True
            continue
        elif stripped == ": BUG :":
            flags["bug"] = True
            continue
        header_processed.append(line)

    text = "\n".join(header_processed)

    marker = ": OUTPUT"
    if marker not in text:
        raise ValueError(f"{path}: OUTPUT block not found")

    before, after = text.split(marker, 1)
    after = after.lstrip()

    if not after.endswith(":"):
        raise ValueError(f"{path}: OUTPUT block must end with ':'")

    expected = after[:-1].rstrip()
    return before.rstrip(), expected, flags

def _run_test(binary: str, test_file: Path) -> dict:
    code, expected, flags = _parse_test_file(test_file)
    with tempfile.NamedTemporaryFile(mode="w", suffix=".cpl", encoding="utf-8", delete=False) as tmp:
        tmp.write(code)
        tmp_path = tmp.name

    try:
        cmd = [ binary, tmp_path, str(test_file.parent) ]
        if flags["test_debug"]:
            debugger = "gdb"
            if sys.platform == "darwin":
                debugger = "lldb"

            if debugger == "gdb":
                cmd = [ debugger, "--batch", "-ex", "run", "--args" ] + cmd
            else:
                cmd = [ debugger, "--batch", "-o", "run", "--" ] + cmd

        proc = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )
    except Exception as ex:
        os.remove(tmp_path)
        return {
            "file":     str(test_file),
            "ok":       False,
            "critical": True,
            "warning":  False,
            "diff":     f"Subprocess error: {ex}",
        }
    finally:
        if os.path.exists(tmp_path):
            os.remove(tmp_path)

    expected_n: str = _normalize_output(expected)
    actual_n: str = _normalize_output(proc.stdout)
    ok, why = _matches_expected(expected_n, actual_n)

    critical: bool = False
    warning: bool = False

    if not ok:
        if flags["bug"]:
            warning = True
        elif flags["block_test"]:
            critical = True

    return {
        "file": str(test_file),
        "expected": expected_n,
        "actual": actual_n,
        "ok": ok,
        "critical": critical,
        "warning": warning,
        "diff": None if ok else (why + "\n\n" + _make_diff(expected_n, actual_n))
    }

def find_test_roots(start_path: Path) -> list[Path]:
    roots = []
    for root, _, _ in os.walk(start_path):
        root_path = Path(root)
        if (root_path / "base.c").is_file() and (root_path / "dependencies.json").is_file():
            roots.append(root_path)

    return roots

def collect_cpl_files(root: Path, all_roots: set[Path]) -> list[Path]:
    cpl_files = []
    try:
        for entry in root.iterdir():
            if entry.is_dir():
                if entry in all_roots:
                    continue
                cpl_files.extend(collect_cpl_files(entry, all_roots))
            elif entry.suffix == ".cpl":
                cpl_files.append(entry)
    except PermissionError:
        pass
    return cpl_files

def _entry() -> None:
    parser = argparse.ArgumentParser(description='Compiler simple unit testing script')
    parser.add_argument('--output-dir', default='./bin')
    parser.add_argument('--path', required=True, help='Unit test path')
    parser.add_argument('--base', default='../')
    parser.add_argument('--compiler', default='gcc')
    args = parser.parse_args()

    test_top: Path = Path(args.path)
    if not test_top.is_dir():
        print(f"Error: Test directory '{args.path}' wasn't found", file=sys.stderr)
        sys.exit(1)

    os.makedirs(args.output_dir, exist_ok=True)

    all_roots = find_test_roots(test_top)
    if not all_roots:
        print(f"No test modules found (no base.c + dependencies.json) under {test_top}", file=sys.stderr)
        sys.exit(1)

    all_roots_set: set[Path] = set(all_roots)
    results: list[dict] = []
    failed_modules: int = 0

    for root in all_roots:
        rel: Path = root.relative_to(test_top)
        module_out_dir: Path = Path(args.output_dir) / rel
        os.makedirs(module_out_dir, exist_ok=True)

        deps = root / "dependencies.json"
        base = root / "base.c"

        with deps.open("r", encoding="utf-8") as f:
            bconf_raw = json.load(f)

        bconf: CCBuilderConfig = CCBuilderConfig(
            include=args.base,
            target="unit_test",
            config=bconf_raw,
            compiler=BuildCompiler(args.compiler)
        )

        builder: CCBuilder = CCBuilder(settings=bconf)
        binary: str | None = builder.build(
            test_file=str(base),
            output_dir=str(module_out_dir),
            extra_flags=['Wno-int-conversion', 'Wno-unused-function']
        )

        if not binary:
            print(f"Failed to build module {root}, skipping its tests.", file=sys.stderr)
            failed_modules += 1
            continue

        cpl_files = collect_cpl_files(root, all_roots_set)
        if not cpl_files:
            print(f"Module {root} has no .cpl files to test.")

        for cpl in tqdm(cpl_files, desc=f"Module {rel}", leave=False):
            results.append(_run_test(binary, cpl))

    failed: int = 0
    critical_failed: bool = False
    for r in results:
        if r.get("ok", False):
            print(f"Succeed: {r['file']}")
        else:
            failed += 1
            if r.get("warning", False):
                print(f"\nWarning (BUG): {r['file']}")
                print(r["diff"])
            elif r.get("critical", False):
                print(f"\nCRITICAL (BLOCK_TEST): {r['file']}")
                print(r["diff"])
                critical_failed = True
            else:
                print(f"\nFailed: {r['file']}")
                print(r["diff"])

    print(f"\nSummary: {len(results)} tests run, {failed} failed, {failed_modules} modules failed to build.")
    if critical_failed:
        print("\nCritical failure detected. Stopping immediately.")
        sys.exit(2)

    if failed > 0 or failed_modules > 0:
        sys.exit(1)

if __name__ == "__main__":
    _entry()
