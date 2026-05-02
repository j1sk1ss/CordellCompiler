from __future__ import annotations

import z3
import re
import os
import json
import hashlib
import tempfile
import argparse

from typing import Any, Iterable
from dataclasses import dataclass, field

from hir_parser import parse_hir_dump
from hir_functions import split_hir_functions, select_hir_function
from z3_build import (
    BranchInfo,
    GotoInfo,
    PhiInfo,
    ReturnInfo,
    SideEffectInfo,
    Z3BaseInfo,
    DEFAULT_PTR_BITS,
    subject_sort,
    prepare_hir_for_z3,
)
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

_PROGRAM_CACHE_VERSION = 1
_ANALYSIS_CACHE_VERSION = 2

def _default_cache_dir() -> str:
    root = os.environ.get("XDG_CACHE_HOME")
    if root:
        return os.path.join(root, "z3_wrapper")
    return os.path.join(os.path.expanduser("~"), ".cache", "z3_wrapper")

def _program_cache_path(path: str, *, input_kind: str, strict_parser: bool, cache_dir: str) -> str:
    stat = os.stat(path)
    key_data = {
        "version": _PROGRAM_CACHE_VERSION,
        "path": os.path.realpath(path),
        "size": stat.st_size,
        "mtime_ns": stat.st_mtime_ns,
        "input_kind": input_kind,
        "strict_parser": strict_parser,
    }
    key_text = json.dumps(key_data, sort_keys=True, separators=(",", ":"))
    key = hashlib.sha256(key_text.encode("utf-8")).hexdigest()
    return os.path.join(cache_dir, key + ".json")

def _read_program_cache(cache_path: str) -> dict[str, Any] | None:
    try:
        with open(cache_path, "r", encoding="utf-8") as file:
            cached = json.load(file)
    except (OSError, json.JSONDecodeError):
        return None

    if cached.get("version") != _PROGRAM_CACHE_VERSION:
        return None

    program = cached.get("program")
    if not isinstance(program, dict):
        return None

    return program

def _write_program_cache(cache_path: str, program: dict[str, Any]) -> None:
    directory = os.path.dirname(cache_path)

    try:
        os.makedirs(directory, exist_ok=True)
        with tempfile.NamedTemporaryFile(
            "w",
            encoding="utf-8",
            dir=directory,
            prefix=".tmp-",
            suffix=".json",
            delete=False,
        ) as file:
            tmp_path = file.name
            json.dump(
                {
                    "version": _PROGRAM_CACHE_VERSION,
                    "program": program,
                },
                file,
                ensure_ascii=False,
            )

        os.replace(tmp_path, cache_path)
    except (OSError, TypeError):
        try:
            os.unlink(tmp_path)
        except (OSError, UnboundLocalError):
            pass

def _analysis_cache_path(
    path: str,
    *,
    input_kind: str,
    strict_parser: bool,
    function: str | None,
    ptr_bits: int,
    strict_z3: bool,
    cache_dir: str,
) -> str:
    stat = os.stat(path)
    key_data = {
        "version": _ANALYSIS_CACHE_VERSION,
        "path": os.path.realpath(path),
        "size": stat.st_size,
        "mtime_ns": stat.st_mtime_ns,
        "input_kind": input_kind,
        "strict_parser": strict_parser,
        "function": function,
        "ptr_bits": ptr_bits,
        "strict_z3": strict_z3,
    }
    key_text = json.dumps(key_data, sort_keys=True, separators=(",", ":"))
    key = hashlib.sha256(key_text.encode("utf-8")).hexdigest()
    return os.path.join(cache_dir, key + ".analysis.json")

def _z3_sort_to_cache(sort: z3.SortRef) -> dict[str, Any]:
    kind = sort.kind()

    if kind == z3.Z3_BOOL_SORT:
        return { "kind": "bool" }
    elif kind == z3.Z3_BV_SORT:
        return { "kind": "bv", "bits": sort.size() }
    elif kind == z3.Z3_INT_SORT:
        return { "kind": "int" }
    elif kind == z3.Z3_REAL_SORT:
        return { "kind": "real" }

    return { "kind": "uninterpreted", "name": str(sort.name()) }

