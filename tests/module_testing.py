import re
import os
import sys
import csv
import json
import difflib
import argparse
import tempfile
import subprocess
import signal
import time

from tqdm import tqdm
from pathlib import Path
from collections import Counter

def _cmd_to_str(cmd: list[object]) -> str:
    return " ".join(str(part) for part in cmd)


def _log(message: str) -> None:
    tqdm.write(str(message))


def _run_interactive_command(cmd: list[str]) -> int:
    _log("[DEBUG] interactive session started; Ctrl+C will be handled by the debugger, not by the Python runner")
    sys.stdout.flush()
    sys.stderr.flush()

    old_sigint = signal.getsignal(signal.SIGINT)
    try:
        signal.signal(signal.SIGINT, signal.SIG_IGN)
        return subprocess.run(cmd).returncode
    finally:
        signal.signal(signal.SIGINT, old_sigint)

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

def _token_counter(text: str) -> Counter:
    return Counter(text.split())

def _unordered_tokens_match(expected: str, actual: str) -> bool:
    return _token_counter(expected) == _token_counter(actual)

def _compile_special_pattern(pattern: str, anywhere: bool) -> tuple[re.Pattern, list[tuple[str, str]]]:
    parts: list[str] = []
    unordered_groups: list[tuple[str, str]] = []

    i = 0
    group_id = 0
    plen = len(pattern)

    while i < plen:
        if pattern.startswith("{X}", i):
            parts.append(".*?")
            i += 3
            continue

        if pattern.startswith("<<", i):
            j = pattern.find(">>", i + 2)
            if j == -1:
                parts.append(re.escape(pattern[i:]))
                break

            inner = pattern[i + 2:j]
            group_name = f"unordered_{group_id}"
            group_id += 1

            tail = pattern[j + 2:]
            has_tail = len(tail) > 0

            if has_tail:
                parts.append(f"(?P<{group_name}>.+?)")
            else:
                parts.append(f"(?P<{group_name}>.+)")

            unordered_groups.append((group_name, inner))
            i = j + 2
            continue

        parts.append(re.escape(pattern[i]))
        i += 1

    regex_body = "".join(parts)

    if anywhere:
        if pattern.rstrip().endswith(">>"):
            regex = re.compile(regex_body + r"$")
        else:
            regex = re.compile(regex_body)
    else:
        regex = re.compile(r"^" + regex_body + r"$")

    return regex, unordered_groups

def _match_special_pattern(pattern: str, actual_line: str, anywhere: bool) -> bool:
    regex, unordered_groups = _compile_special_pattern(pattern, anywhere=anywhere)

    if not unordered_groups:
        if anywhere:
            return regex.search(actual_line) is not None
        return regex.match(actual_line) is not None

    if anywhere:
        for m in regex.finditer(actual_line):
            ok = True
            for group_name, expected_inner in unordered_groups:
                if not _unordered_tokens_match(expected_inner, m.group(group_name)):
                    ok = False
                    break
            if ok:
                return True
        return False

    m = regex.match(actual_line)
    if not m:
        return False

    for group_name, expected_inner in unordered_groups:
        if not _unordered_tokens_match(expected_inner, m.group(group_name)):
            return False

    return True

def _line_matches(expected_line: str, actual_line: str) -> bool:
    return _match_special_pattern(expected_line, actual_line, anywhere=False)

def _line_contains_pattern(pattern: str, actual_line: str) -> bool:
    return _match_special_pattern(pattern, actual_line, anywhere=True)

def _matches_expected(expected_text: str, actual_text: str) -> tuple[bool, str | None]:
    exp_lines = expected_text.splitlines()
    act_lines = actual_text.splitlines()

    anywhere_patterns: list[str] = []
    plan: list[tuple[str, object]] = []

    in_block = False
    block_lines: list[str] = []

    for e in exp_lines:
        s = e.strip()

        if s == "#":
            if not in_block:
                in_block = True
                block_lines = []
            else:
                in_block = False
                plan.append(("block", block_lines))
                block_lines = []
            continue

        if _is_anywhere_line(e):
            anywhere_patterns.append(_extract_anywhere_pattern(e))
            continue

        if in_block:
            block_lines.append(e)
        else:
            plan.append(("pos", e))

    if in_block:
        return False, "Unclosed block marker '#': expected closing #"

    for pattern in anywhere_patterns:
        if not any(_line_contains_pattern(pattern, a) for a in act_lines):
            return False, f"Anywhere pattern not found: {pattern}"

    act_idx = 0
    for kind, payload in plan:
        if kind == "pos":
            pos_line: str = payload
            while act_idx < len(act_lines) and not _line_matches(pos_line, act_lines[act_idx]):
                act_idx += 1
            if act_idx >= len(act_lines):
                return False, f"Positional line not matched: {pos_line}"
            act_idx += 1
            continue

        if kind == "block":
            blines: list[str] = payload
            if not blines:
                continue

            used_indices: set[int] = set()
            matched_indices: list[int] = []

            for b in blines:
                found = None
                for j in range(act_idx, len(act_lines)):
                    if j in used_indices:
                        continue
                    if _line_matches(b, act_lines[j]):
                        found = j
                        break
                if found is None:
                    return False, f"Block line not matched: {b}"
                used_indices.add(found)
                matched_indices.append(found)

            act_idx = max(matched_indices) + 1
            continue

        return False, f"Internal error: unknown plan kind {kind}"

    if "<<ERROR>>" in actual_text:
        err_lines = [ln for ln in actual_text.splitlines() if "<<ERROR>>" in ln]
        msg = "Output contains <<ERROR>>:\n" + "\n".join(f"> {ln}" for ln in err_lines[:10])
        return False, msg

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

