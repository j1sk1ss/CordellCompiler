from __future__ import annotations

from dataclasses import dataclass, field, asdict
import argparse
import json
import re
import sys
from typing import Any, Iterable


VAR_PREFIXES: dict[str, tuple[str, str]] = {
    "strs": ("str", "stack"),
    "arrs": ("arr", "stack"),
    "f64s": ("f64", "stack"),
    "u64s": ("u64", "stack"),
    "i64s": ("i64", "stack"),
    "f32s": ("f32", "stack"),
    "u32s": ("u32", "stack"),
    "i32s": ("i32", "stack"),
    "u16s": ("u16", "stack"),
    "i16s": ("i16", "stack"),
    "u8s": ("u8", "stack"),
    "i8s": ("i8", "stack"),
    "i0s": ("i0", "stack"),
    "strt": ("str", "tmp"),
    "arrt": ("arr", "tmp"),
    "f64t": ("f64", "tmp"),
    "u64t": ("u64", "tmp"),
    "i64t": ("i64", "tmp"),
    "f32t": ("f32", "tmp"),
    "u32t": ("u32", "tmp"),
    "i32t": ("i32", "tmp"),
    "u16t": ("u16", "tmp"),
    "i16t": ("i16", "tmp"),
    "u8t": ("u8", "tmp"),
    "i8t": ("i8", "tmp"),
    "i0t": ("i0", "tmp"),
    "strg": ("str", "global"),
    "arrg": ("arr", "global"),
    "f64g": ("f64", "global"),
    "u64g": ("u64", "global"),
    "i64g": ("i64", "global"),
    "f32g": ("f32", "global"),
    "u32g": ("u32", "global"),
    "i32g": ("i32", "global"),
    "u16g": ("u16", "global"),
    "i16g": ("i16", "global"),
    "u8g": ("u8", "global"),
    "i8g": ("i8", "global"),
    "i0g": ("i0", "global"),
}

NUM_PREFIXES: dict[str, str] = {
    "f64n": "f64",
    "i64n": "i64",
    "u64n": "u64",
    "f32n": "f32",
    "i32n": "i32",
    "u32n": "u32",
    "i16n": "i16",
    "u16n": "u16",
    "i8n": "i8",
    "u8n": "u8",
    "num?": "unknown",
}

CONST_PREFIXES: dict[str, str] = {
    "csi8": "i8",
    "csu8": "u8",
    "csi16": "i16",
    "csu16": "u16",
    "csi32": "i32",
    "csu32": "u32",
    "csi64": "i64",
    "csu64": "u64",
}

BINARY_OPERATORS: tuple[str, ...] = (
    "<<",
    ">>",
    ">=",
    "<=",
    "==",
    "!=",
    "&&",
    "||",
    "+",
    "-",
    "*",
    "/",
    "%",
    ">",
    "<",
    "&",
    "|",
    "^",
)

CAST_TYPES: set[str] = {
    "f64",
    "f32",
    "i64",
    "i32",
    "i16",
    "i8",
    "u64",
    "u32",
    "u16",
    "u8",
    "*ptr",
}


class HIRParseError(ValueError):
    pass


@dataclass(slots=True)
class Subject:
    kind: str
    text: str

    ty: str | None = None
    storage: str | None = None
    ptr: int = 0

    var_id: int | None = None
    label_id: int | None = None

    value: str | int | float | None = None
    name: str | None = None

    items: list["Subject"] = field(default_factory=list)
    pairs: list[dict[str, int]] = field(default_factory=list)

    @property
    def z3_name(self) -> str | None:
        if self.kind != "var" or self.var_id is None:
            return None

        ptr_suffix = "" if self.ptr == 0 else "_p" + str(self.ptr)
        return f"{self.ty}_{self.storage}{ptr_suffix}_{self.var_id}"

    def to_dict(self) -> dict[str, Any]:
        d = asdict(self)
        if self.z3_name is not None:
            d["z3_name"] = self.z3_name
        return d


