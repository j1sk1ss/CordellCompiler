from __future__ import annotations

import z3
import re
import json
import argparse

from typing import Any, Iterable
from dataclasses import dataclass, field

from hir_parser import parse_hir_dump
from hir_functions import split_hir_functions, select_hir_function
from z3_first_raw import prepare_hir_for_z3
from hir_cfg_builder import build_hir_cfg

@dataclass
class WrapperContext:
    program: dict[str, Any]
    function: Any
    prepared: Any
    cfg: Any
    initial_assignments: list[InitialAssignment] = field(default_factory=list)

@dataclass
class PathState:
    block_name: str
    path_blocks: list[str]
    path_edges: list[Any] = field(default_factory=list)
    conditions: list[z3.BoolRef] = field(default_factory=list)
    depth: int = 0
    end_reason: str | None = None

    def condition_expr(self) -> z3.BoolRef:
        if not self.conditions:
            return z3.BoolVal(True)

        return z3.simplify(z3.And(*self.conditions))

@dataclass
class CheckResult:
    title: str
    check: z3.CheckSatResult
    model: z3.ModelRef | None
    condition: z3.BoolRef | None = None
    path: PathState | None = None
    extra: dict[str, Any] = field(default_factory=dict)

@dataclass
class InitialAssignment:
    variable: str
    value: str
    condition: z3.BoolRef

def _load_program_from_file(path: str, *, input_kind: str, strict_parser: bool) -> dict[str, Any]:
    with open(path, "r", encoding="utf-8") as file:
        text = file.read()

    return _load_program_from_text(
        text,
        input_kind=input_kind,
        strict_parser=strict_parser,
    )

def _load_program_from_text(text: str, *, input_kind: str, strict_parser: bool) -> dict[str, Any]:
    if input_kind == "json":
        return json.loads(text)
    elif input_kind == "dump":
        return parse_hir_dump(text, strict=strict_parser).to_dict()
    elif input_kind == "auto":
        try:
            return json.loads(text)
        except:
            pass
        
        try:
            return parse_hir_dump(text, strict=strict_parser).to_dict()
        except:
            pass

    raise ValueError(f"unknown input kind: {input_kind!r}")

def _parse_initial_assignment(text: str) -> tuple[str, str]:
    if "=" not in text:
        raise RuntimeError(f"invalid --set value {text!r}, expected VAR=VALUE")

    variable, value = text.split("=", 1)
    variable = variable.strip()
    value = value.strip()

    if not variable:
        raise RuntimeError(f"invalid --set value {text!r}, empty variable")
    if not value:
        raise RuntimeError(f"invalid --set value {text!r}, empty value")

    return variable, value

def _merged_z3_values(prepared: Any) -> dict[str, z3.ExprRef]:
    merged: dict[str, z3.ExprRef] = {}
    for name, expr in getattr(prepared, "symbols", {}).items():
        merged[name] = expr
    for name, expr in getattr(prepared, "values", {}).items():
        merged[name] = expr

    return merged

def _resolve_assignment_variable(prepared: Any, variable: str) -> tuple[str, z3.ExprRef]:
    merged = _merged_z3_values(prepared)
    if variable in merged:
        return variable, merged[variable]

    if variable.startswith("%"):
        var_id = int(variable[1:], 10)
        candidates = _find_var_candidates(
            prepared,
            var_id,
            ty=None,
            storage=None,
        )
    elif variable.isdigit():
        var_id = int(variable, 10)
        candidates = _find_var_candidates(
            prepared,
            var_id,
            ty=None,
            storage=None,
        )
    else:
        candidates = [
            (name, expr)
            for name, expr in merged.items()
            if name == variable
        ]

    if not candidates:
        raise RuntimeError(f"variable {variable!r} was not found")

    if len(candidates) > 1:
        names = ", ".join(name for name, _ in candidates)
        raise RuntimeError(f"variable {variable!r} is ambiguous: {names}")

    return candidates[0]