def _parse_output_annotations(expected_text: str) -> tuple[str, dict[str, str]]:
    annotations: dict[str, str] = {}
    kept_lines: list[str] = []

    for line in expected_text.splitlines():
        stripped = line.strip()
        if stripped.startswith("@"):
            body = stripped[1:].strip()
            if body:
                if "=" in body:
                    key, value = body.split("=", 1)
                    annotations[key.strip()] = value.strip()
                else:
                    annotations[body] = "true"
                continue
        kept_lines.append(line)

    return "\n".join(kept_lines).rstrip(), annotations

def _annotation_enabled(annotations: dict[str, str], key: str) -> bool:
    if key not in annotations:
        return False

    value = annotations[key].strip().lower()
    return value not in {"0", "false", "no", "off"}

def _parse_repeat_count(annotations: dict[str, str]) -> tuple[int | None, str | None]:
    if "repeat" not in annotations:
        return 1, None

    raw = annotations["repeat"].strip()
    try:
        repeat = int(raw)
    except ValueError:
        return None, f"Invalid @repeat value: {raw}"

    if repeat <= 0:
        return None, f"@repeat must be a positive integer, got {repeat}"

    return repeat, None

def _format_duration(seconds: float | None) -> str:
    if seconds is None:
        return "n/a"
    return f"{seconds:.6f}s"

def _count_text_lines(text: str) -> int:
    if not text:
        return 0
    return len(text.splitlines())

def _parse_output_case_blocks(expected_text: str) -> tuple[dict[int, dict[str, object]] | None, dict[str, str], str | None]:
    if "@case_index=" not in expected_text:
        return None, {}, None

    blocks_raw = re.split(r"(?m)^---\s*$", expected_text)
    cases: dict[int, dict[str, object]] = {}
    global_annotations: dict[str, str] = {}

    for block_raw in blocks_raw:
        block = block_raw.strip()
        if not block:
            continue

        block_expected, block_annotations = _parse_output_annotations(block)

        if "case_index" not in block_annotations:
            if block_expected.strip():
                return None, {}, "Every case block must contain @case_index=N"

            global_annotations.update(block_annotations)
            continue

        try:
            case_index = int(block_annotations["case_index"])
        except ValueError:
            return None, {}, f"Invalid @case_index value: {block_annotations['case_index']}"

        if case_index in cases:
            return None, {}, f"Duplicate @case_index={case_index}"

        cases[case_index] = {
            "expected": block_expected,
            "annotations": block_annotations,
        }

    return cases, global_annotations, None

def _check_single_case(expected_text: str, actual_text: str, annotations: dict[str, str], actual_exit_code: int | None) -> tuple[bool, str | None]:
    ok, why = _matches_expected(_normalize_output(expected_text), _normalize_output(actual_text))

    if not ok:
        return False, why

    if "exit_code" in annotations:
        if actual_exit_code is None:
            return False, "Annotation @exit_code is supported only for executable runs"

        try:
            expected_exit_code = int(annotations["exit_code"])
        except ValueError:
            return False, f"Invalid @exit_code value: {annotations['exit_code']}"

        if actual_exit_code != expected_exit_code:
            return False, f"Exit code mismatch: expected {expected_exit_code}, actual {actual_exit_code}"

    return True, None

def _rewrite_test_output(path: Path, actual_output: str) -> None:
    text = path.read_text(encoding="utf-8")

    lines = []
    for line in text.splitlines():
        if line.strip() == ": REWRITE :":
            continue
        lines.append(line)

    text_wo_rewrite = "\n".join(lines)

    marker = ":/ OUTPUT"
    if marker not in text_wo_rewrite:
        raise ValueError(f"{path}: OUTPUT block not found")

    before, _ = text_wo_rewrite.split(marker, 1)
    before = before.rstrip()
    actual_output = actual_output.rstrip()

    rewritten = f"{before}\n\n{marker}\n"
    if actual_output:
        rewritten += actual_output + "\n"
    rewritten += "/:"

    path.write_text(rewritten, encoding="utf-8")



def _split_unquoted(text: str, sep: str = "|") -> list[str]:
    parts: list[str] = []
    current: list[str] = []
    in_quotes = False
    i = 0

    while i < len(text):
        ch = text[i]

        if ch == '"':
            current.append(ch)
            if in_quotes and i + 1 < len(text) and text[i + 1] == '"':
                current.append(text[i + 1])
                i += 1
            else:
                in_quotes = not in_quotes
        elif ch == sep and not in_quotes:
            parts.append("".join(current).strip())
            current = []
        else:
            current.append(ch)

        i += 1

    parts.append("".join(current).strip())
    return parts