@dataclass(slots=True)
class Instruction:
    op: str
    line_no: int
    indent: int
    raw: str

    dst: Subject | None = None
    args: list[Subject] = field(default_factory=list)

    src: Subject | None = None
    lhs: Subject | None = None
    rhs: Subject | None = None
    base: Subject | None = None

    cond: Subject | None = None
    true_label: Subject | None = None
    false_label: Subject | None = None
    label: Subject | None = None

    func: Subject | None = None
    target: Subject | None = None
    arg: Subject | None = None

    text: str | None = None
    ty: str | None = None
    extra: dict[str, Any] = field(default_factory=dict)

    def to_dict(self) -> dict[str, Any]:
        result: dict[str, Any] = {
            "op": self.op,
            "line_no": self.line_no,
            "indent": self.indent,
            "raw": self.raw,
        }

        for name in (
            "dst",
            "src",
            "lhs",
            "rhs",
            "base",
            "cond",
            "true_label",
            "false_label",
            "label",
            "func",
            "target",
            "arg",
        ):
            value = getattr(self, name)
            if value is not None:
                result[name] = value.to_dict()

        if self.args:
            result["args"] = [arg.to_dict() for arg in self.args]

        if self.text is not None:
            result["text"] = self.text

        if self.ty is not None:
            result["ty"] = self.ty

        if self.extra:
            result["extra"] = self.extra

        return result


@dataclass(slots=True)
class Program:
    instructions: list[Instruction]

    def to_dict(self) -> dict[str, Any]:
        return {
            "instructions": [instr.to_dict() for instr in self.instructions],
        }

    def to_json(self, *, indent: int = 2) -> str:
        return json.dumps(self.to_dict(), ensure_ascii=False, indent=indent)