def _build_initial_assignments(prepared: Any, assignments: list[str]) -> list[InitialAssignment]:
    result: list[InitialAssignment] = []

    for assignment_text in assignments:
        variable_spec, value_text = _parse_initial_assignment(assignment_text)
        variable_name, expr = _resolve_assignment_variable(
            prepared,
            variable_spec,
        )

        value = _value_for_sort(
            value_text,
            expr.sort(),
        )

        result.append(
            InitialAssignment(
                variable=variable_name,
                value=value_text,
                condition=expr == value,
            )
        )

    return result

def _build_context(args: argparse.Namespace) -> WrapperContext:
    full_program = _load_program_from_file(args.input, input_kind=args.input_kind, strict_parser=args.strict_parser)
    functions = split_hir_functions(full_program)
    function = select_hir_function(functions, args.function)
    program = function.to_program()
    prepared = prepare_hir_for_z3(program, ptr_bits=args.ptr_bits, strict=args.strict_z3)
    initial_assignments = _build_initial_assignments(prepared, args.sets)
    for assignment in initial_assignments:
        prepared.constraints.append(assignment.condition)
        prepared.solver.add(assignment.condition)

    cfg = build_hir_cfg(program, prepared=prepared)
    return WrapperContext(
        program=program,
        function=function,
        prepared=prepared,
        cfg=cfg,
        initial_assignments=initial_assignments,
    )

def _make_solver(prepared: Any, conditions: Iterable[z3.BoolRef] = ()) -> z3.Solver:
    solver = z3.Solver()
    for constraint in getattr(prepared, "constraints", []):
        solver.add(constraint)

    for condition in conditions:
        if z3.is_true(condition):
            continue

        solver.add(condition)

    return solver

def _iter_subjects(value: Any) -> Iterable[dict[str, Any]]:
    if isinstance(value, dict):
        if "kind" in value:
            yield value

        for nested in value.values():
            yield from _iter_subjects(nested)
    elif isinstance(value, list):
        for item in value:
            yield from _iter_subjects(item)

def _expr_from_var_subject(prepared: Any, subject: dict[str, Any]) -> z3.ExprRef:
    name = subject.get("z3_name")

    if name is None:
        name = _fallback_z3_name_for_var(subject)

    values = getattr(prepared, "values", {})
    symbols = getattr(prepared, "symbols", {})

    if name in values:
        return values[name]
    elif name in symbols:
        return symbols[name]

    raise RuntimeError(f"variable was not prepared for z3: {name}")

def _find_var_subject_in_block(block: Any, var_id: int, *, ty: str | None, storage: str | None) -> dict[str, Any] | None:
    fallback: dict[str, Any] | None = None

    for instr in getattr(block, "instructions", []):
        for subject in _iter_subjects(instr):
            if subject.get("kind") != "var":
                continue
            elif int(subject.get("var_id") or -1) != var_id:
                continue

            if fallback is None:
                fallback = subject

            if ty is not None and subject.get("ty") != ty:
                continue
            if storage is not None and subject.get("storage") != storage:
                continue

            return subject

    return fallback

def _find_phi_source_subject(ctx: WrapperContext, phi_instr: dict[str, Any], src_block: Any) -> dict[str, Any] | None:
    dst_subject = phi_instr.get("dst")

    if not dst_subject:
        return None

    dst_var_id = dst_subject.get("var_id")

    for instr in getattr(src_block, "instructions", []):
        if instr.get("op") != "phi_preamble":
            continue

        preamble_dst = instr.get("dst")
        preamble_src = instr.get("src")

        if not preamble_dst or not preamble_src:
            continue
        if preamble_dst.get("var_id") == dst_var_id:
            return preamble_src

    phi_arg = phi_instr.get("arg")

    if not phi_arg or phi_arg.get("kind") != "phiset":
        return None

    for pair in phi_arg.get("pairs", []):
        source_var_id = int(pair.get("var_id") or -1)

        subject = _find_var_subject_in_block(
            src_block,
            source_var_id,
            ty=dst_subject.get("ty"),
            storage=dst_subject.get("storage"),
        )

        if subject is not None:
            return subject

    return None