def _z3_sort_from_cache(data: dict[str, Any]) -> z3.SortRef:
    return _z3_sort_from_cache_with_memo(data, {})

def _z3_sort_from_cache_with_memo(data: dict[str, Any], memo: dict[str, z3.SortRef]) -> z3.SortRef:
    kind = data["kind"]

    if kind == "bool":
        return z3.BoolSort()
    elif kind == "bv":
        return z3.BitVecSort(int(data["bits"]))
    elif kind == "int":
        return z3.IntSort()
    elif kind == "real":
        return z3.RealSort()
    elif kind == "uninterpreted":
        name = str(data["name"])
        if name not in memo:
            memo[name] = z3.DeclareSort(name)

        return memo[name]

    raise RuntimeError(f"unknown cached z3 sort: {kind!r}")

def _z3_expr_to_cache(expr: z3.ExprRef | None) -> dict[str, Any] | None:
    if expr is None:
        return None

    return {
        "sexpr": expr.sexpr(),
        "sort": _z3_sort_to_cache(expr.sort()),
    }

def _z3_symbols_to_cache(symbols: dict[str, z3.ExprRef]) -> dict[str, Any]:
    return {
        name: _z3_sort_to_cache(expr.sort())
        for name, expr in symbols.items()
    }

def _z3_symbols_from_cache(
    data: dict[str, Any],
    sort_memo: dict[str, z3.SortRef],
) -> dict[str, z3.ExprRef]:
    return {
        name: z3.Const(name, _z3_sort_from_cache_with_memo(sort_data, sort_memo))
        for name, sort_data in data.items()
    }

def _collect_z3_function_decls(expr: z3.ExprRef | None, result: dict[str, dict[str, Any]]) -> None:
    if expr is None:
        return

    decl = expr.decl()
    if decl.arity() > 0 and decl.kind() == z3.Z3_OP_UNINTERPRETED:
        name = str(decl.name())
        result[name] = {
            "name": name,
            "domain": [
                _z3_sort_to_cache(decl.domain(index))
                for index in range(decl.arity())
            ],
            "range": _z3_sort_to_cache(decl.range()),
        }

    for child in expr.children():
        _collect_z3_function_decls(child, result)

def _z3_function_decls_to_cache(prepared: Z3BaseInfo) -> list[dict[str, Any]]:
    result: dict[str, dict[str, Any]] = {}

    for constraint in prepared.constraints:
        _collect_z3_function_decls(constraint, result)
    for constraint in prepared.solver.assertions():
        _collect_z3_function_decls(constraint, result)
    for branch in prepared.branches:
        _collect_z3_function_decls(branch.cond, result)
    for item in prepared.returns:
        _collect_z3_function_decls(item.value, result)

    return list(result.values())

def _z3_parse_env(
    function_decls_data: list[dict[str, Any]],
    sort_memo: dict[str, z3.SortRef],
    *tables: dict[str, z3.ExprRef],
) -> tuple[dict[str, z3.SortRef], dict[str, z3.FuncDeclRef]]:
    symbols: dict[str, z3.ExprRef] = {}

    for table in tables:
        symbols.update(table)

    sorts: dict[str, z3.SortRef] = {}
    decls: dict[str, z3.FuncDeclRef] = {}

    for name, expr in symbols.items():
        decls[name] = expr.decl()
        sort = expr.sort()
        if sort.kind() not in {
            z3.Z3_BOOL_SORT,
            z3.Z3_BV_SORT,
            z3.Z3_INT_SORT,
            z3.Z3_REAL_SORT,
        }:
            sorts[str(sort.name())] = sort

    for item in function_decls_data:
        domain = [
            _z3_sort_from_cache_with_memo(sort_data, sort_memo)
            for sort_data in item.get("domain", [])
        ]
        result_sort = _z3_sort_from_cache_with_memo(item["range"], sort_memo)
        fn = z3.Function(str(item["name"]), *domain, result_sort)
        decls[str(item["name"])] = fn

        for sort in domain + [ result_sort ]:
            if sort.kind() not in {
                z3.Z3_BOOL_SORT,
                z3.Z3_BV_SORT,
                z3.Z3_INT_SORT,
                z3.Z3_REAL_SORT,
            }:
                sorts[str(sort.name())] = sort

    return sorts, decls

