from __future__ import annotations

import z3
import json

from typing import Any
from dataclasses import dataclass, field

INT_WIDTHS: dict[str, int] = { "i0": 1, "i8": 8, "u8": 8, "i16": 16, "u16": 16, "i32": 32, "u32": 32, "i64": 64, "u64": 64 }
FLOAT_TYPES: set[str]      = { "f32", "f64" }
OPAQUE_TYPES: set[str]     = { "str", "arr" }
DEFAULT_PTR_BITS = 64

class HIRZ3Error(RuntimeError):
    pass

def is_int_ty(ty: str | None) -> bool:
    return ty in INT_WIDTHS and ty != "i0"

def is_unsigned_ty(ty: str | None) -> bool:
    return ty is not None and ty.startswith("u")

def is_float_ty(ty: str | None) -> bool:
    return ty in FLOAT_TYPES

def bit_width(ty: str | None) -> int:
    if ty not in INT_WIDTHS:
        raise HIRZ3Error(f"not an integer/bitvector type: {ty!r}")
    return INT_WIDTHS[ty]

def subject_sort(subject: dict[str, Any], *, ptr_bits: int = DEFAULT_PTR_BITS) -> z3.SortRef:
    ty = subject.get("ty")
    ptr = int(subject.get("ptr") or 0)

    if ptr > 0:
        return z3.BitVecSort(ptr_bits)
    elif ty == "i0":
        return z3.BoolSort()
    elif is_int_ty(ty):
        return z3.BitVecSort(bit_width(ty))
    elif is_float_ty(ty):
        return z3.RealSort()
    elif ty == "str":
        return z3.DeclareSort("HIR_str")
    elif ty == "arr":
        return z3.DeclareSort("HIR_arr")
    elif ty == "unknown":
        return z3.DeclareSort("HIR_unknown")

    return z3.DeclareSort("HIR_opaque")

def safe_symbol_name(name: str) -> str:
    out: list[str] = []
    for ch in name:
        if ch.isalnum() or ch == "_":
            out.append(ch)
        else:
            out.append("_")

    result = "".join(out).strip("_")
    return result or "unnamed"

def subject_display_name(subject: dict[str, Any]) -> str:
    if subject.get("kind") == "var":
        if subject.get("z3_name"):
            return str(subject["z3_name"])

        ty = subject.get("ty") or "unknown"
        storage = subject.get("storage") or "var"
        var_id = subject.get("var_id")
        ptr = int(subject.get("ptr") or 0)
        ptr_suffix = "" if ptr == 0 else "_p" + str(ptr)

        if var_id is not None:
            return f"{ty}_{storage}{ptr_suffix}_{var_id}"

    if subject.get("name"):
        return str(subject["name"])
    elif subject.get("text"):
        return str(subject["text"])

    return "unnamed"

@dataclass
class BranchInfo:
    line_no: int
    cond: z3.ExprRef
    true_label: str
    false_label: str
    raw: str

@dataclass
class GotoInfo:
    line_no: int
    target_label: str
    raw: str

@dataclass
class PhiInfo:
    line_no: int
    dst: dict[str, Any]
    base: dict[str, Any] | None
    arg: dict[str, Any] | None
    raw: str

@dataclass
class ReturnInfo:
    line_no: int
    value: z3.ExprRef | None
    raw: str

@dataclass
class SideEffectInfo:
    line_no: int
    op: str
    raw: str
    data: dict[str, Any] = field(default_factory=dict)

@dataclass
class Z3BaseInfo:
    solver: z3.Solver
    symbols: dict[str, z3.ExprRef]     = field(default_factory=dict)
    values: dict[str, z3.ExprRef]      = field(default_factory=dict)
    constraints: list[z3.BoolRef]      = field(default_factory=list)
    labels: dict[str, int]             = field(default_factory=dict)
    branches: list[BranchInfo]         = field(default_factory=list)
    gotos: list[GotoInfo]              = field(default_factory=list)
    phis: list[PhiInfo]                = field(default_factory=list)
    returns: list[ReturnInfo]          = field(default_factory=list)
    side_effects: list[SideEffectInfo] = field(default_factory=list)
    deferred: list[dict[str, Any]]     = field(default_factory=list)

    def add_constraint(self, constraint: z3.BoolRef) -> None:
        self.constraints.append(constraint)
        self.solver.add(constraint)

