from __future__ import annotations

import os
import subprocess
import tempfile
from pathlib import Path
from typing import Any

from flask import Flask, jsonify, request

app = Flask(__name__)

BASE_DIR = Path(__file__).resolve().parent
COMPILER_PATH = BASE_DIR / "ccompiler"

COMPILER_TIMEOUT = 20
PROGRAM_TIMEOUT = 5
MAX_CODE_SIZE = 200_000


def _make_cors(resp):
    resp.headers["Access-Control-Allow-Origin"] = "*"
    resp.headers["Access-Control-Allow-Headers"] = "Content-Type"
    resp.headers["Access-Control-Allow-Methods"] = "POST, OPTIONS"
    return resp


@app.after_request
def _after_request(resp):
    return _make_cors(resp)


@app.route("/cpl/run", methods=["OPTIONS"])
def cpl_run_options():
    return _make_cors(app.response_class(status=204))


def _read_json() -> dict[str, Any]:
    try:
        data = request.get_json(force=True, silent=False)
    except Exception:
        return {}
    return data if isinstance(data, dict) else {}


def _format_output(
    compiler_stdout: str,
    compiler_stderr: str,
    compiler_exit_code: int,
    program_stdout: str,
    program_stderr: str,
    program_exit_code: int | None,
) -> str:
    parts: list[str] = []

    parts.append("=== compiler stdout ===")
    parts.append(compiler_stdout.rstrip() or "<empty>")
    parts.append("")

    parts.append("=== compiler stderr ===")
    parts.append(compiler_stderr.rstrip() or "<empty>")
    parts.append("")

    parts.append(f"=== compiler exit code ===\n{compiler_exit_code}")
    parts.append("")

    if program_exit_code is not None:
        parts.append("=== program stdout ===")
        parts.append(program_stdout.rstrip() or "<empty>")
        parts.append("")

        parts.append("=== program stderr ===")
        parts.append(program_stderr.rstrip() or "<empty>")
        parts.append("")

        parts.append(f"=== program exit code ===\n{program_exit_code}")

    return "\n".join(parts).strip() + "\n"


def _json_error(message: str, status: int = 400):
    return jsonify({
        "success": False,
        "error": message,
        "stderr": message,
    }), status


@app.route("/cpl/run", methods=["POST"])
def cpl_run():
    data = _read_json()

    lang = str(data.get("lang", "cpl"))
    code = data.get("code", "")

    if lang != "cpl":
        return _json_error("Only lang='cpl' is supported.")
    if not isinstance(code, str):
        return _json_error("Field 'code' must be a string.")
    if not code.strip():
        return _json_error("Field 'code' is empty.")
    if len(code) > MAX_CODE_SIZE:
        return _json_error(f"Code is too large. Max size is {MAX_CODE_SIZE} bytes.")

    if not COMPILER_PATH.exists():
        return _json_error(f"Compiler not found: {COMPILER_PATH}", 500)
    if not os.access(COMPILER_PATH, os.X_OK):
        return _json_error(f"Compiler is not executable: {COMPILER_PATH}", 500)

    compiler_stdout = ""
    compiler_stderr = ""
    compiler_exit_code = -1

    program_stdout = ""
    program_stderr = ""
    program_exit_code = None

    try:
        with tempfile.TemporaryDirectory(prefix="cplrun_") as tmp:
            tmpdir = Path(tmp)
            src_path = tmpdir / "main.cpl"
            exe_path = tmpdir / "program.out"

            src_path.write_text(code, encoding="utf-8")

            compile_cmd = [
                str(COMPILER_PATH),
                str(src_path),
                "--ast-analysis",
                "--ir-analysis",
                "--output",
                str(exe_path),
            ]

            comp = subprocess.run(
                compile_cmd,
                cwd=tmpdir,
                capture_output=True,
                text=True,
                timeout=COMPILER_TIMEOUT,
            )

            compiler_stdout = comp.stdout or ""
            compiler_stderr = comp.stderr or ""
            compiler_exit_code = int(comp.returncode)

            if compiler_exit_code == 0 and exe_path.exists():
                try:
                    current_mode = exe_path.stat().st_mode
                    exe_path.chmod(current_mode | 0o111)
                except Exception:
                    pass

                prog = subprocess.run(
                    [str(exe_path)],
                    cwd=tmpdir,
                    capture_output=True,
                    text=True,
                    timeout=PROGRAM_TIMEOUT,
                )

                program_stdout = prog.stdout or ""
                program_stderr = prog.stderr or ""
                program_exit_code = int(prog.returncode)

    except subprocess.TimeoutExpired as e:
        partial_stdout = ""
        partial_stderr = ""
        if e.stdout:
            partial_stdout = e.stdout if isinstance(e.stdout, str) else e.stdout.decode("utf-8", "replace")
        if e.stderr:
            partial_stderr = e.stderr if isinstance(e.stderr, str) else e.stderr.decode("utf-8", "replace")

        payload = {
            "success": False,
            "error": "Timeout while compiling or running the program.",
            "compiler_stdout": compiler_stdout or partial_stdout,
            "compiler_stderr": compiler_stderr or partial_stderr,
            "compiler_exit_code": compiler_exit_code,
            "program_stdout": program_stdout,
            "program_stderr": program_stderr,
            "program_exit_code": program_exit_code,
            "stdout": _format_output(
                compiler_stdout or partial_stdout,
                compiler_stderr or partial_stderr,
                compiler_exit_code,
                program_stdout,
                program_stderr,
                program_exit_code,
            ),
            "stderr": "Timeout while compiling or running the program.",
        }
        return jsonify(payload), 200
    except Exception as e:
        return _json_error(f"Backend error: {e}", 500)

    success = compiler_exit_code == 0 and program_exit_code == 0

    payload = {
        "success": success,
        "compiler_stdout": compiler_stdout,
        "compiler_stderr": compiler_stderr,
        "compiler_exit_code": compiler_exit_code,
        "program_stdout": program_stdout,
        "program_stderr": program_stderr,
        "program_exit_code": program_exit_code,
        "stdout": _format_output(
            compiler_stdout,
            compiler_stderr,
            compiler_exit_code,
            program_stdout,
            program_stderr,
            program_exit_code,
        ),
        "stderr": "",
    }

    return jsonify(payload), 200


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=8000, debug=False)