def _z3_expr_from_cache(
    data: dict[str, Any] | None,
    *,
    sorts: dict[str, z3.SortRef],
    decls: dict[str, z3.FuncDeclRef],
) -> z3.ExprRef | None:
    if data is None:
        return None

    sort = _z3_sort_from_cache_with_memo(data["sort"], sorts)
    tmp = z3.Const("__z3_cache_expr", sort)
    local_decls = dict(decls)
    local_decls["__z3_cache_expr"] = tmp.decl()

    parsed = z3.parse_smt2_string(
        f"(assert (= __z3_cache_expr {data['sexpr']}))",
        sorts=sorts,
        decls=local_decls,
    )

    if len(parsed) != 1:
        raise RuntimeError("failed to parse cached z3 expression")

    return parsed[0].children()[1]

def _z3_base_info_to_cache(prepared: Z3BaseInfo) -> dict[str, Any]:
    return {
        "solver_smt2": prepared.solver.to_smt2(),
        "symbols": _z3_symbols_to_cache(prepared.symbols),
        "values": _z3_symbols_to_cache(prepared.values),
        "functions": _z3_function_decls_to_cache(prepared),
        "labels": prepared.labels,
        "branches": [
            {
                "line_no": item.line_no,
                "cond": _z3_expr_to_cache(item.cond),
                "true_label": item.true_label,
                "false_label": item.false_label,
                "raw": item.raw,
            }
            for item in prepared.branches
        ],
        "gotos": [
            {
                "line_no": item.line_no,
                "target_label": item.target_label,
                "raw": item.raw,
            }
            for item in prepared.gotos
        ],
        "phis": [
            {
                "line_no": item.line_no,
                "dst": item.dst,
                "base": item.base,
                "arg": item.arg,
                "raw": item.raw,
            }
            for item in prepared.phis
        ],
        "returns": [
            {
                "line_no": item.line_no,
                "value": _z3_expr_to_cache(item.value),
                "raw": item.raw,
            }
            for item in prepared.returns
        ],
        "side_effects": [
            {
                "line_no": item.line_no,
                "op": item.op,
                "raw": item.raw,
                "data": item.data,
            }
            for item in prepared.side_effects
        ],
        "deferred": prepared.deferred,
    }

def _z3_base_info_from_cache(data: dict[str, Any]) -> Z3BaseInfo:
    solver = z3.Solver()
    solver.from_string(data["solver_smt2"])

    sort_memo: dict[str, z3.SortRef] = {}
    symbols = _z3_symbols_from_cache(data.get("symbols", {}), sort_memo)
    values = _z3_symbols_from_cache(data.get("values", {}), sort_memo)
    sorts, decls = _z3_parse_env(data.get("functions", []), sort_memo, symbols, values)

    return Z3BaseInfo(
        solver=solver,
        symbols=symbols,
        values=values,
        constraints=list(solver.assertions()),
        labels=dict(data.get("labels", {})),
        branches=[
            BranchInfo(
                line_no=int(item["line_no"]),
                cond=_z3_expr_from_cache(item["cond"], sorts=sorts, decls=decls),
                true_label=str(item["true_label"]),
                false_label=str(item["false_label"]),
                raw=str(item["raw"]),
            )
            for item in data.get("branches", [])
        ],
        gotos=[
            GotoInfo(
                line_no=int(item["line_no"]),
                target_label=str(item["target_label"]),
                raw=str(item["raw"]),
            )
            for item in data.get("gotos", [])
        ],
        phis=[
            PhiInfo(
                line_no=int(item["line_no"]),
                dst=item.get("dst") or {},
                base=item.get("base"),
                arg=item.get("arg"),
                raw=str(item["raw"]),
            )
            for item in data.get("phis", [])
        ],
        returns=[
            ReturnInfo(
                line_no=int(item["line_no"]),
                value=_z3_expr_from_cache(item.get("value"), sorts=sorts, decls=decls),
                raw=str(item["raw"]),
            )
            for item in data.get("returns", [])
        ],
        side_effects=[
            SideEffectInfo(
                line_no=int(item["line_no"]),
                op=str(item["op"]),
                raw=str(item["raw"]),
                data=item.get("data") or {},
            )
            for item in data.get("side_effects", [])
        ],
        deferred=list(data.get("deferred", [])),
    )