def _phi_constraints_for_edge(ctx: WrapperContext, edge: Any) -> list[z3.BoolRef]:
    dst_name = getattr(edge, "dst", None)
    src_name = getattr(edge, "src", None)

    if dst_name is None or src_name is None:
        return []

    dst_block = _block_by_name(ctx.cfg, dst_name)
    src_block = _block_by_name(ctx.cfg, src_name)
    constraints: list[z3.BoolRef] = []

    for instr in getattr(dst_block, "instructions", []):
        if instr.get("op") != "phi":
            continue

        dst_subject = instr.get("dst")
        src_subject = _find_phi_source_subject(ctx, instr, src_block)

        if dst_subject is None or src_subject is None:
            continue

        dst_expr = _expr_from_var_subject(ctx.prepared, dst_subject)
        src_expr = _expr_from_var_subject(ctx.prepared, src_subject)

        constraints.append(dst_expr == src_expr)

    return constraints

def _phi_constraints_for_path(ctx: WrapperContext, path: PathState) -> list[z3.BoolRef]:
    constraints: list[z3.BoolRef] = []
    for edge in path.path_edges:
        constraints.extend(_phi_constraints_for_edge(ctx, edge))

    return constraints

def _check_path_with_conditions(ctx: WrapperContext, path: PathState, extra_conditions: Iterable[z3.BoolRef]) -> tuple[z3.CheckSatResult, z3.ModelRef | None]:
    conditions = list(path.conditions)
    conditions.extend(_phi_constraints_for_path(ctx, path))
    conditions.extend(extra_conditions)

    return _check_with_conditions(ctx.prepared, conditions)

def _check_with_conditions(prepared: Any, conditions: Iterable[z3.BoolRef]) -> tuple[z3.CheckSatResult, z3.ModelRef | None]:
    solver = _make_solver(prepared, conditions)
    check = solver.check()
    if check == z3.sat:
        return check, solver.model()

    return check, None

def _as_bool_expr(expr: z3.ExprRef) -> z3.BoolRef:
    if expr.sort().kind() == z3.Z3_BOOL_SORT:
        return expr
    elif expr.sort().kind() == z3.Z3_BV_SORT:
        return expr != z3.BitVecVal(0, expr.size())
    elif expr.sort().kind() in {z3.Z3_INT_SORT, z3.Z3_REAL_SORT}:
        return expr != 0

    raise TypeError(f"cannot convert expression to bool: {expr} : {expr.sort()}")

def _edge_condition(edge: Any, prepared: Any) -> z3.BoolRef:
    kind = getattr(edge, "kind", None)
    if kind not in {"true", "false"}:
        return z3.BoolVal(True)

    condition = getattr(edge, "condition_z3", None)
    if condition is None:
        condition = _condition_from_subject(
            getattr(edge, "condition_subject", None),
            prepared,
        )

    condition = _as_bool_expr(condition)
    if kind == "true":
        return condition

    return z3.Not(condition)

def _condition_from_subject(subject: dict[str, Any] | None, prepared: Any) -> z3.ExprRef:
    if not subject:
        raise RuntimeError("branch edge has no z3 condition and no condition_subject")

    kind = subject.get("kind")
    if kind == "var":
        name = subject.get("z3_name")

        if name is None:
            name = _fallback_z3_name_for_var(subject)

        values = getattr(prepared, "values", {})
        symbols = getattr(prepared, "symbols", {})

        if name in values:
            return values[name]
        elif name in symbols:
            return symbols[name]

        raise RuntimeError(f"condition variable was not prepared for z3: {name}")

    raise RuntimeError(f"cannot rebuild condition from subject kind={kind!r}: {subject}")

