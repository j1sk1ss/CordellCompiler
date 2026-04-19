import json
from collections import defaultdict
try:
    from ordered_set import OrderedSet
except ImportError:
    class OrderedSet(dict):
        def add(self, item):
            self[item] = None
        def __iter__(self):
            return iter(self.keys())

from src.pattern import (
    Operand, OperandType, Instruction, Pattern
)

class CodeBuilder:
    def __init__(self, indent: str = "    "):
        self.lines: list[str] = []
        self.level = 0
        self.indent = indent

    def line(self, text: str = "") -> None:
        if text:
            self.lines.append(f"{self.indent * self.level}{text}")
        else:
            self.lines.append("")

    def extend_block(self, block: str) -> None:
        for line in block.splitlines():
            self.line(line)

    def open(self, header: str) -> None:
        self.line(f"{header} {{")
        self.level += 1

    def close(self, suffix: str = "") -> None:
        self.level -= 1
        if suffix:
            self.line(f"}} {suffix}")
        else:
            self.line("}")

    def render(self) -> str:
        return "\n".join(self.lines)

class CodeGenerator:
    def __init__(self, patterns: list[Pattern], path: str = "config.json") -> None:
        self.patterns = patterns
        self.generated_code: dict[str, list[str]] = {}
        self.var_map: dict[str, str] = {}
        self._temp_counter = 0

        with open(path, "r") as f:
            self.gen_info: dict = json.load(f)

        self.PTRN_TO_LIR: dict[str, list[str]] = self.gen_info.get("mapping", {})
        self.LIR_TO_PTRN: dict[str, str] = {}
        for ptrn, lirs in self.PTRN_TO_LIR.items():
            for lir in lirs:
                self.LIR_TO_PTRN[lir] = ptrn

    def _opcode_condition(self, instr: Instruction, base_ptr: str) -> str:
        lir_ops = self.PTRN_TO_LIR.get(instr.mnemonic, [])
        if not lir_ops:
            raise ValueError(f"Mnemonic {instr.mnemonic} not found!")

        if len(lir_ops) == 1:
            return f"{base_ptr}->op == {lir_ops[0]}"

        joined = " || ".join(f"{base_ptr}->op == {op}" for op in lir_ops)
        return f"({joined})"

    def _canonical_operand_ptr(self, instr: Instruction, base_ptr: str, operand_idx: int) -> str:
        for hidden_idx, explicit_idx in self._hidden_alias_pairs(instr):
            if explicit_idx == operand_idx:
                return self._arg_ptr(base_ptr, hidden_idx)
        return self._arg_ptr(base_ptr, operand_idx)

    def _new_temp(self, prefix: str = "tmp") -> str:
        self._temp_counter += 1
        return f"_{prefix}_{self._temp_counter}"

    def _used_args_count(self, instr: Instruction) -> int:
        return int(self.gen_info.get("used_args", {}).get(instr.mnemonic, 0) or 0)

    def _effective_arg_names(self) -> list[str]:
        names = list(self.gen_info.get("arg_names", []))
        fallback = ["farg", "sarg", "targ"]

        while len(names) < 3:
            names.append(fallback[len(names)])

        if len(names) >= 3 and names[1] == names[2] == "sarg":
            names[2] = "targ"

        return names

    def _arg_name(self, logical_idx: int) -> str:
        arg_names = self._effective_arg_names()
        if logical_idx >= len(arg_names):
            raise IndexError(f"Argument index {logical_idx} is out of bounds for arg_names")
        return arg_names[logical_idx]

    def _arg_ptr(self, base_ptr: str, logical_idx: int) -> str:
        return f"{base_ptr}->{self._arg_name(logical_idx)}"

    def _arg_ptrs(self, base_ptr: str) -> list[str]:
        return [self._arg_ptr(base_ptr, i) for i in range(3)]

    def _instr_ptr(self, instr_idx: int) -> str:
        if instr_idx == 0:
            return "lh"
        return f"{self.gen_info.get('functions').get('next')}(lh, bb->lmap.exit, {instr_idx})"

    def _first_explicit_operand_index(self, instr: Instruction) -> int:
        return self._used_args_count(instr)

    def _hidden_alias_pairs(self, instr: Instruction) -> list[tuple[int, int]]:
        used_args = self._used_args_count(instr)
        pairs: list[tuple[int, int]] = []
        for hidden_idx in range(used_args):
            explicit_idx = used_args + hidden_idx
            if explicit_idx < len(instr.operands) and instr.operands[explicit_idx] is not None:
                pairs.append((hidden_idx, explicit_idx))
        return pairs

    def _setup_var_map_for_instruction(self, instr: Instruction, base_ptr: str = "lh") -> dict[str, str]:
        var_map: dict[str, str] = {}
        first_explicit = self._first_explicit_operand_index(instr)

        for i, operand in enumerate(instr.operands):
            if i < first_explicit:
                continue
            if operand and operand.var_name and i < 3 and operand.var_name not in var_map:
                var_map[operand.var_name] = self._canonical_operand_ptr(instr, base_ptr, i)

        return var_map

    def _operand_to_condition(self, operand: Operand, arg_name: str) -> str | None:
        if operand.type == OperandType.REG:
            if operand.var_name is not None and operand.value is None:
                return f"{arg_name}->t == LIR_REGISTER"
            return f'{arg_name}->reg->reg == {operand.value.upper()}'
        elif operand.type == OperandType.AREG:
            return f"{arg_name}->t == LIR_REGISTER"
        elif operand.type == OperandType.CONST:
            if operand.var_name is not None and operand.value is None:
                return f"({arg_name}->t == LIR_NUMBER || {arg_name}->t == LIR_CONSTVAL)"
            try:
                if operand.value.startswith("0x"):
                    value = int(operand.value, 16)
                elif operand.value.startswith("0b"):
                    value = int(operand.value[2:], 2)
                else:
                    value = int(operand.value)
                return (
                    f"(({arg_name}->t == LIR_NUMBER || {arg_name}->t == LIR_CONSTVAL) && "
                    f"{self.gen_info.get('functions').get('atoi')}({arg_name}) == {value})"
                )
            except ValueError:
                raise ValueError("There is no constant value for 'const' keyword!")
        elif operand.type == OperandType.ACONST:
            return f"({arg_name}->t == LIR_NUMBER || {arg_name}->t == LIR_CONSTVAL)"
        elif operand.type == OperandType.MEM:
            return f"{arg_name}->t == LIR_MEMORY"
        elif operand.type == OperandType.OBJ:
            return None
        else:
            raise KeyError("Unknown operation type!")

    def _generate_instruction_condition(self, instr: Instruction, base_ptr: str = "lh") -> str:
        conditions: list[str] = []

        for i, operand in enumerate(instr.operands):
            if operand and i < 3:
                arg_name = self._arg_ptr(base_ptr, i)
                cond: str | None = self._operand_to_condition(operand, arg_name)
                if cond:
                    conditions.append(f"{arg_name}")
                    conditions.append(cond)

        equals_fn = self.gen_info.get("functions", {}).get("equals")
        for hidden_idx, explicit_idx in self._hidden_alias_pairs(instr):
            explicit_operand = instr.operands[explicit_idx]
            hidden_ptr = self._arg_ptr(base_ptr, hidden_idx)
            explicit_ptr = self._arg_ptr(base_ptr, explicit_idx)

            hidden_cond = self._operand_to_condition(explicit_operand, hidden_ptr)
            if hidden_cond:
                conditions.append(f"{hidden_ptr}")
                conditions.append(hidden_cond)

            if equals_fn:
                conditions.append(f"{equals_fn}({hidden_ptr}, {explicit_ptr})")

        return " &&\n".join(conditions) if conditions else "1"

    def _apply_condact_template(self, template: str, *args: str) -> str:
        result = template
        for i, arg in enumerate(args, start=1):
            result = result.replace(f"%{i}", arg)
        return result

    def _generate_pattern_condition(self, pattern: Pattern) -> str:
        conditions: OrderedSet = OrderedSet()
        if not pattern.match:
            return ""

        for i, instr in enumerate(pattern.match):
            if i == 0:
                base_ptr = "lh"
            else:
                base_ptr = f"{self.gen_info.get('functions').get('next')}(lh, bb->lmap.exit, {i})"
                conditions.add(base_ptr)

            conditions.add(self._opcode_condition(instr, base_ptr))

            instr_cond = self._generate_instruction_condition(instr, base_ptr)
            if instr_cond != "1":
                conditions.add(f"({instr_cond})")

        all_var_maps: list[dict[str, str]] = []
        for i, instr in enumerate(pattern.match):
            if not instr.operands:
                continue

            base_ptr = "lh" if i == 0 else f"{self.gen_info.get('functions').get('next')}(lh, bb->lmap.exit, {i})"
            current_var_map = self._setup_var_map_for_instruction(instr, base_ptr)
            all_var_maps.append(current_var_map)

            for op in instr.operands:
                if op and op.conditions:
                    for conds in op.conditions:
                        base: str = self.gen_info.get("conditions").get(conds.get("value"))
                        if base:
                            conditions.add(self._apply_condact_template(base, *current_var_map.values()))

        var_to_ptr: dict[str, str] = {}
        equals_fn = self.gen_info.get("functions", {}).get("equals")
        for var_map in all_var_maps:
            for var_name, ptr in var_map.items():
                if var_name in var_to_ptr:
                    conditions.add(f"{equals_fn}({ptr}, {var_to_ptr[var_name]})")
                else:
                    var_to_ptr[var_name] = ptr

        return " &&\n".join(conditions) if conditions else "1"

    def _release_if_unaliased_lines(
        self,
        ptr_expr: str,
        base_ptr: str,
        free_fn: str,
        extra_live_ptrs: list[str] | None = None,
    ) -> list[str]:
        guards = [f"{ptr_expr}"]
        for live_ptr in self._arg_ptrs(base_ptr):
            guards.append(f"{ptr_expr} != {live_ptr}")
        for live_ptr in extra_live_ptrs or []:
            guards.append(f"{ptr_expr} != {live_ptr}")
        return [f"if ({' && '.join(guards)}) {{", f"    {free_fn}({ptr_expr});", "}"]

    def _capture_match_sources(
        self,
        action_lines: list[str],
        match_var_map: dict[str, str],
    ) -> dict[str, str]:
        source_map: dict[str, str] = {}
        for var_name, ptr_expr in match_var_map.items():
            temp_name = self._new_temp(f"src_{var_name}")
            action_lines.append(f"lir_subject_t* {temp_name} = {ptr_expr};")
            source_map[var_name] = temp_name
        return source_map

    def _capture_match_opcodes(self, action_lines: list[str], pattern: Pattern) -> dict[int, str]:
        opcode_map: dict[int, str] = {}
        for i, _ in enumerate(pattern.match):
            temp_name = self._new_temp(f"match_op_{i}")
            action_lines.append(f"lir_operation_t {temp_name} = {self._instr_ptr(i)}->op;")
            opcode_map[i] = temp_name
        return opcode_map

    def _capture_match_live_subjects(self, action_lines: list[str], pattern: Pattern) -> list[str]:
        keep: list[str] = []
        for i, _ in enumerate(pattern.match):
            base_ptr = self._instr_ptr(i)
            for arg_idx in range(3):
                temp_name = self._new_temp(f"keep_{i}_{arg_idx}")
                action_lines.append(f"lir_subject_t* {temp_name} = {self._arg_ptr(base_ptr, arg_idx)};")
                keep.append(temp_name)
        return keep

    def _live_arg_indices(self, instr: Instruction) -> set[int]:
        live: set[int] = set()
        alias_pairs = self._hidden_alias_pairs(instr)

        for hidden_idx, explicit_idx in alias_pairs:
            if hidden_idx < 3:
                live.add(hidden_idx)
            if explicit_idx < 3:
                live.add(explicit_idx)

        first_explicit = self._first_explicit_operand_index(instr)
        for i, operand in enumerate(instr.operands):
            if not operand or i >= 3:
                continue
            if i < first_explicit:
                continue
            live.add(i)

        return live

    def _clear_dead_args(
        self,
        action_lines: list[str],
        instr: Instruction,
        base_ptr: str,
        free_fn: str,
        protected_ptrs: list[str],
    ) -> None:
        live = self._live_arg_indices(instr)
        for i in range(3):
            if i in live:
                continue

            arg_ptr = self._arg_ptr(base_ptr, i)
            old_tmp = self._new_temp("old")
            action_lines.append(f"lir_subject_t* {old_tmp} = {arg_ptr};")
            action_lines.append(f"if ({old_tmp}) {{")
            action_lines.append(f"    {arg_ptr} = NULL;")
            action_lines.append("    optimized = 1;")
            for line in self._release_if_unaliased_lines(old_tmp, base_ptr, free_fn, protected_ptrs):
                action_lines.append(f"    {line}" if line != "}" else "    }")
            action_lines.append("}")

    def _build_full_match_var_map(self, pattern: Pattern) -> dict[str, str]:
        merged: dict[str, str] = {}
        for i, instr in enumerate(pattern.match):
            if not instr.operands:
                continue
            base_ptr = "lh" if i == 0 else f"{self.gen_info.get('functions').get('next')}(lh, bb->lmap.exit, {i})"
            current = self._setup_var_map_for_instruction(instr, base_ptr)
            for var_name, ptr_expr in current.items():
                if var_name not in merged:
                    merged[var_name] = ptr_expr
        return merged

    def _replacement_opcode_expr(
        self,
        pattern: Pattern,
        replace_instr: Instruction,
        replace_idx: int = 0,
        match_opcode_map: dict[int, str] | None = None,
    ) -> str:
        replace_lir: list[str] = self.PTRN_TO_LIR.get(replace_instr.mnemonic, [])
        if not replace_lir:
            raise ValueError(f"Mnemonic {replace_instr.mnemonic} not found!")

        if len(replace_lir) == 1:
            return replace_lir[0]

        if match_opcode_map is None:
            match_opcode_map = {}

        if replace_idx < len(pattern.match) and pattern.match[replace_idx].mnemonic == replace_instr.mnemonic:
            return match_opcode_map.get(replace_idx, f"{self._instr_ptr(replace_idx)}->op")

        for i, instr in enumerate(pattern.match):
            if instr.mnemonic == replace_instr.mnemonic:
                return match_opcode_map.get(i, f"{self._instr_ptr(i)}->op")

        return replace_lir[0]

    def _apply_actions(self, pattern: Pattern, protected_ptrs: list[str]) -> list[str]:
        action_lines: list[str] = []
        free_fn = self.gen_info.get("functions", {}).get("free")
        sqrt_fn = self.gen_info.get("functions", {}).get("sqrt")
        log2_fn = self.gen_info.get("functions", {}).get("log2")

        for i, instr in enumerate(pattern.replace):
            if not instr.operands:
                continue

            base_ptr = "lh" if i == 0 else f"{self.gen_info.get('functions').get('next')}(lh, bb->lmap.exit, {i})"
            vars_map: dict[str, str] = self._setup_var_map_for_instruction(instr, base_ptr)
            for op_idx, op in enumerate(instr.operands):
                if not op or not op.actions or op_idx >= 3:
                    continue

                dest_ptr = self._arg_ptr(base_ptr, op_idx)
                for acts in op.actions:
                    action_name = acts.get("value")

                    if action_name == "log2" and log2_fn:
                        old_tmp = self._new_temp("old")
                        action_lines.append(f"lir_subject_t* {old_tmp} = {dest_ptr};")
                        action_lines.append(f"{dest_ptr} = LIR_SUBJ_CONST({log2_fn}({old_tmp}));")
                        action_lines.extend(self._release_if_unaliased_lines(old_tmp, base_ptr, free_fn, protected_ptrs))
                        continue

                    if action_name == "sqrt2" and sqrt_fn:
                        old_tmp = self._new_temp("old")
                        action_lines.append(f"lir_subject_t* {old_tmp} = {dest_ptr};")
                        action_lines.append(f"{dest_ptr} = LIR_SUBJ_CONST({sqrt_fn}({old_tmp}));")
                        action_lines.extend(self._release_if_unaliased_lines(old_tmp, base_ptr, free_fn, protected_ptrs))
                        continue

                    base: str = self.gen_info.get("actions").get(action_name)
                    if base:
                        action_lines.append(self._apply_condact_template(base, *vars_map.values()))

        return action_lines

    def _const_value_to_c_expr(self, operand: Operand) -> str | None:
        if operand.type != OperandType.CONST or not operand.value:
            return None

        try:
            if operand.value.startswith("0x"):
                value = int(operand.value, 16)
            elif operand.value.startswith("0b"):
                value = int(operand.value[2:], 2)
            else:
                value = int(operand.value)
            return f"LIR_SUBJ_CONST({value})"
        except ValueError:
            return None

    def _assign_operand_value(
        self,
        action_lines: list[str],
        dest_ptr: str,
        operand: Operand,
        source_var_map: dict[str, str],
        base_ptr: str,
        free_fn: str,
        assigned_ptrs: set[str],
        protected_ptrs: list[str],
    ) -> None:
        if dest_ptr in assigned_ptrs:
            return

        if operand.var_name and operand.var_name in source_var_map:
            src_ptr = source_var_map[operand.var_name]
            old_tmp = self._new_temp("old")
            action_lines.append(f"lir_subject_t* {old_tmp} = {dest_ptr};")
            action_lines.append(f"if ({old_tmp} != {src_ptr}) {{")
            action_lines.append(f"    {dest_ptr} = {src_ptr};")
            action_lines.append("    optimized = 1;")
            for line in self._release_if_unaliased_lines(old_tmp, base_ptr, free_fn, protected_ptrs):
                action_lines.append(f"    {line}" if line != "}" else "    }")
            action_lines.append("}")
            assigned_ptrs.add(dest_ptr)
            return

        const_expr = self._const_value_to_c_expr(operand)
        if const_expr:
            old_tmp = self._new_temp("old")
            action_lines.append(f"lir_subject_t* {old_tmp} = {dest_ptr};")
            action_lines.append(f"{dest_ptr} = {const_expr};")
            action_lines.append("optimized = 1;")
            action_lines.extend(self._release_if_unaliased_lines(old_tmp, base_ptr, free_fn, protected_ptrs))
            assigned_ptrs.add(dest_ptr)

    def _generate_pattern_action(self, pattern: Pattern) -> list[str]:
        action_lines: list[str] = []
        if not pattern.replace:
            return action_lines

        if len(pattern.replace) == 1 and pattern.replace[0].mnemonic == "delete":
            for i in range(len(pattern.match)):
                ptr = self._instr_ptr(i)
                action_lines.append(f"if (!{ptr}->unused) {{")
                action_lines.append(f"    {ptr}->unused = 1;")
                action_lines.append("    optimized = 1;")
                action_lines.append("}")
            return action_lines

        match_var_map = self._build_full_match_var_map(pattern)
        source_var_map = self._capture_match_sources(action_lines, match_var_map)
        match_opcode_map = self._capture_match_opcodes(action_lines, pattern)
        protected_ptrs = list(source_var_map.values())
        protected_ptrs.extend(self._capture_match_live_subjects(action_lines, pattern))
        free_fn = self.gen_info.get("functions", {}).get("free")

        for rep_idx, replace_instr in enumerate(pattern.replace):
            base_ptr = self._instr_ptr(rep_idx)
            opcode_expr = self._replacement_opcode_expr(pattern, replace_instr, rep_idx, match_opcode_map)

            action_lines.append(f"if ({base_ptr}->op != {opcode_expr}) {{")
            action_lines.append(f"    {base_ptr}->op = {opcode_expr};")
            action_lines.append("    optimized = 1;")
            action_lines.append("}")

            assigned_ptrs: set[str] = set()
            for i, operand in enumerate(replace_instr.operands):
                if not operand or i >= 3:
                    continue
                if i < self._first_explicit_operand_index(replace_instr):
                    continue

                dest_ptr = self._arg_ptr(base_ptr, i)
                self._assign_operand_value(
                    action_lines,
                    dest_ptr,
                    operand,
                    source_var_map,
                    base_ptr,
                    free_fn,
                    assigned_ptrs,
                    protected_ptrs,
                )

            for hidden_idx, explicit_idx in self._hidden_alias_pairs(replace_instr):
                explicit_operand = replace_instr.operands[explicit_idx]
                if explicit_operand is None:
                    continue
                hidden_ptr = self._arg_ptr(base_ptr, hidden_idx)
                self._assign_operand_value(
                    action_lines,
                    hidden_ptr,
                    explicit_operand,
                    source_var_map,
                    base_ptr,
                    free_fn,
                    assigned_ptrs,
                    protected_ptrs,
                )

            self._clear_dead_args(
                action_lines,
                replace_instr,
                base_ptr,
                free_fn,
                protected_ptrs,
            )

        for i in range(len(pattern.replace), len(pattern.match)):
            ptr = self._instr_ptr(i)
            action_lines.append(f"if (!{ptr}->unused) {{")
            action_lines.append(f"    {ptr}->unused = 1;")
            action_lines.append("    optimized = 1;")
            action_lines.append("}")

        extra_actions = self._apply_actions(pattern, protected_ptrs)
        if extra_actions:
            action_lines.append("optimized = 1;")
            action_lines.extend(extra_actions)

        return action_lines

    def _generate_single_pattern(self, pattern: Pattern) -> dict[str, list[str]]:
        result: dict[str, list[str]] = {}
        if not pattern.match:
            return result

        first_instr = pattern.match[0]
        lir_opcodes = self.PTRN_TO_LIR.get(first_instr.mnemonic, [])
        if not lir_opcodes:
            print(f"Warning: unknown mnemonic {first_instr.mnemonic}")
            return result

        condition: str = self._generate_pattern_condition(pattern)
        action: list[str] = self._generate_pattern_action(pattern)
        if not action:
            return result

        if condition == "1":
            code = "{\n"
        else:
            code = f"if ({condition}) {{\n"

        for line in action:
            code += f"    {line}\n"

        code += "}"
        for lir_opcode in lir_opcodes:
            result.setdefault(lir_opcode, []).append(code)

        return result

    def generate(self) -> str:
        self.generated_code.clear()
        self._temp_counter = 0

        for pattern in self.patterns:
            pattern_code = self._generate_single_pattern(pattern)
            for opcode, codes in pattern_code.items():
                self.generated_code.setdefault(opcode, []).extend(codes)

        b = CodeBuilder()

        b.line("/* This is a generated code. Don't change it, use the main.py instead. */")
        b.line("#include <lir/peephole/peephole.h>")
        b.open("int peephole_first_pass(cfg_block_t* bb)")
        b.line("int optimized = 0;")
        b.line("lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);")
        b.open("while (lh)")
        b.open("if (!lh->unused) switch (lh->op)")

        grouped_cases: dict[tuple[str, ...], list[str]] = defaultdict(list)
        for opcode, codes in self.generated_code.items():
            normalized_codes = tuple(code.rstrip() for code in codes)
            grouped_cases[normalized_codes].append(opcode)

        for codes_tuple, opcodes in sorted(grouped_cases.items(), key=lambda item: sorted(item[1])[0]):
            for opcode in sorted(opcodes):
                b.line(f"case {opcode}:")

            b.open("")
            codes = list(codes_tuple)

            if len(codes) == 1:
                b.extend_block(codes[0])
            else:
                for i, code in enumerate(codes):
                    lines = code.splitlines()
                    if not lines:
                        continue

                    if i == 0:
                        for line in lines:
                            b.line(line)
                    else:
                        b.line(f"else {lines[0]}")
                        for line in lines[1:]:
                            b.line(line)

            b.line("break;")
            b.close()
            b.line()

        b.line("default: break;")
        b.close()
        b.line("lh = LIR_get_next(lh, bb->lmap.exit, 1);")
        b.close()
        b.line("return optimized;")
        b.close()

        return b.render()