def _read_analysis_cache(cache_path: str) -> tuple[dict[str, Any], Z3BaseInfo] | None:
    try:
        with open(cache_path, "r", encoding="utf-8") as file:
            cached = json.load(file)
    except (OSError, json.JSONDecodeError):
        return None

    try:
        if cached.get("version") != _ANALYSIS_CACHE_VERSION:
            return None

        program = cached.get("program")
        prepared_data = cached.get("prepared")
        if not isinstance(program, dict):
            return None
        elif not isinstance(prepared_data, dict):
            return None

        return program, _z3_base_info_from_cache(prepared_data)
    except Exception:
        return None

def _write_analysis_cache(cache_path: str, program: dict[str, Any], prepared: Z3BaseInfo) -> None:
    directory = os.path.dirname(cache_path)

    try:
        os.makedirs(directory, exist_ok=True)
        with tempfile.NamedTemporaryFile(
            "w",
            encoding="utf-8",
            dir=directory,
            prefix=".tmp-",
            suffix=".json",
            delete=False,
        ) as file:
            tmp_path = file.name
            json.dump(
                {
                    "version": _ANALYSIS_CACHE_VERSION,
                    "program": program,
                    "prepared": _z3_base_info_to_cache(prepared),
                },
                file,
                ensure_ascii=False,
            )

        os.replace(tmp_path, cache_path)
    except (OSError, TypeError):
        try:
            os.unlink(tmp_path)
        except (OSError, UnboundLocalError):
            pass

def _load_program_from_file(
    path: str, *, input_kind: str, strict_parser: bool, cache_dir: str | None, no_cache: bool
) -> dict[str, Any]:
    if path == "-":
        import sys
        text = sys.stdin.read()
        return _load_program_from_text(
            text,
            input_kind=input_kind,
            strict_parser=strict_parser,
        )

    resolved_cache_dir = cache_dir or _default_cache_dir()
    cache_path = _program_cache_path(
        path,
        input_kind=input_kind,
        strict_parser=strict_parser,
        cache_dir=resolved_cache_dir,
    )

    if not no_cache:
        cached_program = _read_program_cache(cache_path)
        if cached_program is not None:
            return cached_program

    with open(path, "r", encoding="utf-8") as file:
        text = file.read()

    program = _load_program_from_text(text, input_kind=input_kind, strict_parser=strict_parser)
    if not no_cache:
        _write_program_cache(cache_path, program)

    return program

def _load_program_from_text(text: str, *, input_kind: str, strict_parser: bool) -> dict[str, Any]:
    if input_kind == "json":
        return json.loads(text)
    elif input_kind == "dump":
        return parse_hir_dump(text, strict=strict_parser).to_dict()
    elif input_kind == "auto":
        try:
            return json.loads(text)
        except Exception:
            pass

        try:
            return parse_hir_dump(text, strict=strict_parser).to_dict()
        except Exception:
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
    elif not value:
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
        candidates = _find_var_candidates(prepared, var_id, ty=None, storage=None)
    elif variable.isdigit():
        var_id = int(variable, 10)
        candidates = _find_var_candidates(prepared, var_id, ty=None, storage=None)
    else:
        candidates = [ (name, expr) for name, expr in merged.items() if name == variable ]
    
    if not candidates:
        raise RuntimeError(f"variable {variable!r} was not found")
    elif len(candidates) > 1:
        names = ", ".join(name for name, _ in candidates)
        raise RuntimeError(f"variable {variable!r} is ambiguous: {names}")

    return candidates[0]