def _fallback_z3_name_for_var(subject: dict[str, Any]) -> str:
    ty = subject.get("ty") or "unknown"
    storage = subject.get("storage") or "var"
    var_id = subject.get("var_id")
    ptr = int(subject.get("ptr") or 0)
    ptr_suffix = "" if ptr == 0 else "_p" + str(ptr)
    return f"{ty}_{storage}{ptr_suffix}_{var_id}"

def _block_by_name(cfg: Any, name: str) -> Any:
    return cfg.block_by_name(name)

def _terminal_reason(block: Any) -> str | None:
    terminator = getattr(block, "terminator", None)
    if terminator is None:
        edges = getattr(block, "edges", [])
        if not edges:
            return "dead_end"

        return None

    op = terminator.get("op")

    if op == "return":
        return "return"
    elif op == "exit":
        return "exit"

    edges = getattr(block, "edges", [])
    if not edges:
        return f"terminal_{op}"

    return None

def _enumerate_terminal_paths(cfg: Any, prepared: Any, *, max_depth: int) -> list[PathState]:
    if getattr(cfg, "entry", None) is None:
        return []

    results: list[PathState] = []
    stack: list[PathState] = [
        PathState(
            block_name=cfg.entry,
            path_blocks=[cfg.entry],
            depth=0,
        )
    ]

    while stack:
        state = stack.pop()
        block = _block_by_name(cfg, state.block_name)
        reason = _terminal_reason(block)

        if reason is not None:
            state.end_reason = reason
            results.append(state)
            continue

        if state.depth >= max_depth:
            state.end_reason = "max_depth"
            results.append(state)
            continue

        edges = list(getattr(block, "edges", []))
        if not edges:
            state.end_reason = "dead_end"
            results.append(state)
            continue

        for edge in reversed(edges):
            dst = getattr(edge, "dst", None)

            if dst is None:
                unresolved = PathState(
                    block_name=state.block_name,
                    path_blocks=list(state.path_blocks),
                    path_edges=list(state.path_edges) + [edge],
                    conditions=list(state.conditions),
                    depth=state.depth + 1,
                    end_reason="unresolved_edge",
                )
                
                results.append(unresolved)
                continue

            condition = _edge_condition(edge, prepared)
            stack.append(
                PathState(
                    block_name=dst,
                    path_blocks=list(state.path_blocks) + [dst],
                    path_edges=list(state.path_edges) + [edge],
                    conditions=list(state.conditions) + [condition],
                    depth=state.depth + 1,
                )
            )

    return results

def _enumerate_paths_to_block(cfg: Any, prepared: Any, *, target_block: str, max_depth: int) -> list[PathState]:
    if getattr(cfg, "entry", None) is None:
        return []

    results: list[PathState] = []
    stack: list[PathState] = [
        PathState(
            block_name=cfg.entry,
            path_blocks=[cfg.entry],
            depth=0,
        )
    ]

    while stack:
        state = stack.pop()

        if state.block_name == target_block:
            state.end_reason = "target_reached"
            results.append(state)
            continue

        if state.depth >= max_depth:
            continue

        block = _block_by_name(cfg, state.block_name)
        edges = list(getattr(block, "edges", []))

        for edge in reversed(edges):
            dst = getattr(edge, "dst", None)
            if dst is None:
                continue

            condition = _edge_condition(edge, prepared)
            stack.append(
                PathState(
                    block_name=dst,
                    path_blocks=list(state.path_blocks) + [dst],
                    path_edges=list(state.path_edges) + [edge],
                    conditions=list(state.conditions) + [condition],
                    depth=state.depth + 1,
                )
            )

    return results

def _find_block_for_label(cfg: Any, label: str) -> str | None:
    label_to_block = getattr(cfg, "label_to_block", {})
    if label in label_to_block:
        return label_to_block[label]
    if label.isdigit():
        candidate = f"lb{label}"
        if candidate in label_to_block:
            return label_to_block[candidate]

    for block in getattr(cfg, "blocks", []):
        if getattr(block, "name", None) == label:
            return label

    return None