def _parse_run_asm_args(raw: str) -> list[str]:
    raw = raw.strip()
    if raw.endswith(","):
        raw = raw[:-1].rstrip()

    if not raw:
        return []

    return next(csv.reader([raw], skipinitialspace=True))

def _parse_run_asm_cases(spec: str) -> list[list[str]]:
    spec = spec.strip()
    if not spec:
        return [[]]

    cases: list[list[str]] = []
    for chunk in _split_unquoted(spec, sep="|"):
        if not chunk:
            continue

        if not chunk.startswith("args="):
            raise ValueError(f"Unsupported RUN_ASM option block: {chunk}")

        cases.append(_parse_run_asm_args(chunk[len("args="):]))

    return cases or [[]]

def _parse_run_asm_directive(line: str) -> tuple[bool, bool, list[list[str]]] | None:
    m = re.fullmatch(r":\s*(RUN_ASM|RUN_ASM_DEBUG)(?:\[(.*)\])?\s*:", line)
    if not m:
        return None

    kind = m.group(1)
    spec = m.group(2)

    return kind == "RUN_ASM", kind == "RUN_ASM_DEBUG", _parse_run_asm_cases(spec or "")

def _parse_test_file(path: Path) -> tuple[str, str, dict]:
    text = path.read_text(encoding="utf-8")
    flags = {
        "test_debug":    False,
        "block_test":    False,
        "bug":           False,
        "leak_trace":    False,
        "rewrite":       False,
        "run_asm":       False,
        "run_asm_debug": False,
        "run_asm_cases": [[]],
        "output_annotations": {},
        "output_case_blocks": None,
    }

    lines = text.splitlines()
    header_processed = []
    for line in lines:
        stripped = line.strip()

        run_asm_directive = _parse_run_asm_directive(stripped)
        if run_asm_directive:
            flags["run_asm"], flags["run_asm_debug"], flags["run_asm_cases"] = run_asm_directive
            continue

        if stripped == ": TEST_DEBUG :":
            flags["test_debug"] = True
            continue
        elif stripped == ": BLOCK_TEST :":
            flags["block_test"] = True
            continue
        elif stripped == ": BUG :":
            flags["bug"] = True
            continue
        if stripped == ": LEAK_TRACE :":
            flags["leak_trace"] = True
            continue
        if stripped == ": REWRITE :":
            flags["rewrite"] = True
            continue
        header_processed.append(line)

    text = "\n".join(header_processed)

    marker = ":/ OUTPUT"
    if marker not in text:
        raise ValueError(f"{path}: OUTPUT block not found")

    before, after = text.split(marker, 1)
    after = after.lstrip()

    if not after.endswith("/:"):
        raise ValueError(f"{path}: OUTPUT block must end with '/:'")

    raw_expected = after[:-2].rstrip()
    output_case_blocks, output_annotations, case_error = _parse_output_case_blocks(raw_expected)
    if case_error:
        raise ValueError(f"{path}: {case_error}")

    if output_case_blocks is not None:
        flags["output_case_blocks"] = output_case_blocks
        expected = raw_expected
        flags["output_annotations"] = output_annotations
    else:
        expected, output_annotations = _parse_output_annotations(raw_expected)
        flags["output_annotations"] = output_annotations

    return before.rstrip(), expected, flags

def _capture_process(cmd: list[str]) -> tuple[subprocess.CompletedProcess, float]:
    started_at = time.perf_counter()
    proc = subprocess.run(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )
    elapsed = time.perf_counter() - started_at
    return proc, elapsed


def _append_log_section(sections: list[tuple[str, str]], title: str, content: object) -> None:
    if content is None:
        text = ""
    else:
        text = str(content)
    sections.append((title, text.rstrip("\n")))


def _write_failure_log(test_file: Path, sections: list[tuple[str, str]]) -> str:
    log_path = test_file.with_suffix(test_file.suffix + ".full.log")
    lines: list[str] = []

    for index, (title, content) in enumerate(sections):
        if index > 0:
            lines.append("")
        lines.append(f"===== {title} =====")
        if content:
            lines.append(content)

    log_path.write_text("\n".join(lines).rstrip() + "\n", encoding="utf-8")
    return str(log_path)


def _attach_failure_log(result: dict, test_file: Path, sections: list[tuple[str, str]]) -> dict:
    if result.get("ok", False):
        return result

    _append_log_section(sections, "RESULT", json.dumps({
        "file": result.get("file"),
        "ok": result.get("ok"),
        "critical": result.get("critical"),
        "warning": result.get("warning"),
        "metrics": result.get("metrics"),
    }, ensure_ascii=False, indent=2))

    if result.get("diff"):
        _append_log_section(sections, "FAILURE REASON", result["diff"])

    log_path = _write_failure_log(test_file, sections)
    result["failure_log"] = log_path

    note = f"Full log: {log_path}"
    if result.get("diff"):
        result["diff"] = f"{result['diff']}\n\n{note}"
    else:
        result["diff"] = note

    return result


