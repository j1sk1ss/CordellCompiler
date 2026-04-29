from __future__ import annotations

import json

from dataclasses import dataclass
from typing import Any

@dataclass
class FunctionHIR:
    name: str
    index: int
    start_index: int
    end_index: int
    instructions: list[dict[str, Any]]

    def to_program(self) -> dict[str, Any]:
        return { "instructions": self.instructions }

def split_hir_functions(program: dict[str, Any] | str) -> list[FunctionHIR]:
    if isinstance(program, str):
        program = json.loads(program)

    instructions = list(program.get("instructions", []))
    functions: list[FunctionHIR] = []
    index = 0
    function_index = 0

    while index < len(instructions):
        instr = instructions[index]

        if instr.get("op") != "fn":
            index += 1
            continue

        function = _collect_function(
            instructions,
            start_index=index,
            function_index=function_index,
        )

        functions.append(function)
        function_index += 1
        index = function.end_index

    if not functions:
        functions.append(
            FunctionHIR(
                name="<whole>",
                index=0,
                start_index=0,
                end_index=len(instructions),
                instructions=instructions,
            )
        )

    return functions

def select_hir_function(functions: list[FunctionHIR], name: str | None) -> FunctionHIR:
    if name is None:
        if len(functions) == 1:
            return functions[0]

        available = ", ".join(function.name for function in functions)
        raise RuntimeError(f"multiple functions found, select one with --function. Available: {available}")

    for function in functions:
        if function.name == name:
            return function

    available = ", ".join(function.name for function in functions)
    raise RuntimeError(f"unknown function {name!r}. Available: {available}")

def _collect_function(instructions: list[dict[str, Any]], *, start_index: int, function_index: int) -> FunctionHIR:
    name = _function_name(instructions[start_index])
    items: list[dict[str, Any]] = []
    index = start_index
    depth: int | None = None

    while index < len(instructions):
        instr = instructions[index]
        if index != start_index and instr.get("op") == "fn" and depth is None:
            break

        items.append(instr)

        op = instr.get("op")
        if op == "scope_start":
            if depth is None:
                depth = 1
            else:
                depth += 1
        elif op == "scope_end" and depth is not None:
            depth -= 1

            if depth == 0:
                index += 1
                break

        index += 1

    return FunctionHIR(
        name=name,
        index=function_index,
        start_index=start_index,
        end_index=index,
        instructions=items,
    )

def _function_name(instr: dict[str, Any]) -> str:
    func = instr.get("func") or {}
    if func.get("name"):
        return str(func["name"])
    if func.get("text"):
        return str(func["text"])

    line_no = instr.get("line_no", "?")
    return f"<function_at_line_{line_no}>"