def _find_block_for_line(cfg: Any, line_no: int) -> str | None:
    for block in getattr(cfg, "blocks", []):
        for instr in getattr(block, "instructions", []):
            if int(instr.get("line_no") or -1) == line_no:
                return block.name

    return None

def _return_value_for_block(prepared: Any, block: Any) -> z3.ExprRef | None:
    terminator = getattr(block, "terminator", None)
    if not terminator or terminator.get("op") != "return":
        return None

    line_no = int(terminator.get("line_no") or -1)
    for ret in getattr(prepared, "returns", []):
        if int(getattr(ret, "line_no", -1)) == line_no:
            return getattr(ret, "value", None)

    return None

def _parse_int_literal(text: str) -> int:
    return int(text, 0)

def _parse_bool_literal(text: str) -> bool:
    lowered = text.strip().lower()
    if lowered in {"true", "yes", "on"}:
        return True
    elif lowered in {"false", "no", "off"}:
        return False

    return _parse_int_literal(lowered) != 0

def _value_for_sort(value_text: str, sort: z3.SortRef) -> z3.ExprRef:
    if sort.kind() == z3.Z3_BOOL_SORT:
        return z3.BoolVal(_parse_bool_literal(value_text))
    elif sort.kind() == z3.Z3_BV_SORT:
        return z3.BitVecVal(_parse_int_literal(value_text), sort.size())
    elif sort.kind() == z3.Z3_INT_SORT:
        return z3.IntVal(_parse_int_literal(value_text))
    elif sort.kind() == z3.Z3_REAL_SORT:
        return z3.RealVal(value_text)

    raise RuntimeError(f"cannot build literal {value_text!r} for sort {sort}")

_VAR_NAME_RE = re.compile(r"^(?P<ty>[A-Za-z0-9]+)_(?P<storage>tmp|stack|global)(?:_p(?P<ptr>\d+))?_(?P<id>\d+)$")

def _find_var_candidates(prepared: Any, var_id: int, *, ty: str | None, storage: str | None) -> list[tuple[str, z3.ExprRef]]:
    merged: dict[str, z3.ExprRef] = {}

    for name, expr in getattr(prepared, "symbols", {}).items():
        merged[name] = expr
    for name, expr in getattr(prepared, "values", {}).items():
        merged[name] = expr

    result: list[tuple[str, z3.ExprRef]] = []

    for name, expr in sorted(merged.items()):
        match = _VAR_NAME_RE.fullmatch(name)

        if match is None:
            continue
        elif int(match.group("id")) != var_id:
            continue
        elif ty is not None and match.group("ty") != ty:
            continue
        elif storage is not None and match.group("storage") != storage:
            continue

        result.append((name, expr))

    return result

def _investigate_branches(ctx: WrapperContext) -> list[CheckResult]:
    results: list[CheckResult] = []
    for block in ctx.cfg.blocks:
        for edge in block.edges:
            if edge.kind not in { "true", "false" }:
                continue

            condition = _edge_condition(edge, ctx.prepared)
            check, model = _check_with_conditions(ctx.prepared, [condition])
            title = (
                f"{block.name} --{edge.kind}/{edge.label}--> "
                f"{edge.dst if edge.dst is not None else '<unresolved>'}"
            )

            results.append(
                CheckResult(
                    title=title,
                    check=check,
                    model=model,
                    condition=condition,
                    extra={
                        "src": block.name,
                        "dst": edge.dst,
                        "edge_kind": edge.kind,
                        "label": edge.label,
                        "line_no": edge.line_no,
                    },
                )
            )

    return results