class Z3Builder:
    def __init__(
        self, program: dict[str, Any] | str, *,
        ptr_bits: int = DEFAULT_PTR_BITS, strict: bool = False
    ) -> None:
        if isinstance(program, str):
            program = json.loads(program)

        self.program = program
        self.ptr_bits = ptr_bits
        self.strict = strict
        self.prepared = Z3BaseInfo(solver=z3.Solver())
        self._fresh_counter = 0
        self._functions: dict[tuple[str, tuple[str, ...], str], z3.FuncDeclRef] = {}

    def prepare(self) -> Z3BaseInfo:
        for instr in self.program.get("instructions", []):
            self.prepare_instruction(instr)

        return self.prepared

    def expr_of_subject(self, subject: dict[str, Any]) -> z3.ExprRef:
        kind = subject.get("kind")
        if kind == "var":
            return self.var_expr(subject)
        elif kind in { "number", "const" }:
            return self.const_expr(subject)
        elif kind in { "symbol", "raw" }:
            return self.opaque_symbol(subject)
        elif kind in { "empty", "arglist", "phiset", "label" }:
            raise HIRZ3Error("The subject cannot be converted to z3 expression")

        return self.opaque_symbol(subject)

    def var_expr(self, subject: dict[str, Any]) -> z3.ExprRef:
        name = subject_display_name(subject)
        if name in self.prepared.symbols:
            return self.prepared.symbols[name]

        sort = subject_sort(subject, ptr_bits=self.ptr_bits)
        symbol = z3.Const(name, sort)
        self.prepared.symbols[name] = symbol
        self.prepared.values[name] = symbol
        return symbol

    def const_expr(self, subject: dict[str, Any]) -> z3.ExprRef:
        ty = subject.get("ty")
        value = subject.get("value")

        if ty == "i0":
            return z3.BoolVal(_parse_bool_value(value))
        elif is_int_ty(ty):
            width = bit_width(ty)
            return z3.BitVecVal(_parse_int_value(value), width)
        elif is_float_ty(ty):
            return z3.RealVal(str(value))

        name = safe_symbol_name(f"const_{subject.get('text', value)}")
        return z3.Const(name, subject_sort(subject, ptr_bits=self.ptr_bits))

    def opaque_symbol(self, subject: dict[str, Any]) -> z3.ExprRef:
        name = safe_symbol_name(subject_display_name(subject))
        if name in self.prepared.symbols:
            return self.prepared.symbols[name]

        sort = subject_sort(subject, ptr_bits=self.ptr_bits)
        symbol = z3.Const(name, sort)
        self.prepared.symbols[name] = symbol
        return symbol

    def fresh_value(self, prefix: str, sort: z3.SortRef) -> z3.ExprRef:
        self._fresh_counter += 1
        name = safe_symbol_name(f"{prefix}_{self._fresh_counter}")
        symbol = z3.Const(name, sort)
        self.prepared.symbols[name] = symbol
        return symbol

    def assign_dst(self, dst_subject: dict[str, Any], value: z3.ExprRef) -> z3.BoolRef:
        dst = self.var_expr(dst_subject)
        value = self.coerce_to_sort(value, dst.sort(), dst_subject=dst_subject)
        constraint = dst == value
        self.prepared.values[subject_display_name(dst_subject)] = dst
        self.prepared.add_constraint(constraint)
        return constraint

    def prepare_instruction(self, instr: dict[str, Any]) -> None:
        op = instr.get("op")
        try:
            if op == "label":
                self.lower_label(instr)
                return
            elif op == "goto":
                self.lower_goto(instr)
                return
            elif op == "if":
                self.lower_if(instr)
                return
            elif op == "return":
                self.lower_return(instr)
                return
            elif op in { "phi", "phi_preamble" }:
                self.lower_phi(instr)
                return
            elif op in { "assign", "load_arg", "load_starg" }:
                self.lower_assign_like(instr)
                return
            elif op == "binary":
                self.lower_binary(instr)
                return
            elif op == "not":
                self.lower_not(instr)
                return
            elif op == "cast":
                self.lower_cast(instr)
                return
            elif op in { "store_call", "store_syscall" }:
                self.lower_call_with_result(instr)
                return
            elif op in { "call", "syscall" }:
                self.lower_call_without_result(instr)
                return
            elif op in {
                "local_alloc", "global_alloc", "arr_alloc", "str_alloc", "ref", "load_ref",
            }:
                self.lower_fresh_result(instr)
                return
            elif op == "store_ref":
                self.lower_store_ref(instr)
                return
            elif op in { 
                "extern_func", "extern_var", "fn", "use", "breakpoint", "exit",
                "start", "scope_start", "scope_end", "break", "raw", "asm_start"
            }:
                self.defer(instr, reason="ignored instruction")
                return
            else:
                self.defer(instr, reason="unsupported unstruction")
                if self.strict:
                    raise HIRZ3Error(f"unsupported instruction op={op!r}: {instr!r}")
        except Exception as exc:
            if self.strict:
                raise

            self.prepared.deferred.append(
                {
                    "reason": f"{type(exc).__name__}: {exc}",
                    "instruction": instr,
                }
            )

    def lower_label(self, instr: dict[str, Any]) -> None:
        label = instr.get("label") or {}
        name = str(label.get("name") or label.get("text"))
        self.prepared.labels[name] = int(instr.get("line_no") or -1)

    def lower_goto(self, instr: dict[str, Any]) -> None:
        target = instr.get("target") or {}
        label_name = str(target.get("name") or target.get("text"))
        self.prepared.gotos.append(
            GotoInfo(
                line_no=int(instr.get("line_no") or -1),
                target_label=label_name,
                raw=str(instr.get("raw") or ""),
            )
        )

    def lower_if(self, instr: dict[str, Any]) -> None:
        cond_subject = instr["cond"]
        true_label_subject = instr["true_label"]
        false_label_subject = instr["false_label"]

        cond = self.as_bool(self.expr_of_subject(cond_subject))

        true_label = str(true_label_subject.get("name") or true_label_subject.get("text"))
        false_label = str(false_label_subject.get("name") or false_label_subject.get("text"))

        self.prepared.branches.append(
            BranchInfo(
                line_no=int(instr.get("line_no") or -1), cond=cond, true_label=true_label, false_label=false_label,
                raw=str(instr.get("raw") or "")
            )
        )

    def lower_return(self, instr: dict[str, Any]) -> None:
        arg = instr.get("arg")
        value = self.expr_of_subject(arg) if arg else None

        self.prepared.returns.append(
            ReturnInfo(line_no=int(instr.get("line_no") or -1), value=value, raw=str(instr.get("raw") or ""))
        )

    def lower_phi(self, instr: dict[str, Any]) -> None:
        self.prepared.phis.append(
            PhiInfo(
                line_no=int(instr.get("line_no") or -1),
                dst=instr.get("dst") or {},
                base=instr.get("base"),
                arg=instr.get("arg"),
                raw=str(instr.get("raw") or ""),
            )
        )

        self.defer(instr, reason="phi needs CFG predecessor/path context before lowering to z3")

    def lower_assign_like(self, instr: dict[str, Any]) -> None:
        dst_subject = instr["dst"]
        op = instr.get("op")

        if op in { "load_arg", "load_starg" }:
            dst_sort = subject_sort(dst_subject, ptr_bits=self.ptr_bits)
            prefix = f"{op}_{subject_display_name(dst_subject)}"
            value = self.fresh_value(prefix, dst_sort)
            self.assign_dst(dst_subject, value)
            return

        src_subject = instr["src"]
        value = self.expr_of_subject(src_subject)
        self.assign_dst(dst_subject, value)

    def lower_binary(self, instr: dict[str, Any]) -> None:
        dst_subject = instr["dst"]
        lhs_subject = instr["lhs"]
        rhs_subject = instr["rhs"]
        op = instr["text"]
        lhs = self.expr_of_subject(lhs_subject)
        rhs = self.expr_of_subject(rhs_subject)
        result = self.binary_expr(lhs, rhs, op, lhs_subject=lhs_subject, rhs_subject=rhs_subject)
        self.assign_dst(dst_subject, result)

    def lower_not(self, instr: dict[str, Any]) -> None:
        dst_subject = instr["dst"]
        src_subject = instr["src"]

        src = self.expr_of_subject(src_subject)
        if src.sort().kind() == z3.Z3_BOOL_SORT:
            result = z3.Not(src)
        elif src.sort().kind() == z3.Z3_BV_SORT:
            result = src == z3.BitVecVal(0, src.size())
        else:
            result = z3.Not(self.as_bool(src))

        self.assign_dst(dst_subject, result)

    def lower_cast(self, instr: dict[str, Any]) -> None:
        dst_subject = instr["dst"]
        src_subject = instr["src"]
        target_ty = instr.get("ty") or dst_subject.get("ty")

        src = self.expr_of_subject(src_subject)
        result = self.cast_expr(
            src,
            target_ty=target_ty,
            src_subject=src_subject,
            dst_subject=dst_subject,
        )

        self.assign_dst(dst_subject, result)

    def lower_call_with_result(self, instr: dict[str, Any]) -> None:
        dst_subject = instr["dst"]
        func_subject = instr.get("func") or { "kind": "symbol", "text": "unknown_call", "name": "unknown_call" }
        args_subjects = instr.get("args") or []

        args = [self.expr_of_subject(arg) for arg in args_subjects]
        result_sort = subject_sort(dst_subject, ptr_bits=self.ptr_bits)

        func_name = str(func_subject.get("name") or func_subject.get("text") or "unknown_call")
        result = self.call_expr(func_name, args, result_sort)

        self.assign_dst(dst_subject, result)

        self.prepared.side_effects.append(
            SideEffectInfo(
                op=str(instr.get("op")),
                line_no=int(instr.get("line_no") or -1),
                raw=str(instr.get("raw") or ""),
                data={
                    "func": func_name,
                    "argc": len(args),
                    "has_result": True,
                },
            )
        )

    def lower_call_without_result(self, instr: dict[str, Any]) -> None:
        func_subject = instr.get("func") or { "kind": "symbol", "text": "unknown_call", "name": "unknown_call" }
        args_subjects = instr.get("args") or []
        args = [self.expr_of_subject(arg) for arg in args_subjects]

        func_name = str(func_subject.get("name") or func_subject.get("text") or "unknown_call")

        self.prepared.side_effects.append(
            SideEffectInfo(
                op=str(instr.get("op")),
                line_no=int(instr.get("line_no") or -1),
                raw=str(instr.get("raw") or ""),
                data={
                    "func": func_name,
                    "argc": len(args),
                    "has_result": False,
                },
            )
        )

        self.defer(instr, reason="call without result has side effects only; no z3 value constraint emitted",)

    def lower_fresh_result(self, instr: dict[str, Any]) -> None:
        dst_subject = instr.get("dst")
        if not dst_subject:
            self.defer(instr, reason="fresh-result op without dst")
            return

        dst_sort = subject_sort(dst_subject, ptr_bits=self.ptr_bits)
        prefix = f"{instr.get('op')}_{subject_display_name(dst_subject)}"
        value = self.fresh_value(prefix, dst_sort)

        self.assign_dst(dst_subject, value)
        self.prepared.side_effects.append(
            SideEffectInfo(
                op=str(instr.get("op")),
                line_no=int(instr.get("line_no") or -1),
                raw=str(instr.get("raw") or ""),
                data={
                    "fresh_result": str(value),
                },
            )
        )

    def lower_store_ref(self, instr: dict[str, Any]) -> None:
        ptr = self.expr_of_subject(instr["dst"])
        value = self.expr_of_subject(instr["src"])

        self.prepared.side_effects.append(
            SideEffectInfo(
                op="store_ref",
                line_no=int(instr.get("line_no") or -1),
                raw=str(instr.get("raw") or ""),
                data={
                    "ptr": str(ptr),
                    "value": str(value),
                },
            )
        )

        self.defer(instr, reason="memory model is not implemented yet; store_ref recorded as side effect")

    def binary_expr(
        self,
        lhs: z3.ExprRef, rhs: z3.ExprRef, op: str,
        *,
        lhs_subject: dict[str, Any], rhs_subject: dict[str, Any]
    ) -> z3.ExprRef:
        lhs, rhs = self.align_binary_operands(lhs, rhs)
        if op in { "+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^" }:
            return self.binary_arith_or_bitwise(lhs, rhs, op, lhs_subject=lhs_subject)
        elif op in { ">", "<", ">=", "<=", "==", "!=" }:
            return self.binary_compare(lhs, rhs, op, lhs_subject=lhs_subject)
        elif op == "&&":
            return z3.And(self.as_bool(lhs), self.as_bool(rhs))
        elif op == "||":
            return z3.Or(self.as_bool(lhs), self.as_bool(rhs))
        raise HIRZ3Error(f"unsupported binary operator: {op!r}")

    def binary_arith_or_bitwise(
        self,
        lhs: z3.ExprRef, rhs: z3.ExprRef,
        op: str,
        *,
        lhs_subject: dict[str, Any]
    ) -> z3.ExprRef:
        if lhs.sort().kind() == z3.Z3_BOOL_SORT or rhs.sort().kind() == z3.Z3_BOOL_SORT:
            raise HIRZ3Error(f"operator {op!r} is not valid for Bool operands")
        elif lhs.sort().kind() == z3.Z3_BV_SORT:
            if op == "+":
                return lhs + rhs
            elif op == "-":
                return lhs - rhs
            elif op == "*":
                return lhs * rhs
            elif op == "/":
                return z3.UDiv(lhs, rhs) if is_unsigned_ty(lhs_subject.get("ty")) else lhs / rhs
            elif op == "%":
                return z3.URem(lhs, rhs) if is_unsigned_ty(lhs_subject.get("ty")) else lhs % rhs
            elif op == "<<":
                return lhs << rhs
            elif op == ">>":
                return z3.LShR(lhs, rhs) if is_unsigned_ty(lhs_subject.get("ty")) else lhs >> rhs
            elif op == "&":
                return lhs & rhs
            elif op == "|":
                return lhs | rhs
            elif op == "^":
                return lhs ^ rhs
        elif lhs.sort().kind() == z3.Z3_REAL_SORT:
            if op == "+":
                return lhs + rhs
            elif op == "-":
                return lhs - rhs
            elif op == "*":
                return lhs * rhs
            elif op == "/":
                return lhs / rhs

        raise HIRZ3Error(f"unsupported arithmetic/bitwise operation: {lhs.sort()} {op} {rhs.sort()}")

    def binary_compare(self, lhs: z3.ExprRef, rhs: z3.ExprRef, op: str, *, lhs_subject: dict[str, Any]) -> z3.BoolRef:
        if op == "==":
            return lhs == rhs
        elif op == "!=":
            return lhs != rhs
        
        if lhs.sort().kind() == z3.Z3_BV_SORT:
            unsigned = is_unsigned_ty(lhs_subject.get("ty"))
            if op == ">":
                return z3.UGT(lhs, rhs) if unsigned else lhs > rhs
            elif op == "<":
                return z3.ULT(lhs, rhs) if unsigned else lhs < rhs
            elif op == ">=":
                return z3.UGE(lhs, rhs) if unsigned else lhs >= rhs
            elif op == "<=":
                return z3.ULE(lhs, rhs) if unsigned else lhs <= rhs
        elif lhs.sort().kind() in {z3.Z3_REAL_SORT, z3.Z3_INT_SORT}:
            if op == ">":
                return lhs > rhs
            elif op == "<":
                return lhs < rhs
            elif op == ">=":
                return lhs >= rhs
            elif op == "<=":
                return lhs <= rhs

        raise HIRZ3Error(f"unsupported compare: {lhs.sort()} {op} {rhs.sort()}")

    def call_expr(self, func_name: str, args: list[z3.ExprRef], result_sort: z3.SortRef) -> z3.ExprRef:
        clean_name = safe_symbol_name(f"fn_{func_name}")
        arg_sorts = tuple(arg.sort() for arg in args)
        key = (
            clean_name,
            tuple(sort.sexpr() for sort in arg_sorts),
            result_sort.sexpr(),
        )

        fn = self._functions.get(key)
        if fn is None:
            fn = z3.Function(clean_name, *arg_sorts, result_sort)
            self._functions[key] = fn

        return fn(*args)

    def align_binary_operands(
        self,
        lhs: z3.ExprRef,
        rhs: z3.ExprRef,
    ) -> tuple[z3.ExprRef, z3.ExprRef]:
        if lhs.sort() == rhs.sort():
            return lhs, rhs
        elif lhs.sort().kind() == z3.Z3_BV_SORT and rhs.sort().kind() == z3.Z3_BV_SORT:
            lw = lhs.size()
            rw = rhs.size()

            if lw == rw:
                return lhs, rhs
            elif lw > rw:
                rhs = z3.ZeroExt(lw - rw, rhs)
                return lhs, rhs

            lhs = z3.ZeroExt(rw - lw, lhs)
            return lhs, rhs
        elif lhs.sort().kind() == z3.Z3_BOOL_SORT and rhs.sort().kind() == z3.Z3_BV_SORT:
            lhs = z3.If(lhs, z3.BitVecVal(1, rhs.size()), z3.BitVecVal(0, rhs.size()))
            return lhs, rhs
        elif lhs.sort().kind() == z3.Z3_BV_SORT and rhs.sort().kind() == z3.Z3_BOOL_SORT:
            rhs = z3.If(rhs, z3.BitVecVal(1, lhs.size()), z3.BitVecVal(0, lhs.size()))
            return lhs, rhs

        return lhs, rhs

    def coerce_to_sort(
        self,
        value: z3.ExprRef,
        target_sort: z3.SortRef,
        *,
        dst_subject: dict[str, Any] | None = None
    ) -> z3.ExprRef:
        if value.sort() == target_sort:
            return value
        elif target_sort.kind() == z3.Z3_BOOL_SORT:
            return self.as_bool(value)
        elif target_sort.kind() == z3.Z3_BV_SORT:
            width = target_sort.size()

            if value.sort().kind() == z3.Z3_BOOL_SORT:
                return z3.If(value, z3.BitVecVal(1, width), z3.BitVecVal(0, width))
            elif value.sort().kind() == z3.Z3_BV_SORT:
                current = value.size()
                if current == width:
                    return value
                elif current < width:
                    return z3.ZeroExt(width - current, value)

                return z3.Extract(width - 1, 0, value)
        elif target_sort.kind() == z3.Z3_REAL_SORT:
            if value.sort().kind() == z3.Z3_BV_SORT:
                return self.fresh_value("bv_to_real", target_sort)
            if value.sort().kind() == z3.Z3_BOOL_SORT:
                return z3.If(value, z3.RealVal(1), z3.RealVal(0))

        raise HIRZ3Error(f"cannot coerce {value} : {value.sort()} to {target_sort}")

    def as_bool(self, value: z3.ExprRef) -> z3.BoolRef:
        if value.sort().kind() == z3.Z3_BOOL_SORT:
            return value
        elif value.sort().kind() == z3.Z3_BV_SORT:
            return value != z3.BitVecVal(0, value.size())
        elif value.sort().kind() in {z3.Z3_INT_SORT, z3.Z3_REAL_SORT}:
            return value != 0

        pred_name = safe_symbol_name(f"is_true_{value.sort().name()}")
        key = (pred_name, (value.sort().sexpr(),), z3.BoolSort().sexpr())
        pred = self._functions.get(key)
        if pred is None:
            pred = z3.Function(pred_name, value.sort(), z3.BoolSort())
            self._functions[key] = pred

        return pred(value)

    def cast_expr(self, value: z3.ExprRef, *, target_ty: str, src_subject: dict[str, Any], dst_subject: dict[str, Any]) -> z3.ExprRef:
        if target_ty == "*ptr":
            target_sort = z3.BitVecSort(self.ptr_bits)
        else:
            fake_subject = dict(dst_subject)
            fake_subject["ty"] = target_ty
            target_sort = subject_sort(fake_subject, ptr_bits=self.ptr_bits)

        if value.sort() == target_sort:
            return value
        if target_sort.kind() == z3.Z3_BOOL_SORT:
            return self.as_bool(value)

        if target_sort.kind() == z3.Z3_BV_SORT:
            width = target_sort.size()
            if value.sort().kind() == z3.Z3_BOOL_SORT:
                return z3.If(value, z3.BitVecVal(1, width), z3.BitVecVal(0, width))

            if value.sort().kind() == z3.Z3_BV_SORT:
                current = value.size()

                if current == width:
                    return value
                elif current < width:
                    if is_unsigned_ty(src_subject.get("ty")):
                        return z3.ZeroExt(width - current, value)
                    return z3.SignExt(width - current, value)

                return z3.Extract(width - 1, 0, value)

            if value.sort().kind() == z3.Z3_REAL_SORT:
                return self.fresh_value("real_to_bv", target_sort)

        if target_sort.kind() == z3.Z3_REAL_SORT:
            if value.sort().kind() == z3.Z3_BOOL_SORT:
                return z3.If(value, z3.RealVal(1), z3.RealVal(0))
            elif value.sort().kind() == z3.Z3_BV_SORT:
                return self.fresh_value("bv_to_real", target_sort)

        raise HIRZ3Error(
            f"unsupported cast from {value.sort()} to {target_ty}"
        )

    def defer(
        self,
        instr: dict[str, Any],
        *,
        reason: str = "not lowered at this preparation stage",
    ) -> None:
        self.prepared.deferred.append({ "reason": reason, "instruction": instr })

def _parse_int_value(value: Any) -> int:
    if isinstance(value, int):
        return value

    text = str(value).strip()
    if text.startswith(("0x", "-0x", "+0x")):
        return int(text, 16)
    if text.startswith(("0b", "-0b", "+0b")):
        return int(text, 2)

    return int(text, 10)

def _parse_bool_value(value: Any) -> bool:
    if isinstance(value, bool):
        return value
    elif isinstance(value, int):
        return value != 0

    text = str(value).strip().lower()
    if text in { "true", "yes", "on" }:
        return True
    if text in { "false", "no", "off" }:
        return False

    return _parse_int_value(text) != 0

def prepare_hir_for_z3(program: dict[str, Any] | str, *, ptr_bits: int = DEFAULT_PTR_BITS, strict: bool = False) -> Z3BaseInfo:
    return Z3Builder(program, ptr_bits=ptr_bits, strict=strict).prepare()
