#!/usr/bin/env python3
from __future__ import annotations

from dataclasses import dataclass, field
import argparse
import json
from typing import Any, Iterable


TERMINATOR_OPS: set[str] = {
    "if",
    "goto",
    "return",
    "exit",
}


@dataclass
class CFGEdge:
    src: str
    dst: str | None
    kind: str
    line_no: int | None = None
    raw: str | None = None
    label: str | None = None
    condition_subject: dict[str, Any] | None = None
    condition_z3: Any = None

    def to_dict(self) -> dict[str, Any]:
        result: dict[str, Any] = {
            "src": self.src,
            "dst": self.dst,
            "kind": self.kind,
        }

        if self.line_no is not None:
            result["line_no"] = self.line_no

        if self.raw is not None:
            result["raw"] = self.raw

        if self.label is not None:
            result["label"] = self.label

        if self.condition_subject is not None:
            result["condition_subject"] = self.condition_subject

        if self.condition_z3 is not None:
            result["condition_z3"] = str(self.condition_z3)

        return result


@dataclass
class BasicBlock:
    index: int
    name: str
    start_index: int
    end_index: int

    instructions: list[dict[str, Any]]
    labels: list[str] = field(default_factory=list)

    terminator: dict[str, Any] | None = None
    edges: list[CFGEdge] = field(default_factory=list)

    @property
    def first_line_no(self) -> int | None:
        if not self.instructions:
            return None
        return _line_no(self.instructions[0])

    @property
    def last_line_no(self) -> int | None:
        if not self.instructions:
            return None
        return _line_no(self.instructions[-1])

    def has_terminator(self) -> bool:
        return self.terminator is not None

    def to_dict(self, *, include_instructions: bool = True) -> dict[str, Any]:
        result: dict[str, Any] = {
            "index": self.index,
            "name": self.name,
            "start_index": self.start_index,
            "end_index": self.end_index,
            "first_line_no": self.first_line_no,
            "last_line_no": self.last_line_no,
            "labels": list(self.labels),
            "edges": [edge.to_dict() for edge in self.edges],
        }

        if self.terminator is not None:
            result["terminator"] = {
                "op": self.terminator.get("op"),
                "line_no": self.terminator.get("line_no"),
                "raw": self.terminator.get("raw"),
            }
        else:
            result["terminator"] = None

        if include_instructions:
            result["instructions"] = self.instructions
        else:
            result["instruction_count"] = len(self.instructions)
            result["raw"] = [
                str(instr.get("raw", ""))
                for instr in self.instructions
            ]

        return result