def _investigate_paths(ctx: WrapperContext, *, max_depth: int) -> list[CheckResult]:
    path_states = _enumerate_terminal_paths(
        ctx.cfg,
        ctx.prepared,
        max_depth=max_depth,
    )

    results: list[CheckResult] = []
    for index, path in enumerate(path_states, 1):
        condition = path.condition_expr()
        check, model = _check_with_conditions(ctx.prepared, path.conditions)
        end_block = _block_by_name(ctx.cfg, path.block_name)
        return_value = _return_value_for_block(ctx.prepared, end_block)

        results.append(
            CheckResult(
                title=f"path #{index}: {' -> '.join(path.path_blocks)}",
                check=check,
                model=model,
                condition=condition,
                path=path,
                extra={
                    "end_reason": path.end_reason,
                    "return_value": str(return_value) if return_value is not None else None,
                },
            )
        )

    return results

def _investigate_label(ctx: WrapperContext, *, label: str, max_depth: int) -> list[CheckResult]:
    target_block = _find_block_for_label(ctx.cfg, label)
    if target_block is None:
        raise RuntimeError(f"unknown label/block: {label!r}")

    path_states = _enumerate_paths_to_block(
        ctx.cfg,
        ctx.prepared,
        target_block=target_block,
        max_depth=max_depth,
    )

    results: list[CheckResult] = []
    for index, path in enumerate(path_states, 1):
        condition = path.condition_expr()
        check, model = _check_with_conditions(ctx.prepared, path.conditions)

        results.append(
            CheckResult(
                title=f"label {label} path #{index}: {' -> '.join(path.path_blocks)}",
                check=check,
                model=model,
                condition=condition,
                path=path,
                extra={
                    "target_block": target_block,
                },
            )
        )

    return results

def _investigate_line(ctx: WrapperContext, *, line_no: int, max_depth: int) -> list[CheckResult]:
    target_block = _find_block_for_line(ctx.cfg, line_no)

    if target_block is None:
        raise RuntimeError(f"no CFG block contains line_no={line_no}")

    path_states = _enumerate_paths_to_block(
        ctx.cfg,
        ctx.prepared,
        target_block=target_block,
        max_depth=max_depth,
    )

    results: list[CheckResult] = []
    for index, path in enumerate(path_states, 1):
        condition = path.condition_expr()
        check, model = _check_with_conditions(ctx.prepared, path.conditions)
        results.append(
            CheckResult(
                title=f"line {line_no} path #{index}: {' -> '.join(path.path_blocks)}",
                check=check,
                model=model,
                condition=condition,
                path=path,
                extra={
                    "target_block": target_block,
                },
            )
        )

    return results

def _investigate_var_eq(
    ctx: WrapperContext, *, var_id: int, 
    value_text: str, ty: str | None, storage: str | None, max_depth: int
) -> list[CheckResult]:
    candidates = _find_var_candidates(
        ctx.prepared,
        var_id,
        ty=ty,
        storage=storage,
    )

    if not candidates:
        raise RuntimeError(f"variable id %{var_id} was not found")

    paths = _enumerate_terminal_paths(
        ctx.cfg,
        ctx.prepared,
        max_depth=max_depth,
    )

    results: list[CheckResult] = []
    for name, expr in candidates:
        value = _value_for_sort(value_text, expr.sort())
        eq_condition = expr == value
        neq_condition = expr != value

        can_equal = False
        can_differ = False
        sat_path_count = 0
        first_model: z3.ModelRef | None = None

        for path in paths:
            base_check, _ = _check_path_with_conditions(ctx, path, [])
            if base_check != z3.sat:
                continue

            sat_path_count += 1

            eq_check, eq_model = _check_path_with_conditions(ctx, path, [eq_condition])
            if eq_check == z3.sat:
                can_equal = True

                if first_model is None:
                    first_model = eq_model

            neq_check, _ = _check_path_with_conditions(ctx, path, [neq_condition])
            if neq_check == z3.sat:
                can_differ = True

        if sat_path_count == 0:
            status = "inconsistent"
            check = z3.unsat
        elif can_equal and not can_differ:
            status = "must_equal"
            check = z3.sat
        elif can_equal and can_differ:
            status = "may_equal"
            check = z3.sat
        elif not can_equal and can_differ:
            status = "cannot_equal"
            check = z3.unsat
        else:
            status = "unknown"
            check = z3.unknown

        results.append(
            CheckResult(
                title=f"{name} == {value_text}",
                check=check,
                model=first_model,
                condition=eq_condition,
                extra={
                    "variable": name,
                    "value": value_text,
                    "status": status,
                    "can_equal": str(z3.sat if can_equal else z3.unsat),
                    "can_differ": str(z3.sat if can_differ else z3.unsat),
                    "sat_paths": sat_path_count,
                    "sort": str(expr.sort()),
                },
            )
        )

    return results