def _build_initial_assignments(prepared: Any, assignments: list[str]) -> list[InitialAssignment]:
    result: list[InitialAssignment] = []
    for assignment_text in assignments:
        variable_spec, value_text = _parse_initial_assignment(assignment_text)
        variable_name, expr = _resolve_assignment_variable(prepared, variable_spec)
        value = _value_for_sort(value_text, expr.sort())
        result.append(InitialAssignment(variable=variable_name, value=value_text, condition=expr == value))

    return result

def _apply_initial_assignments(prepared: Any, assignments: list[InitialAssignment]) -> None:
    for assignment in assignments:
        prepared.constraints.append(assignment.condition)
        prepared.solver.add(assignment.condition)

def _build_context(args: argparse.Namespace) -> WrapperContext:
    analysis_cache_path: str | None = None
    resolved_cache_dir = args.cache_dir or _default_cache_dir()

    if args.input != "-":
        analysis_cache_path = _analysis_cache_path(
            args.input,
            input_kind=args.input_kind,
            strict_parser=args.strict_parser,
            function=args.function,
            ptr_bits=args.ptr_bits,
            strict_z3=args.strict_z3,
            cache_dir=resolved_cache_dir,
        )

        if not args.no_cache:
            cached = _read_analysis_cache(analysis_cache_path)
            if cached is not None:
                program, prepared = cached
                setattr(prepared, "ptr_bits", args.ptr_bits)
                _ensure_program_var_symbols(prepared, program)
                initial_assignments = _build_initial_assignments(prepared, args.sets)
                _apply_initial_assignments(prepared, initial_assignments)
                cfg = build_hir_cfg(program, prepared=prepared)

                return WrapperContext(
                    program=program,
                    function=None,
                    prepared=prepared,
                    cfg=cfg,
                    initial_assignments=initial_assignments,
                )

    full_program = _load_program_from_file(
        args.input,
        input_kind=args.input_kind,
        strict_parser=args.strict_parser,
        cache_dir=args.cache_dir,
        no_cache=args.no_cache,
    )
    functions = split_hir_functions(full_program)
    function = select_hir_function(functions, args.function)
    program = function.to_program()
    prepared = prepare_hir_for_z3(program, ptr_bits=args.ptr_bits, strict=args.strict_z3)
    setattr(prepared, "ptr_bits", args.ptr_bits)
    _ensure_program_var_symbols(prepared, program)

    if analysis_cache_path is not None and not args.no_cache:
        _write_analysis_cache(analysis_cache_path, program, prepared)

    initial_assignments = _build_initial_assignments(prepared, args.sets)
    _apply_initial_assignments(prepared, initial_assignments)

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
    return _ensure_var_subject_prepared(prepared, subject)

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


def _ensure_var_subject_prepared(prepared: Any, subject: dict[str, Any]) -> z3.ExprRef:
    if not subject or subject.get("kind") != "var":
        raise RuntimeError(f"expected variable subject, got: {subject!r}")

    name = subject.get("z3_name")
    if name is None:
        name = _fallback_z3_name_for_var(subject)

    values = getattr(prepared, "values", None)
    symbols = getattr(prepared, "symbols", None)
    if values is None or symbols is None:
        raise RuntimeError("prepared z3 info has no symbols/values tables")

    if name in values:
        return values[name]

    if name in symbols:
        expr = symbols[name]
        values[name] = expr
        return expr

    ptr_bits = int(getattr(prepared, "ptr_bits", DEFAULT_PTR_BITS))
    expr = z3.Const(name, subject_sort(subject, ptr_bits=ptr_bits))
    symbols[name] = expr
    values[name] = expr
    return expr


def _ensure_program_var_symbols(prepared: Any, program: dict[str, Any]) -> None:
    for subject in _iter_subjects(program):
        if subject.get("kind") == "var":
            _ensure_var_subject_prepared(prepared, subject)

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

def _investigate_label(ctx: WrapperContext, *, label: str, max_depth: int) -> list[CheckResult]:
    target_block = _find_block_for_label(ctx.cfg, label)
    if target_block is None:
        raise RuntimeError(f"unknown label: {label!r}")

    path_states = _enumerate_paths_to_block(ctx.cfg, ctx.prepared, target_block=target_block, max_depth=max_depth)
    results: list[CheckResult] = []
    for index, path in enumerate(path_states, 1):
        condition = path.condition_expr()
        check, model = _check_path_with_conditions(ctx, path, [])
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

    paths = _enumerate_terminal_paths(ctx.cfg, ctx.prepared, max_depth=max_depth)
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