@dataclass
class HIRCFG:
    blocks: list[BasicBlock]
    entry: str | None

    label_to_block: dict[str, str]
    unresolved_edges: list[CFGEdge] = field(default_factory=list)
    duplicate_labels: dict[str, list[str]] = field(default_factory=dict)

    def block_by_name(self, name: str) -> BasicBlock:
        for block in self.blocks:
            if block.name == name:
                return block

        raise KeyError(f"unknown block: {name!r}")

    def successors(self, block_name: str) -> list[CFGEdge]:
        return list(self.block_by_name(block_name).edges)

    def predecessors(self) -> dict[str, list[CFGEdge]]:
        result: dict[str, list[CFGEdge]] = {
            block.name: []
            for block in self.blocks
        }

        for block in self.blocks:
            for edge in block.edges:
                if edge.dst is not None:
                    result.setdefault(edge.dst, []).append(edge)

        return result

    def to_dict(self, *, include_instructions: bool = True) -> dict[str, Any]:
        return {
            "entry": self.entry,
            "label_to_block": dict(self.label_to_block),
            "duplicate_labels": self.duplicate_labels,
            "unresolved_edges": [
                edge.to_dict()
                for edge in self.unresolved_edges
            ],
            "blocks": [
                block.to_dict(include_instructions=include_instructions)
                for block in self.blocks
            ],
        }

    def dump(self) -> str:
        lines: list[str] = []

        lines.append(f"entry: {self.entry}")
        lines.append("")

        for block in self.blocks:
            label_suffix = ""
            if block.labels:
                label_suffix = " labels=" + ",".join(block.labels)

            lines.append(
                f"{block.name}:{label_suffix} "
                f"[instr {block.start_index}..{block.end_index}) "
                f"[lines {block.first_line_no}..{block.last_line_no}]"
            )

            for instr in block.instructions:
                raw = str(instr.get("raw", ""))
                line_no = instr.get("line_no", "?")
                lines.append(f"  {line_no}: {raw}")

            if block.edges:
                lines.append("  edges:")
                for edge in block.edges:
                    dst = edge.dst if edge.dst is not None else "<unresolved>"
                    extra = ""

                    if edge.kind in {"true", "false"}:
                        cond = (
                            edge.condition_z3
                            if edge.condition_z3 is not None
                            else edge.condition_subject
                        )
                        extra = f" label={edge.label} cond={cond}"

                    elif edge.label is not None:
                        extra = f" label={edge.label}"

                    lines.append(f"    - {edge.kind} -> {dst}{extra}")
            else:
                lines.append("  edges: <none>")

            lines.append("")

        if self.unresolved_edges:
            lines.append("unresolved edges:")
            for edge in self.unresolved_edges:
                lines.append(
                    f"  {edge.src} --{edge.kind}/{edge.label}--> <missing label>"
                )
            lines.append("")

        if self.duplicate_labels:
            lines.append("duplicate labels:")
            for label, blocks in self.duplicate_labels.items():
                lines.append(f"  {label}: {blocks}")
            lines.append("")

        return "\n".join(lines)

    def to_dot(self) -> str:
        """
        Graphviz DOT.

        Usage:

            python hir_cfg.py parsed.json --dot > cfg.dot
            dot -Tpng cfg.dot -o cfg.png
        """

        lines: list[str] = []
        lines.append("digraph HIRCFG {")
        lines.append("  node [shape=box];")

        for block in self.blocks:
            label_lines: list[str] = [block.name]

            if block.labels:
                label_lines.append("labels: " + ", ".join(block.labels))

            for instr in block.instructions:
                raw = _dot_escape(str(instr.get("raw", "")))
                line_no = instr.get("line_no", "?")
                label_lines.append(f"{line_no}: {raw}")

            label = "\\l".join(label_lines) + "\\l"

            lines.append(
                f'  "{_dot_escape(block.name)}" [label="{label}"];'
            )

        for block in self.blocks:
            for edge in block.edges:
                if edge.dst is None:
                    missing_name = f"missing_{edge.src}_{edge.kind}_{edge.label}"
                    lines.append(
                        f'  "{_dot_escape(missing_name)}" '
                        f'[label="missing label: {_dot_escape(str(edge.label))}", shape=octagon];'
                    )
                    dst = missing_name
                else:
                    dst = edge.dst

                edge_label = edge.kind

                if edge.kind in {"true", "false"} and edge.label:
                    edge_label += f": {edge.label}"
                elif edge.label:
                    edge_label += f": {edge.label}"

                lines.append(
                    f'  "{_dot_escape(block.name)}" -> "{_dot_escape(dst)}" '
                    f'[label="{_dot_escape(edge_label)}"];'
                )

        lines.append("}")
        return "\n".join(lines)