def _print_constraints(ctx: WrapperContext) -> None:
    print(f";; function {ctx.function.name}")
    print(";; constraints")

    for constraint in ctx.prepared.constraints:
        print(" ", constraint)

    if ctx.initial_assignments:
        print(";; initial assignments")

        for assignment in ctx.initial_assignments:
            print(f"  {assignment.variable} = {assignment.value}")
            print(f"  {assignment.condition}")

        print()
        print(";; generated constraints")

    print()
    print(";; branches")

    for branch in getattr(ctx.prepared, "branches", []):
        print(
            f"  line {branch.line_no}: if {branch.cond} "
            f"then {branch.true_label} else {branch.false_label}"
        )

    print()
    print(";; returns")

    for ret in getattr(ctx.prepared, "returns", []):
        print(f"  line {ret.line_no}: return {ret.value}")

    print()
    print(";; deferred")

    for item in getattr(ctx.prepared, "deferred", []):
        instr = item.get("instruction", {})
        print(f"  {item.get('reason')}: {instr.get('raw')}")

def _print_results(results: list[CheckResult], *, show_model: bool) -> None:
    if not results:
        print("no results")
        return

    for result in results:
        print(result.title)
        print(f"  check: {result.check}")

        if result.condition is not None:
            print(f"  path condition: {result.condition}")

        if result.extra:
            for key, value in result.extra.items():
                if value is not None:
                    print(f"  {key}: {value}")

        if show_model and result.model is not None:
            print("  model:")

            for line in _format_model_lines(result.model):
                print(f"    {line}")

        print()

def _format_model_lines(model: z3.ModelRef) -> list[str]:
    lines: list[str] = []

    declarations = sorted(
        model.decls(),
        key=lambda declaration: declaration.name(),
    )

    for declaration in declarations:
        lines.append(f"{declaration.name()} = {model[declaration]}")

    return lines

def _results_to_jsonable(results: list[CheckResult], *, include_model: bool) -> list[dict[str, Any]]:
    output: list[dict[str, Any]] = []

    for result in results:
        item: dict[str, Any] = {
            "title": result.title,
            "check": str(result.check),
            "condition": str(result.condition) if result.condition is not None else None,
            "extra": result.extra,
        }

        if result.path is not None:
            item["path"] = {
                "blocks": result.path.path_blocks,
                "end_reason": result.path.end_reason,
                "conditions": [str(condition) for condition in result.path.conditions],
            }
        if include_model and result.model is not None:
            item["model"] = {
                declaration.name(): str(result.model[declaration])
                for declaration in sorted(result.model.decls(), key=lambda declaration: declaration.name())
            }

        output.append(item)

    return output