def _assemble_and_run(
    asm_text: str,
    debug: bool,
    runs: list[list[str]] | None = None,
    log_sections: list[tuple[str, str]] | None = None,
) -> tuple[bool, str | None, list[str] | None, list[int] | None, float | None]:
    with tempfile.TemporaryDirectory(prefix="cpl_asm_") as tmp_dir:
        tmp_dir_path = Path(tmp_dir)

        asm_path = tmp_dir_path / "program.asm"
        obj_path = tmp_dir_path / "program.o"
        exe_path = tmp_dir_path / "program.out"

        _log(f"[ASM] temporary asm: {asm_path}")
        _log(f"[ASM] object file: {obj_path}")
        _log(f"[ASM] executable path: {exe_path}")

        if log_sections is not None:
            _append_log_section(log_sections, "ASM PATHS", json.dumps({
                "asm_path": str(asm_path),
                "obj_path": str(obj_path),
                "exe_path": str(exe_path),
            }, ensure_ascii=False, indent=2))

        asm_path.write_text(asm_text, encoding="utf-8")

        if sys.platform == "darwin":
            nasm_format = "macho64"
        else:
            nasm_format = "elf64"

        nasm_cmd = ["nasm", "-f", nasm_format]
        if debug:
            nasm_cmd.append("-g")
        nasm_cmd.extend([str(asm_path), "-o", str(obj_path)])

        _log(f"[ASM] assembling command: {_cmd_to_str(nasm_cmd)}")
        nasm_proc, _ = _capture_process(nasm_cmd)
        if log_sections is not None:
            _append_log_section(log_sections, "ASM ASSEMBLE", "\n".join([
                f"command: {_cmd_to_str(nasm_cmd)}",
                f"exit_code: {nasm_proc.returncode}",
                "output:",
                nasm_proc.stdout.rstrip("\n"),
            ]))
        if nasm_proc.returncode != 0:
            return False, nasm_proc.stdout, None, None, None

        if sys.platform == "darwin":
            link_cmd = [
                "ld",
                "-e", "_main",
                "-macos_version_min", "10.13",
                "-lSystem",
                "-o", str(exe_path),
                str(obj_path)
            ]
        else:
            link_cmd = [
                "ld",
                "-o", str(exe_path),
                str(obj_path)
            ]

        _log(f"[ASM] linking command: {_cmd_to_str(link_cmd)}")
        link_proc, _ = _capture_process(link_cmd)
        if log_sections is not None:
            _append_log_section(log_sections, "ASM LINK", "\n".join([
                f"command: {_cmd_to_str(link_cmd)}",
                f"exit_code: {link_proc.returncode}",
                "output:",
                link_proc.stdout.rstrip("\n"),
            ]))
        if link_proc.returncode != 0:
            return False, link_proc.stdout, None, None, None

        run_cases = runs or [[]]

        if debug:
            debug_args = run_cases[0] if run_cases else []
            debugger = "gdb"
            if sys.platform == "darwin":
                debugger = "lldb"

            if debugger == "gdb":
                debug_cmd = [debugger, "--args", str(exe_path), *debug_args]
            else:
                debug_cmd = [debugger, "--", str(exe_path), *debug_args]

            _log(f"[ASM] starting debugger for executable: {exe_path}")
            _log(f"[ASM] debugger command: {_cmd_to_str(debug_cmd)}")
            if log_sections is not None:
                _append_log_section(log_sections, "ASM DEBUG", "\n".join([
                    f"command: {_cmd_to_str(debug_cmd)}",
                    "interactive debugger session was started",
                ]))
            _run_interactive_command(debug_cmd)
            return True, None, None, None, None

        outputs: list[str] = []
        exit_codes: list[int] = []
        program_elapsed = 0.0

        for run_index, run_args in enumerate(run_cases):
            run_cmd = [str(exe_path), *run_args]
            _log(f"[ASM] run #{run_index}: executable={exe_path} args={run_args}")
            run_proc, run_elapsed = _capture_process(run_cmd)
            outputs.append(run_proc.stdout.rstrip("\n"))
            exit_codes.append(run_proc.returncode)
            program_elapsed += run_elapsed
            if log_sections is not None:
                _append_log_section(log_sections, f"ASM RUN #{run_index}", "\n".join([
                    f"command: {_cmd_to_str(run_cmd)}",
                    f"exit_code: {run_proc.returncode}",
                    f"elapsed: {run_elapsed:.6f}s",
                    "output:",
                    run_proc.stdout.rstrip("\n"),
                ]))

        return True, "\n".join(outputs), outputs, exit_codes, program_elapsed


def _make_case_failure(case_index: int, case_expected: str, case_actual: str, case_reason: str) -> str:
    expected_n = _normalize_output(case_expected)
    actual_n = _normalize_output(case_actual)

    parts = [f"Case {case_index} failed: {case_reason}"]
    diff = _make_diff(expected_n, actual_n)
    if diff:
        parts.append("")
        parts.append(diff)

    return "\n".join(parts)