def _build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="CPL HIR to z3")
    parser.add_argument("input", help="HIR dump file or parsed JSON file.")
    parser.add_argument("-f", "--function", default=None, help="Function name to investigate.")
    parser.add_argument("--set", dest="sets", action="append", default=[], help="Set initial function variable value. Example: --set %%1=10 or --set i32_tmp_1=10.")
    parser.add_argument("what", choices=[ "label", "var-eq" ], help="What to investigate.")
    parser.add_argument("targets", nargs="*", help="Targets for selected mode.")
    parser.add_argument("--input-kind", choices=["auto", "dump", "json"], default="auto", help="Input format.")
    parser.add_argument("--ptr-bits", type=int, default=64, help="Pointer size for z3 layer.")
    parser.add_argument("--max-depth", type=int, default=None, help="Max CFG exploration depth.")
    parser.add_argument("--no-model", action="store_true", help="Do not print z3 model.")
    parser.add_argument("--json", action="store_true", help="Print machine-readable JSON.")
    parser.add_argument("--strict-parser", action="store_true", help="Parser fails on unknown HIR lines.")
    parser.add_argument("--strict-z3", action="store_true", help="z3 preparation fails on unsupported operations.")
    parser.add_argument("--ty", default=None, help="Filter variable by HIR type.")
    parser.add_argument("--storage", choices=[ "tmp", "stack", "global" ], default=None, help="Filter variable by storage class.")
    parser.add_argument("--cache-dir", default=None, help="Directory for parsed input cache.")
    parser.add_argument("--no-cache", action="store_true", help="Disable parsed input cache.")
    return parser

def _validate_args(parser: argparse.ArgumentParser, args: argparse.Namespace) -> None:
    if args.what == "label" and len(args.targets) != 1:
        parser.error(f"{args.what!r} mode requires exactly one target")
    if args.what == "var-eq" and len(args.targets) != 2:
        parser.error("'var-eq' mode requires variable id and value")

def _dispatch(ctx: WrapperContext, args: argparse.Namespace, *, max_depth: int) -> list[CheckResult]:
    if args.what == "label":
        return _investigate_label(ctx, label=str(args.targets[0]), max_depth=max_depth)
    elif args.what == "var-eq":
        try:
            var_id = int(str(args.targets[0]).lstrip("%"), 10)
        except ValueError as exception:
            raise SystemExit(f"variable id must be integer or %integer, got: {args.targets[0]!r}") from exception
        return _investigate_var_eq(
            ctx, var_id=var_id, value_text=str(args.targets[1]), ty=args.ty, storage=args.storage, max_depth=max_depth
        )

    raise AssertionError(f"unhandled mode: {args.what}")

def _var_eq_exit_code(results: list[CheckResult]) -> int:
    statuses = [ result.extra.get("status") for result in results ]
    if any(status == "must_equal" for status in statuses):
        return 1
    elif any(status == "may_equal" for status in statuses):
        return 2
    elif statuses and all(status == "cannot_equal" for status in statuses):
        return 0

    return 3

def _label_exit_code(results: list[CheckResult]) -> int:
    if any(result.check == z3.sat for result in results):
        return 1

    return 0

def _main(argv: Iterable[str] | None = None) -> int:
    parser = _build_arg_parser()
    args = parser.parse_args(list(argv) if argv is not None else None)

    _validate_args(parser, args)

    ctx = _build_context(args)
    block_count = len(getattr(ctx.cfg, "blocks", []))
    max_depth = args.max_depth
    if max_depth is None:
        max_depth = max(16, block_count * 4)

    results = _dispatch(ctx, args, max_depth=max_depth)
    if args.what == "label":
        return _label_exit_code(results)
    elif args.what == "var-eq":
        return _var_eq_exit_code(results)
    return 0

if __name__ == "__main__":
    raise SystemExit(_main())
    