def _build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="CPL HIR to z3")
    parser.add_argument("input", help="HIR dump file or parsed JSON file.")
    parser.add_argument("-f", "--function", default=None, help="Function name to investigate.")
    parser.add_argument("--set", dest="sets", action="append", default=[], help="Set initial function variable value. Example: --set %%1=10 or --set i32_tmp_1=10.")
    parser.add_argument(
        "what",
        choices=[ "cfg", "dot", "constraints", "branches", "paths", "label", "line", "var-eq" ],
        help="What to investigate.",
    )
    parser.add_argument("targets", nargs="*", help="Targets for selected mode.")
    parser.add_argument("--input-kind", choices=["auto", "dump", "json"], default="auto", help="Input format.")
    parser.add_argument("--ptr-bits", type=int, default=64, help="Pointer size for z3 layer.")
    parser.add_argument("--max-depth", type=int, default=None, help="Max CFG exploration depth.")
    parser.add_argument("--no-model", action="store_true", help="Do not print z3 model.")
    parser.add_argument("--json", action="store_true", help="Print machine-readable JSON.")
    parser.add_argument("--strict-parser", action="store_true", help="Parser fails on unknown HIR lines.")
    parser.add_argument("--strict-z3", action="store_true", help="z3 preparation fails on unsupported operations.")
    parser.add_argument("--ty", default=None, help="Filter variable by HIR type.")
    parser.add_argument("--storage", choices=["tmp", "stack", "global"], default=None, help="Filter variable by storage class.")
    return parser

def _validate_args(parser: argparse.ArgumentParser, args: argparse.Namespace) -> None:
    if args.what in {"label", "line"} and len(args.targets) != 1:
        parser.error(f"{args.what!r} mode requires exactly one target")
    if args.what == "var-eq" and len(args.targets) != 2:
        parser.error("'var-eq' mode requires variable id and value")

def _dispatch(ctx: WrapperContext, args: argparse.Namespace, *, max_depth: int) -> list[CheckResult]:
    if args.what == "branches":
        return _investigate_branches(ctx)
    elif args.what == "paths":
        return _investigate_paths(
            ctx,
            max_depth=max_depth,
        )
    elif args.what == "label":
        return _investigate_label(
            ctx,
            label=str(args.targets[0]),
            max_depth=max_depth,
        )
    elif args.what == "line":
        try:
            line_no = int(str(args.targets[0]), 10)
        except ValueError as exception:
            raise SystemExit(f"line target must be integer, got: {args.targets[0]!r}") from exception

        return _investigate_line(
            ctx,
            line_no=line_no,
            max_depth=max_depth,
        )
    elif args.what == "var-eq":
        try:
            var_id = int(str(args.targets[0]).lstrip("%"), 10)
        except ValueError as exception:
            raise SystemExit(f"variable id must be integer or %integer, got: {args.targets[0]!r}") from exception

        return _investigate_var_eq(
            ctx,
            var_id=var_id,
            value_text=str(args.targets[1]),
            ty=args.ty,
            storage=args.storage,
            max_depth=max_depth,
        )

    raise AssertionError(f"unhandled mode: {args.what}")

def _main(argv: Iterable[str] | None = None) -> int:
    parser = _build_arg_parser()
    args = parser.parse_args(list(argv) if argv is not None else None)

    _validate_args(parser, args)

    ctx = _build_context(args)
    block_count = len(getattr(ctx.cfg, "blocks", []))
    max_depth = args.max_depth
    if max_depth is None:
        max_depth = max(16, block_count * 4)

    if args.what == "cfg":
        if args.json:
            print(
                json.dumps(
                    ctx.cfg.to_dict(include_instructions=True),
                    ensure_ascii=False,
                    indent=2,
                )
            )
        else:
            print(ctx.cfg.dump())

        return 0
    elif args.what == "dot":
        print(ctx.cfg.to_dot())
        return 0
    elif args.what == "constraints":
        _print_constraints(ctx)
        return 0

    results = _dispatch(ctx, args, max_depth=max_depth)
    if args.json:
        print(
            json.dumps(
                _results_to_jsonable(results, include_model=not args.no_model),
                ensure_ascii=False, indent=2
            )
        )
    else:
        _print_results(results, show_model=not args.no_model)

    return 0

if __name__ == "__main__":
    _main()
    