def _check_output_annotations(flags: dict, actual_exit_codes: list[int] | None) -> tuple[bool, str | None]:
    annotations = flags.get("output_annotations", {})

    if "case_index" in annotations:
        return False, "Global @case_index is not supported; use case blocks separated by ---"

    if "exit_code" in annotations:
        if actual_exit_codes is None:
            return False, "Annotation @exit_code is supported only for executable runs"

        try:
            expected_exit_code = int(annotations["exit_code"])
        except ValueError:
            return False, f"Invalid @exit_code value: {annotations['exit_code']}"

        if len(actual_exit_codes) != 1:
            return False, f"@exit_code expects exactly one run, got {len(actual_exit_codes)}"

        if actual_exit_codes[0] != expected_exit_code:
            return False, f"Exit code mismatch: expected {expected_exit_code}, actual {actual_exit_codes[0]}"

    return True, None

def _count_output_lines(expected: str, flags: dict) -> int:
    case_blocks = flags.get("output_case_blocks")
    if case_blocks is not None:
        total = 0
        for case_index in sorted(case_blocks):
            total += _count_text_lines(case_blocks[case_index]["expected"])
        return total

    return _count_text_lines(expected)

def _build_measure_info(
    measure_time: bool,
    test_elapsed: float,
    program_elapsed: float | None,
    measure_lines: bool,
    input_lines: int,
    output_lines: int
) -> str | None:
    parts: list[str] = []

    if measure_time:
        parts.append(f"test={_format_duration(test_elapsed)}")
        if program_elapsed is not None:
            parts.append(f"program={_format_duration(program_elapsed)}")

    if measure_lines:
        parts.append(f"input_lines={input_lines}")
        parts.append(f"output_lines={output_lines}")

    if not parts:
        return None

    return ", ".join(parts)