class HIRDumpParser:
    def __init__(self, *, strict: bool = False) -> None:
        self.strict = strict

    def parse(self, text: str) -> Program:
        instructions: list[Instruction] = []

        for line_no, full_line in enumerate(text.splitlines(), 1):
            if not full_line.strip():
                continue

            try:
                instructions.append(self.parse_line(full_line, line_no))
            except HIRParseError:
                if self.strict:
                    raise

                instructions.append(
                    self._instr(
                        "unknown",
                        line_no,
                        full_line,
                        text=full_line.strip(),
                    )
                )

        return Program(instructions)

    def parse_line(self, full_line: str, line_no: int) -> Instruction:
        line = full_line.strip()
        if line == "start {":
            return self._instr("start", line_no, full_line)

        if line == "{":
            return self._instr("scope_start", line_no, full_line)

        if line == "}":
            return self._instr("scope_end", line_no, full_line)

        if line == "// break;":
            return self._instr("break", line_no, full_line)

        m = re.fullmatch(r"(lb\d+):", line)
        if m:
            return self._instr(
                "label",
                line_no,
                full_line,
                label=parse_subject(m.group(1)),
            )

        if line.startswith("fn "):
            return self._instr(
                "fn",
                line_no,
                full_line,
                func=parse_subject(line[3:].strip()),
            )

        m = re.fullmatch(r"\(fun\) extern\((.*)\);", line)
        if m:
            return self._instr(
                "extern_func",
                line_no,
                full_line,
                func=parse_subject(m.group(1).strip()),
            )

        m = re.fullmatch(r"\(var\) extern\((.*)\);", line)
        if m:
            return self._instr(
                "extern_var",
                line_no,
                full_line,
                arg=parse_subject(m.group(1).strip()),
            )

        m = re.fullmatch(r"\[raw,\s*\"(.*)\"\]", line)
        if m:
            return self._instr(
                "raw",
                line_no,
                full_line,
                text=m.group(1),
            )

        m = re.fullmatch(r"asm\((.*)\)\s*\{", line)
        if m:
            return self._instr(
                "asm_start",
                line_no,
                full_line,
                text=m.group(1),
            )

        m = re.fullmatch(r"return\s+(.+);", line)
        if m:
            return self._instr(
                "return",
                line_no,
                full_line,
                arg=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"goto\s+(lb\d+);", line)
        if m:
            return self._instr(
                "goto",
                line_no,
                full_line,
                target=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"exit\s+(.+);", line)
        if m:
            return self._instr(
                "exit",
                line_no,
                full_line,
                arg=parse_subject(m.group(1)),
            )

        m = re.fullmatch(
            r"if\s+(.+),\s*goto\s+(lb\d+),\s*else\s*goto\s+(lb\d+);",
            line,
        )
        if m:
            return self._instr(
                "if",
                line_no,
                full_line,
                cond=parse_subject(m.group(1)),
                true_label=parse_subject(m.group(2)),
                false_label=parse_subject(m.group(3)),
            )

        m = re.fullmatch(r"breakpoint\((.*)\);", line)
        if m:
            return self._instr(
                "breakpoint",
                line_no,
                full_line,
                arg=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"use\s+(.+);", line)
        if m:
            return self._instr(
                "use",
                line_no,
                full_line,
                arg=parse_subject(m.group(1)),
            )

        # SSA / phi helpers.
        m = re.fullmatch(
            r"\[SSA\]\s*future:\s*(.+)\s*<<==\s*previous:\s*(.+);",
            line,
        )
        if m:
            return self._instr(
                "phi_preamble",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                src=parse_subject(m.group(2)),
            )

        m = re.fullmatch(
            r"\[base:\s*(.+)\]\s+(.+?)\s*=\s*phi\((.*)\);",
            line,
        )
        if m:
            return self._instr(
                "phi",
                line_no,
                full_line,
                base=parse_subject(m.group(1)),
                dst=parse_subject(m.group(2)),
                arg=parse_subject(m.group(3)),
            )

        m = re.fullmatch(r"syscall\((.*)\);?", line)
        if m:
            return self._instr(
                "syscall",
                line_no,
                full_line,
                func=parse_subject("syscall"),
                args=parse_arglist(m.group(1)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*load_starg\(\);", line)
        if m:
            return self._instr(
                "load_starg",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*load_arg\(\);", line)
        if m:
            return self._instr(
                "load_arg",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*arr_alloc\((.*)\);", line)
        if m:
            return self._instr(
                "arr_alloc",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                arg=parse_subject(m.group(2)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*str_alloc\((.*)\);", line)
        if m:
            return self._instr(
                "str_alloc",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                arg=parse_subject(m.group(2)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*alloc\((.*)\);", line)
        if m:
            return self._instr(
                "global_alloc",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                arg=parse_subject(m.group(2)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*alloc;", line)
        if m:
            return self._instr(
                "local_alloc",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*\*\((.+)\);", line)
        if m:
            return self._instr(
                "load_ref",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                src=parse_subject(m.group(2)),
            )

        m = re.fullmatch(r"\*\((.+)\)\s*=\s*(.+);", line)
        if m:
            return self._instr(
                "store_ref",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                src=parse_subject(m.group(2)),
            )

        m = re.fullmatch(r"(.+?)\s*=\s*&\((.+)\);", line)
        if m:
            return self._instr(
                "ref",
                line_no,
                full_line,
                dst=parse_subject(m.group(1)),
                src=parse_subject(m.group(2)),
            )

        if line.endswith(";") and " = " in line:
            return self._parse_assignment(
                line[:-1],
                line_no,
                full_line,
            )

        m = re.fullmatch(r"(.+?)\((.*)\);", line)
        if m:
            return self._instr(
                "call",
                line_no,
                full_line,
                func=parse_subject(m.group(1).strip()),
                args=parse_arglist(m.group(2)),
            )

        raise HIRParseError(f"cannot parse line {line_no}: {full_line!r}")

    def _parse_assignment(
        self,
        stmt: str,
        line_no: int,
        full_line: str,
    ) -> Instruction:
        dst_text, expr = stmt.split(" = ", 1)
        dst = parse_subject(dst_text)
        expr = expr.strip()

        m = re.fullmatch(r"(.+?)\s+as\s+(\*ptr|[fiu]\d+)", expr)
        if m and m.group(2) in CAST_TYPES:
            return self._instr(
                "cast",
                line_no,
                full_line,
                dst=dst,
                src=parse_subject(m.group(1)),
                ty=m.group(2),
            )

        if expr.startswith("not "):
            return self._instr(
                "not",
                line_no,
                full_line,
                dst=dst,
                src=parse_subject(expr[4:]),
            )

        m = re.fullmatch(r"syscall\((.*)\)", expr)
        if m:
            return self._instr(
                "store_syscall",
                line_no,
                full_line,
                dst=dst,
                func=parse_subject("syscall"),
                args=parse_arglist(m.group(1)),
            )

        m = re.fullmatch(r"(.+?)\((.*)\)", expr)
        if m:
            return self._instr(
                "store_call",
                line_no,
                full_line,
                dst=dst,
                func=parse_subject(m.group(1).strip()),
                args=parse_arglist(m.group(2)),
            )

        bin_split = split_binary_expr(expr)
        if bin_split is not None:
            lhs_text, op, rhs_text = bin_split
            return self._instr(
                "binary",
                line_no,
                full_line,
                dst=dst,
                lhs=parse_subject(lhs_text),
                rhs=parse_subject(rhs_text),
                text=op,
            )

        return self._instr(
            "assign",
            line_no,
            full_line,
            dst=dst,
            src=parse_subject(expr),
        )

    def _instr(
        self,
        op: str,
        line_no: int,
        full_line: str,
        **kwargs: Any,
    ) -> Instruction:
        return Instruction(
            op=op,
            line_no=line_no,
            indent=_indent_level(full_line),
            raw=full_line.strip(),
            **kwargs,
        )


def parse_subject(text: str) -> Subject:
    text = text.strip()

    if not text:
        return Subject(kind="empty", text=text)

    var_prefix_alt = "|".join(
        re.escape(prefix)
        for prefix in sorted(VAR_PREFIXES, key=len, reverse=True)
    )

    m = re.fullmatch(rf"({var_prefix_alt})(\**)?\s+%(\d+)", text)
    if m:
        prefix, stars, var_id = m.groups()
        ty, storage = VAR_PREFIXES[prefix]
        var_id_int = int(var_id)

        return Subject(
            kind="var",
            text=text,
            ty=ty,
            storage=storage,
            ptr=len(stars or ""),
            var_id=var_id_int,
            name=f"%{var_id_int}",
        )

    num_prefix_alt = "|".join(
        re.escape(prefix)
        for prefix in sorted(NUM_PREFIXES, key=len, reverse=True)
    )

    m = re.fullmatch(rf"({num_prefix_alt})\s+(.+)", text)
    if m:
        prefix, value = m.groups()
        return Subject(
            kind="number",
            text=text,
            ty=NUM_PREFIXES[prefix],
            value=value,
        )

    const_prefix_alt = "|".join(
        re.escape(prefix)
        for prefix in sorted(CONST_PREFIXES, key=len, reverse=True)
    )

    m = re.fullmatch(rf"({const_prefix_alt})\s+(-?\d+)", text)
    if m:
        prefix, value = m.groups()
        return Subject(
            kind="const",
            text=text,
            ty=CONST_PREFIXES[prefix],
            value=int(value),
        )

    m = re.fullmatch(r"lb(\d+)", text)
    if m:
        return Subject(
            kind="label",
            text=text,
            label_id=int(m.group(1)),
            name=text,
        )

    if text.startswith("set "):
        pairs: list[dict[str, int]] = []

        for var_id, bb_id in re.findall(
            r"\[%\s*(\d+)\s*,\s*bb\s*(\d+)\]",
            text[4:],
        ):
            pairs.append(
                {
                    "var_id": int(var_id),
                    "bb": int(bb_id),
                }
            )

        return Subject(
            kind="phiset",
            text=text,
            pairs=pairs,
        )

    parts = split_top_level_commas(text)
    if len(parts) > 1:
        return Subject(
            kind="arglist",
            text=text,
            items=[parse_subject(part) for part in parts],
        )

    # Function declaration form:
    #
    #   name(type arg, ...) -> type
    m = re.fullmatch(
        r"([A-Za-z_$\.][\w$\.]*)\((.*)\)(?:\s*->\s*(.+))?",
        text,
    )
    if m:
        return Subject(
            kind="function",
            text=text,
            name=m.group(1),
            value=m.group(3),
        )

    if re.fullmatch(r"[A-Za-z_$\.][\w$\.]*", text):
        return Subject(
            kind="symbol",
            text=text,
            name=text,
        )

    return Subject(
        kind="raw",
        text=text,
        value=text,
    )


def parse_arglist(text: str) -> list[Subject]:
    text = text.strip()

    if not text:
        return []

    return [
        parse_subject(part)
        for part in split_top_level_commas(text)
    ]


def split_binary_expr(expr: str) -> tuple[str, str, str] | None:
    """
    Split:

        lhs OP rhs

    where OP is surrounded by spaces.

    This is important because operands themselves contain spaces:

        i32t %1 + i32n 3

    The dumper prints binary operators with spaces around them, so this split
    is reliable for the current format.
    """

    for op in BINARY_OPERATORS:
        needle = f" {op} "
        pos = expr.find(needle)

        if pos >= 0:
            lhs = expr[:pos].strip()
            rhs = expr[pos + len(needle):].strip()

            if lhs and rhs:
                return lhs, op, rhs

    return None


def split_top_level_commas(text: str) -> list[str]:
    """
    Split by commas, but only at top level.

    Good:

        i32t %1,i32n 2,u8t %3

    Also safe for future nested forms:

        foo(i32t %1,i32n 2),bar(u8t %3)
    """

    parts: list[str] = []
    start = 0

    paren = 0
    bracket = 0
    brace = 0

    quote: str | None = None
    escaped = False

    for i, ch in enumerate(text):
        if quote is not None:
            if escaped:
                escaped = False
            elif ch == "\\":
                escaped = True
            elif ch == quote:
                quote = None
            continue

        if ch in {'"', "'"}:
            quote = ch
            continue

        if ch == "(":
            paren += 1
        elif ch == ")" and paren:
            paren -= 1
        elif ch == "[":
            bracket += 1
        elif ch == "]" and bracket:
            bracket -= 1
        elif ch == "{":
            brace += 1
        elif ch == "}" and brace:
            brace -= 1
        elif ch == "," and paren == 0 and bracket == 0 and brace == 0:
            parts.append(text[start:i].strip())
            start = i + 1

    parts.append(text[start:].strip())

    return [
        part
        for part in parts
        if part
    ]


def _indent_level(full_line: str) -> int:
    spaces = len(full_line) - len(full_line.lstrip(" "))
    return spaces // 4


def parse_hir_dump(text: str, *, strict: bool = False) -> Program:
    return HIRDumpParser(strict=strict).parse(text)


def _main(argv: Iterable[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Parse textual HIR dump into JSON.",
    )

    parser.add_argument(
        "path",
        nargs="?",
        help="HIR dump path; stdin is used when omitted",
    )

    parser.add_argument(
        "--strict",
        action="store_true",
        help="fail instead of emitting op=unknown",
    )

    parser.add_argument(
        "--compact",
        action="store_true",
        help="print compact JSON",
    )

    ns = parser.parse_args(
        list(argv)
        if argv is not None
        else None
    )

    if ns.path:
        with open(ns.path, "r", encoding="utf-8") as f:
            data = f.read()
    else:
        data = sys.stdin.read()

    program = parse_hir_dump(
        data,
        strict=ns.strict,
    )

    print(
        json.dumps(
            program.to_dict(),
            ensure_ascii=False,
            indent=None if ns.compact else 2,
        )
    )

    return 0


if __name__ == "__main__":
    raise SystemExit(_main())