class HIRCFGBuilder:
    def __init__(
        self,
        program: dict[str, Any] | str,
        *,
        prepared: Any = None,
    ) -> None:
        if isinstance(program, str):
            program = json.loads(program)

        self.program = program
        self.instructions: list[dict[str, Any]] = list(
            program.get("instructions", [])
        )

        self.prepared = prepared

        # line_no -> z3 condition from PreparedHIR.branches
        self.branch_conditions_by_line: dict[int, Any] = (
            self._collect_prepared_branch_conditions(prepared)
        )

    def build(self) -> HIRCFG:
        if not self.instructions:
            return HIRCFG(
                blocks=[],
                entry=None,
                label_to_block={},
            )

        leaders = self._find_leaders()
        blocks = self._make_blocks(leaders)

        label_to_block, duplicate_labels = self._build_label_map(blocks)
        unresolved_edges = self._connect_blocks(blocks, label_to_block)

        entry = blocks[0].name if blocks else None

        return HIRCFG(
            blocks=blocks,
            entry=entry,
            label_to_block=label_to_block,
            unresolved_edges=unresolved_edges,
            duplicate_labels=duplicate_labels,
        )

    def _find_leaders(self) -> list[int]:
        leaders: set[int] = {0}
        n = len(self.instructions)

        for idx, instr in enumerate(self.instructions):
            op = instr.get("op")

            if op == "label":
                leaders.add(idx)

            if op in TERMINATOR_OPS and idx + 1 < n:
                leaders.add(idx + 1)

        return sorted(leaders)

    def _make_blocks(self, leaders: list[int]) -> list[BasicBlock]:
        blocks: list[BasicBlock] = []

        for block_index, start in enumerate(leaders):
            if block_index + 1 < len(leaders):
                end = leaders[block_index + 1]
            else:
                end = len(self.instructions)

            block_instructions = self.instructions[start:end]
            labels = self._labels_in_block(block_instructions)

            if start == 0 and not labels:
                name = "entry"
            elif labels:
                name = labels[0]
            else:
                name = f"bb{block_index}"

            terminator = self._find_block_terminator(block_instructions)

            blocks.append(
                BasicBlock(
                    index=block_index,
                    name=name,
                    start_index=start,
                    end_index=end,
                    instructions=block_instructions,
                    labels=labels,
                    terminator=terminator,
                )
            )

        return blocks

    def _labels_in_block(
        self,
        instructions: list[dict[str, Any]],
    ) -> list[str]:
        labels: list[str] = []

        for instr in instructions:
            if instr.get("op") != "label":
                continue

            label = _label_name_from_subject(instr.get("label"))
            if label is not None:
                labels.append(label)

        return labels

    def _find_block_terminator(
        self,
        instructions: list[dict[str, Any]],
    ) -> dict[str, Any] | None:
        if not instructions:
            return None

        last = instructions[-1]
        if last.get("op") in TERMINATOR_OPS:
            return last

        return None

    def _build_label_map(
        self,
        blocks: list[BasicBlock],
    ) -> tuple[dict[str, str], dict[str, list[str]]]:
        label_to_block: dict[str, str] = {}
        duplicate_labels: dict[str, list[str]] = {}

        for block in blocks:
            for label in block.labels:
                if label in label_to_block:
                    duplicate_labels.setdefault(
                        label,
                        [label_to_block[label]],
                    ).append(block.name)
                else:
                    label_to_block[label] = block.name

        return label_to_block, duplicate_labels

    def _connect_blocks(
        self,
        blocks: list[BasicBlock],
        label_to_block: dict[str, str],
    ) -> list[CFGEdge]:
        unresolved_edges: list[CFGEdge] = []

        for idx, block in enumerate(blocks):
            next_block = blocks[idx + 1] if idx + 1 < len(blocks) else None

            edges = self._edges_for_block(
                block,
                next_block=next_block,
                label_to_block=label_to_block,
            )

            block.edges.extend(edges)

            for edge in edges:
                if edge.dst is None:
                    unresolved_edges.append(edge)

        return unresolved_edges

    def _edges_for_block(
        self,
        block: BasicBlock,
        *,
        next_block: BasicBlock | None,
        label_to_block: dict[str, str],
    ) -> list[CFGEdge]:
        terminator = block.terminator

        if terminator is None:
            if next_block is None:
                return []

            return [
                CFGEdge(
                    src=block.name,
                    dst=next_block.name,
                    kind="fallthrough",
                    line_no=block.last_line_no,
                    raw=None,
                )
            ]

        op = terminator.get("op")

        if op == "if":
            return self._if_edges(
                block,
                terminator,
                label_to_block=label_to_block,
            )

        if op == "goto":
            return [
                self._goto_edge(
                    block,
                    terminator,
                    label_to_block=label_to_block,
                )
            ]

        if op in {"return", "exit"}:
            return []

        # Should not happen if TERMINATOR_OPS and this function stay in sync.
        if next_block is None:
            return []

        return [
            CFGEdge(
                src=block.name,
                dst=next_block.name,
                kind="fallthrough",
                line_no=block.last_line_no,
                raw=str(terminator.get("raw", "")),
            )
        ]

    def _if_edges(
        self,
        block: BasicBlock,
        instr: dict[str, Any],
        *,
        label_to_block: dict[str, str],
    ) -> list[CFGEdge]:
        true_label = _label_name_from_subject(instr.get("true_label"))
        false_label = _label_name_from_subject(instr.get("false_label"))

        line_no = _line_no(instr)
        condition_z3 = None

        if line_no is not None:
            condition_z3 = self.branch_conditions_by_line.get(line_no)

        true_dst = label_to_block.get(true_label or "")
        false_dst = label_to_block.get(false_label or "")

        return [
            CFGEdge(
                src=block.name,
                dst=true_dst,
                kind="true",
                line_no=line_no,
                raw=str(instr.get("raw", "")),
                label=true_label,
                condition_subject=instr.get("cond"),
                condition_z3=condition_z3,
            ),
            CFGEdge(
                src=block.name,
                dst=false_dst,
                kind="false",
                line_no=line_no,
                raw=str(instr.get("raw", "")),
                label=false_label,
                condition_subject=instr.get("cond"),
                condition_z3=condition_z3,
            ),
        ]

    def _goto_edge(
        self,
        block: BasicBlock,
        instr: dict[str, Any],
        *,
        label_to_block: dict[str, str],
    ) -> CFGEdge:
        label = _label_name_from_subject(instr.get("target"))
        dst = label_to_block.get(label or "")

        return CFGEdge(
            src=block.name,
            dst=dst,
            kind="goto",
            line_no=_line_no(instr),
            raw=str(instr.get("raw", "")),
            label=label,
        )

    def _collect_prepared_branch_conditions(
        self,
        prepared: Any,
    ) -> dict[int, Any]:
        if prepared is None:
            return {}

        result: dict[int, Any] = {}

        for branch in getattr(prepared, "branches", []):
            line_no = getattr(branch, "line_no", None)
            cond = getattr(branch, "cond", None)

            if line_no is not None and cond is not None:
                result[int(line_no)] = cond

        return result