def _run_test_once(
    binary: str,
    binary_leak: str | None,
    test_file: Path,
    code: str,
    expected: str,
    flags: dict,
    measure_time: bool,
    measure_lines: bool,
    input_lines: int,
    initial_output_lines: int,
) -> dict:
    output_lines = initial_output_lines
    test_started_at = time.perf_counter()
    program_elapsed: float | None = None
    log_sections: list[tuple[str, str]] = []

    _append_log_section(log_sections, "TEST INFO", json.dumps({
        "test_file": str(test_file),
        "binary": str(binary),
        "binary_leak": str(binary_leak) if binary_leak else None,
        "measure_time": measure_time,
        "measure_lines": measure_lines,
        "input_lines": input_lines,
        "expected_output_lines": initial_output_lines,
    }, ensure_ascii=False, indent=2))
    _append_log_section(log_sections, "FLAGS", json.dumps(flags, ensure_ascii=False, indent=2, default=str))
    _append_log_section(log_sections, "SOURCE CODE", code)
    _append_log_section(log_sections, "EXPECTED OUTPUT", expected)

    _log(f"[TEST] starting: {test_file}")

    chosen_bin = binary
    if flags["leak_trace"]:
        if not binary_leak:
            test_elapsed = time.perf_counter() - test_started_at
            result = {
                "file": str(test_file),
                "ok": False,
                "critical": True,
                "warning": False,
                "diff": "LEAK_TRACE requested, but leak-instrumented binary was not built.",
                "metrics": _build_measure_info(measure_time, test_elapsed, None, measure_lines, input_lines, output_lines),
                "_test_elapsed": test_elapsed,
                "_program_elapsed": None,
                "_input_lines": input_lines,
                "_output_lines": output_lines,
            }
            return _attach_failure_log(result, test_file, log_sections)
        chosen_bin = binary_leak

    with tempfile.NamedTemporaryFile(mode="w", suffix=".cpl", encoding="utf-8", delete=False) as tmp:
        tmp.write(code)
        tmp_path = tmp.name

    _log(f"[TEST] temp source path: {tmp_path}")
    _log(f"[TEST] executable path: {chosen_bin}")
    _append_log_section(log_sections, "TEMP FILE", json.dumps({
        "tmp_source_path": tmp_path,
        "chosen_binary": str(chosen_bin),
    }, ensure_ascii=False, indent=2))

    try:
        actual_output = ""
        actual_outputs_by_run: list[str] | None = None
        actual_exit_codes: list[int] | None = None

        if flags["run_asm"] or flags["run_asm_debug"]:
            compile_cmd = [str(chosen_bin), str(tmp_path), str(test_file.parent)]
            _log(f"[TEST] compiler command: {_cmd_to_str(compile_cmd)}")
            compiler_proc, compiler_elapsed = _capture_process(compile_cmd)
            output_lines = _count_text_lines(compiler_proc.stdout)
            _append_log_section(log_sections, "COMPILER OUTPUT", "\n".join([
                f"command: {_cmd_to_str(compile_cmd)}",
                f"exit_code: {compiler_proc.returncode}",
                f"elapsed: {compiler_elapsed:.6f}s",
                "output:",
                compiler_proc.stdout.rstrip("\n"),
            ]))

            if compiler_proc.returncode != 0:
                actual_output = compiler_proc.stdout
            else:
                asm_ok, asm_output, actual_outputs_by_run, actual_exit_codes, program_elapsed = _assemble_and_run(
                    compiler_proc.stdout,
                    debug=flags["run_asm_debug"],
                    runs=flags["run_asm_cases"],
                    log_sections=log_sections,
                )

                if flags["run_asm_debug"] and asm_ok:
                    test_elapsed = time.perf_counter() - test_started_at
                    return {
                        "file": str(test_file),
                        "ok": True,
                        "critical": False,
                        "warning": False,
                        "diff": None,
                        "metrics": _build_measure_info(measure_time, test_elapsed, program_elapsed, measure_lines, input_lines, output_lines),
                        "_test_elapsed": test_elapsed,
                        "_program_elapsed": program_elapsed,
                        "_input_lines": input_lines,
                        "_output_lines": output_lines,
                    }

                actual_output = asm_output or ""
                if actual_outputs_by_run is None:
                    actual_outputs_by_run = [""] * len(flags["run_asm_cases"])

        else:
            cmd = [str(chosen_bin), str(tmp_path), str(test_file.parent)]
            _log(f"[TEST] run command: {_cmd_to_str(cmd)}")

            if flags["test_debug"]:
                debugger = "gdb"
                if sys.platform == "darwin":
                    debugger = "lldb"

                if debugger == "gdb":
                    cmd = [debugger, "--args", str(binary), str(tmp_path), str(test_file.parent)]
                else:
                    cmd = [debugger, "--", str(binary), str(tmp_path), str(test_file.parent)]

                _log(f"[TEST] debugger command: {_cmd_to_str(cmd)}")
                _append_log_section(log_sections, "DEBUGGER", "\n".join([
                    f"command: {_cmd_to_str(cmd)}",
                    "interactive debugger session was started",
                ]))
                _run_interactive_command(cmd)
                test_elapsed = time.perf_counter() - test_started_at
                return {
                    "file": str(test_file),
                    "ok": True,
                    "critical": False,
                    "warning": False,
                    "diff": None,
                    "metrics": _build_measure_info(measure_time, test_elapsed, program_elapsed, measure_lines, input_lines, output_lines),
                    "_test_elapsed": test_elapsed,
                    "_program_elapsed": program_elapsed,
                    "_input_lines": input_lines,
                    "_output_lines": output_lines,
                }

            proc, run_elapsed = _capture_process(cmd)
            actual_output = proc.stdout
            actual_outputs_by_run = [proc.stdout]
            actual_exit_codes = [proc.returncode]
            _append_log_section(log_sections, "PROGRAM OUTPUT", "\n".join([
                f"command: {_cmd_to_str(cmd)}",
                f"exit_code: {proc.returncode}",
                f"elapsed: {run_elapsed:.6f}s",
                "output:",
                proc.stdout.rstrip("\n"),
            ]))

    except Exception as ex:
        test_elapsed = time.perf_counter() - test_started_at
        _append_log_section(log_sections, "EXCEPTION", repr(ex))
        result = {
            "file": str(test_file),
            "ok": False,
            "critical": True,
            "warning": False,
            "diff": f"Subprocess error: {ex}",
            "metrics": _build_measure_info(measure_time, test_elapsed, program_elapsed, measure_lines, input_lines, output_lines),
            "_test_elapsed": test_elapsed,
            "_program_elapsed": program_elapsed,
            "_input_lines": input_lines,
            "_output_lines": output_lines,
        }
        return _attach_failure_log(result, test_file, log_sections)
    finally:
        if os.path.exists(tmp_path):
            os.remove(tmp_path)

    expected_n: str = _normalize_output(expected)
    actual_n: str = _normalize_output(actual_output)

    _append_log_section(log_sections, "NORMALIZED OUTPUT", actual_n)
    _append_log_section(log_sections, "EXIT CODES", json.dumps({
        "actual_exit_codes": actual_exit_codes,
    }, ensure_ascii=False, indent=2))

    if flags["rewrite"]:
        _rewrite_test_output(test_file, actual_n)
        test_elapsed = time.perf_counter() - test_started_at
        return {
            "file": str(test_file),
            "expected": expected_n,
            "actual": actual_n,
            "ok": True,
            "critical": False,
            "warning": False,
            "diff": None,
            "metrics": _build_measure_info(measure_time, test_elapsed, program_elapsed, measure_lines, input_lines, output_lines),
            "_test_elapsed": test_elapsed,
            "_program_elapsed": program_elapsed,
            "_input_lines": input_lines,
            "_output_lines": output_lines,
        }

    case_blocks = flags.get("output_case_blocks")
    if case_blocks is not None:
        if actual_exit_codes is None:
            ok = False
            why = "Case blocks require executable runs"
        elif len(actual_exit_codes) != len(flags["run_asm_cases"]):
            ok = False
            why = f"Case count mismatch: expected {len(flags['run_asm_cases'])} runs, got {len(actual_exit_codes)}"
        else:
            missing = [i for i in range(len(flags["run_asm_cases"])) if i not in case_blocks]
            extra = sorted(i for i in case_blocks if i < 0 or i >= len(flags["run_asm_cases"]))
            if missing:
                ok = False
                why = "Missing case blocks for indexes: " + ", ".join(map(str, missing))
            elif extra:
                ok = False
                why = "Unexpected case indexes: " + ", ".join(map(str, extra))
            else:
                ok = True
                reasons: list[str] = []
                actual_outputs = actual_outputs_by_run or []
                for i in range(len(flags["run_asm_cases"])):
                    case = case_blocks[i]
                    case_actual = actual_outputs[i] if i < len(actual_outputs) else ""
                    case_ok, case_why = _check_single_case(
                        expected_text=case["expected"],
                        actual_text=case_actual,
                        annotations=case["annotations"],
                        actual_exit_code=actual_exit_codes[i],
                    )
                    if not case_ok:
                        ok = False
                        reasons.append(_make_case_failure(
                            case_index=i,
                            case_expected=case["expected"],
                            case_actual=case_actual,
                            case_reason=case_why or "Unknown case failure",
                        ))
                why = "\n\n".join(reasons) if reasons else None
    else:
        annotations_ok, annotations_why = _check_output_annotations(flags, actual_exit_codes)

        ok, why = _matches_expected(expected_n, actual_n)

        if ok and not annotations_ok:
            ok = False
            why = annotations_why

    if flags["leak_trace"]:
        from leaks import find_leaks
        with tempfile.NamedTemporaryFile(mode="w", suffix=".mem.log", encoding="utf-8", delete=False) as lf:
            lf.write(actual_output)
            log_path = lf.name
        try:
            print("Trying to determine leaks...")
            find_leaks(path=log_path)
        finally:
            if os.path.exists(log_path):
                os.remove(log_path)

    critical: bool = False
    warning: bool = False

    if not ok:
        if flags["bug"]:
            warning = True
        elif flags["block_test"]:
            critical = True

    diff_text = None
    if not ok:
        if case_blocks is not None:
            diff_text = why
        else:
            diff_text = why + "\n\n" + _make_diff(expected_n, actual_n)

    test_elapsed = time.perf_counter() - test_started_at
    result = {
        "file": str(test_file),
        "expected": expected_n,
        "actual": actual_n,
        "ok": ok,
        "critical": critical,
        "warning": warning,
        "diff": diff_text,
        "metrics": _build_measure_info(measure_time, test_elapsed, program_elapsed, measure_lines, input_lines, output_lines),
        "_test_elapsed": test_elapsed,
        "_program_elapsed": program_elapsed,
        "_input_lines": input_lines,
        "_output_lines": output_lines,
    }
    return _attach_failure_log(result, test_file, log_sections)


def _run_test(binary: str, binary_leak: str | None, test_file: Path) -> dict:
    code, expected, flags = _parse_test_file(test_file)
    annotations = flags.get("output_annotations", {})
    measure_time = _annotation_enabled(annotations, "measure_time")
    measure_lines = _annotation_enabled(annotations, "measure_lines")
    repeat_count, repeat_error = _parse_repeat_count(annotations)
    input_lines = _count_text_lines(code)
    initial_output_lines = _count_output_lines(expected, flags)

    if repeat_error is not None:
        return {
            "file": str(test_file),
            "ok": False,
            "critical": True,
            "warning": False,
            "diff": repeat_error,
            "metrics": _build_measure_info(measure_time, 0.0, None, measure_lines, input_lines, initial_output_lines),
        }

    if repeat_count == 1:
        result = _run_test_once(
            binary=binary,
            binary_leak=binary_leak,
            test_file=test_file,
            code=code,
            expected=expected,
            flags=flags,
            measure_time=measure_time,
            measure_lines=measure_lines,
            input_lines=input_lines,
            initial_output_lines=initial_output_lines,
        )
        result.pop("_test_elapsed", None)
        result.pop("_program_elapsed", None)
        result.pop("_input_lines", None)
        result.pop("_output_lines", None)
        return result

    repeated_results: list[dict] = []
    for repeat_index in range(repeat_count):
        _log(f"[TEST] repeat {repeat_index + 1}/{repeat_count}: {test_file}")
        result = _run_test_once(
            binary=binary,
            binary_leak=binary_leak,
            test_file=test_file,
            code=code,
            expected=expected,
            flags=flags,
            measure_time=measure_time,
            measure_lines=measure_lines,
            input_lines=input_lines,
            initial_output_lines=initial_output_lines,
        )
        repeated_results.append(result)
        if not result.get("ok", False):
            diff = result.get("diff")
            prefix = f"Repeat {repeat_index + 1}/{repeat_count} failed"
            result["diff"] = prefix if not diff else f"{prefix}\n{diff}"
            result.pop("_test_elapsed", None)
            result.pop("_program_elapsed", None)
            result.pop("_input_lines", None)
            result.pop("_output_lines", None)
            return result

    avg_test_elapsed = sum(float(r.get("_test_elapsed", 0.0) or 0.0) for r in repeated_results) / repeat_count
    program_values = [r.get("_program_elapsed") for r in repeated_results if r.get("_program_elapsed") is not None]
    avg_program_elapsed = None
    if program_values:
        avg_program_elapsed = sum(float(v) for v in program_values) / len(program_values)

    output_lines = int(repeated_results[-1].get("_output_lines", initial_output_lines))
    final_result = dict(repeated_results[-1])
    final_result["metrics"] = _build_measure_info(
        measure_time,
        avg_test_elapsed,
        avg_program_elapsed,
        measure_lines,
        input_lines,
        output_lines,
    )
    final_result.pop("_test_elapsed", None)
    final_result.pop("_program_elapsed", None)
    final_result.pop("_input_lines", None)
    final_result.pop("_output_lines", None)
    return final_result