def build_hir_cfg(
    program: dict[str, Any] | str,
    *,
    prepared: Any = None,
) -> HIRCFG:
    return HIRCFGBuilder(
        program,
        prepared=prepared,
    ).build()


def _label_name_from_subject(subject: dict[str, Any] | None) -> str | None:
    if not subject:
        return None

    if subject.get("name"):
        return str(subject["name"])

    if subject.get("text"):
        return str(subject["text"])

    label_id = subject.get("label_id")
    if label_id is not None:
        return f"lb{label_id}"

    return None


def _line_no(instr: dict[str, Any]) -> int | None:
    value = instr.get("line_no")

    if value is None:
        return None

    try:
        return int(value)
    except Exception:
        return None


def _dot_escape(text: str) -> str:
    return (
        text
        .replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", "\\n")
        .replace("\r", "")
    )


def main(argv: Iterable[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Build CFG from parsed HIR JSON.",
    )

    parser.add_argument(
        "path",
        nargs="?",
        help="Path to JSON produced by hir_dump_parser.py. stdin is used if omitted.",
    )

    parser.add_argument(
        "--json",
        action="store_true",
        help="Print CFG as JSON.",
    )

    parser.add_argument(
        "--compact",
        action="store_true",
        help="Compact JSON output.",
    )

    parser.add_argument(
        "--dot",
        action="store_true",
        help="Print Graphviz DOT.",
    )

    parser.add_argument(
        "--no-instructions",
        action="store_true",
        help="Do not include full instructions in JSON output.",
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
        import sys
        data = sys.stdin.read()

    program = json.loads(data)
    cfg = build_hir_cfg(program)

    if ns.dot:
        print(cfg.to_dot())
        return 0

    if ns.json:
        print(
            json.dumps(
                cfg.to_dict(include_instructions=not ns.no_instructions),
                ensure_ascii=False,
                indent=None if ns.compact else 2,
            )
        )
        return 0

    print(cfg.dump())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())