def _find_test_roots(start_path: Path) -> list[Path]:
    roots = []
    for root, _, _ in os.walk(start_path):
        root_path = Path(root)
        if (root_path / "base.c").is_file() and (root_path / "dependencies.json").is_file():
            roots.append(root_path)

    return roots

def _collect_cpl_files(root: Path, all_roots: set[Path]) -> list[Path]:
    cpl_files = []
    try:
        for entry in root.iterdir():
            if entry.is_dir():
                if entry in all_roots:
                    continue
                cpl_files.extend(_collect_cpl_files(entry, all_roots))
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

    all_roots = _find_test_roots(test_top)
    if not all_roots:
        print(f"No test modules found (no base.c + dependencies.json) under {test_top}", file=sys.stderr)
        sys.exit(1)

    all_roots_set: set[Path] = set(all_roots)
    results: list[dict] = []
    failed_modules: int = 0

    compile_pbar = tqdm(
        total=len(all_roots),
        desc="Modules compiled",
        unit="module",
        dynamic_ncols=True,
        position=0
    )

    for root in all_roots:
        rel: Path = root.relative_to(test_top)
        module_out_dir: Path = Path(args.output_dir) / rel
        os.makedirs(module_out_dir, exist_ok=True)

        deps = root / "dependencies.json"
        base = root / "base.c"

        _log(f"[BUILD] module root: {root}")
        _log(f"[BUILD] base source: {base}")
        _log(f"[BUILD] dependencies: {deps}")
        _log(f"[BUILD] output dir: {module_out_dir}")

        with deps.open("r", encoding="utf-8") as f:
            bconf_raw = json.load(f)

        bconf: CCBuilderConfig = CCBuilderConfig(
            include=args.base,
            target="unit_test",
            config=bconf_raw,
            compiler=BuildCompiler(args.compiler)
        )

        builder: CCBuilder = CCBuilder(settings=bconf)
        import io
        from contextlib import redirect_stdout, redirect_stderr
        _buf = io.StringIO()
        with redirect_stdout(_buf), redirect_stderr(_buf):
            binary: str | None = builder.build(
                test_file=str(base),
                output_dir=str(module_out_dir),
                extra_flags=['Wno-int-conversion', 'Wno-unused-function', 'Wno-ignored-qualifiers']
            )
        _out: str = _buf.getvalue()
        if _out:
            for _line in _out.rstrip("\n").splitlines():
                tqdm.write(_line)
        compile_pbar.update(1)

        if not binary:
            print(f"Failed to build module {root}, skipping its tests.", file=sys.stderr)
            failed_modules += 1
            continue

        _log(f"[BUILD] executable path: {binary}")

        cpl_files: list = _collect_cpl_files(root, all_roots_set)
        need_leak_bin = False
        for cpl in cpl_files:
            try:
                if ": LEAK_TRACE :" in cpl.read_text(encoding="utf-8"):
                    need_leak_bin = True
                    break
            except Exception:
                pass

        binary_leak: str | None = None
        if need_leak_bin:
            leak_out_dir = module_out_dir / "_leak"
            os.makedirs(leak_out_dir, exist_ok=True)
            _log(f"[BUILD] building leak binary in: {leak_out_dir}")
            _buf = io.StringIO()
            with redirect_stdout(_buf), redirect_stderr(_buf):
                binary_leak = builder.build(
                    test_file=str(base),
                    output_dir=str(leak_out_dir),
                    extra_flags=['Wno-int-conversion', 'Wno-unused-function', 'DMEM_OPERATION_LOGS']
                )
            _out = _buf.getvalue()
            if _out:
                for _line in _out.rstrip("\n").splitlines():
                    tqdm.write(_line)
            if binary_leak:
                _log(f"[BUILD] leak executable path: {binary_leak}")
            
        if not cpl_files:
            print(f"Module {root} has no .cpl files to test.")

        for cpl in tqdm(cpl_files, desc=f"Module {rel}", leave=False, position=1):
            results.append(_run_test(binary, binary_leak, cpl))

    compile_pbar.close()
    failed: int = 0
    critical_failed: bool = False
    for r in results:
        metrics_suffix = f" [{r['metrics']}]" if r.get("metrics") else ""
        if r.get("ok", False):
            print(f"Succeed: {r['file']}{metrics_suffix}")
        else:
            failed += 1
            if r.get("warning", False):
                print(f"\nWarning (BUG): {r['file']}{metrics_suffix}")
                print(r["diff"])
            elif r.get("critical", False):
                print(f"\nCRITICAL (BLOCK_TEST): {r['file']}{metrics_suffix}")
                print(r["diff"])
                critical_failed = True
            else:
                print(f"\nFailed: {r['file']}{metrics_suffix}")
                print(r["diff"])

    print(f"\nSummary: {len(results)} tests run, {failed} failed, {failed_modules} modules failed to build.")
    if critical_failed:
        print("\nCritical failure detected. Stopping immediately.")
        sys.exit(2)

    if failed > 0 or failed_modules > 0:
        sys.exit(1)

if __name__ == "__main__":
    _entry()
