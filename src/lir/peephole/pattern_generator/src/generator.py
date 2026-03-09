import json
from collections import defaultdict
from ordered_set import OrderedSet
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
        
        with open(path, 'r') as f:
            self.gen_info: dict = json.load(f)
            
        self.PTRN_TO_LIR: dict[str, list[str]] = self.gen_info.get("mapping", {})
        self.LIR_TO_PTRN: dict[str, str] = {}
        for ptrn, lirs in self.PTRN_TO_LIR.items():
            for lir in lirs:
                self.LIR_TO_PTRN[lir] = ptrn
    
    def _setup_var_map_for_instruction(self, instr: Instruction, base_ptr: str = "lh") -> dict[str, str]:
        var_map = {}
        for i, operand in enumerate(instr.operands):
            if operand and operand.var_name and i < 3:
                var_map[operand.var_name] = f"{base_ptr}->{self.gen_info.get("arg_names")[i]}"
                    
        return var_map
    
    def _operand_to_condition(self, operand: Operand, arg_name: str) -> str | None:
        if operand.type == OperandType.REG:
            return f'{arg_name}->reg->reg == {operand.value.upper()}'
        elif operand.type == OperandType.AREG:
            return f"{arg_name}->t == LIR_REGISTER"
        elif operand.type == OperandType.CONST:
            try:
                if operand.value.startswith("0x"):
                    value = int(operand.value, 16)
                elif operand.value.startswith("0b"):
                    value = int(operand.value[2:], 2)
                else:
                    value = int(operand.value)
                return f"({arg_name}->t == LIR_NUMBER && {self.gen_info.get("functions").get("atoi")}({arg_name}) == {value})"
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
        conditions = []
        for i, operand in enumerate(instr.operands):
            if operand and i < 3:
                arg_name = f"{base_ptr}->{self.gen_info.get("arg_names")[i]}"
                cond: str | None = self._operand_to_condition(operand, arg_name)
                if cond:
                    conditions.append(f"{arg_name}")
                    conditions.append(cond)
        
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
                base_ptr = f"{self.gen_info.get("functions").get("next")}(lh, bb->lmap.exit, {i})"
                conditions.add(f"{base_ptr}")
            
            instr_cond = self._generate_instruction_condition(instr, base_ptr)
            if instr_cond != "1":
                conditions.add(f"({instr_cond})")
        
        all_var_maps: list[dict[str, str]] = []
        for i, instr in enumerate(pattern.match):
            if not instr.operands:
                continue
            
            base_ptr = "lh" if i == 0 else f"{self.gen_info.get("functions").get("next")}(lh, bb->lmap.exit, {i})"
            all_var_maps.append(self._setup_var_map_for_instruction(instr, base_ptr))
            
            for op in instr.operands:
                if op and op.conditions:
                    for conds in op.conditions:
                        base: str = self.gen_info.get("conditions").get(conds.get("value"))
                        conditions.add(self._apply_condact_template(base, *all_var_maps[-1].values()))
        
        var_to_ptr: dict = {}
        for i, var_map in enumerate(all_var_maps):
            for var_name, ptr in var_map.items():
                if var_name in var_to_ptr:
                    conditions.add(f"{self.gen_info.get("functions").get("equals")}({ptr}, {var_to_ptr[var_name]})")
                else:
                    var_to_ptr[var_name] = ptr
        
        return " &&\n".join(conditions) if conditions else "1"
    
    def _apply_actions(self, pattern: Pattern) -> None:
        action_lines: list = []
        for i, instr in enumerate(pattern.replace):
            if not instr.operands:
                continue
            
            base_ptr = "lh" if i == 0 else f"{self.gen_info.get("functions").get("next")}(lh, bb->lmap.exit, {i})"
            vars_map: dict[str, str] = self._setup_var_map_for_instruction(instr, base_ptr)
            for op in instr.operands:
                if op and op.actions:
                    for acts in op.actions:
                        base: str = self.gen_info.get("actions").get(acts.get("value"))
                        action_lines.append(self._apply_condact_template(base, *vars_map.values()))
    
        return action_lines
    
    def _generate_pattern_action(self, pattern: Pattern) -> list[str]:
        action_lines: list = []
        if not pattern.replace:
            return action_lines
        
        if pattern.replace[0].mnemonic == "delete":
            for i in range(len(pattern.match)):
                if i == 0:
                    action_lines.append("lh->unused = 1;")
                else:
                    action_lines.append(
                        f"{self.gen_info.get("functions").get("next")}(lh, bb->lmap.exit, {i})->unused = 1;"
                    )
            return action_lines
        
        replace_instr: Instruction = pattern.replace[0]
        replace_lir: str = self.PTRN_TO_LIR.get(replace_instr.mnemonic, [])
        if not replace_lir:
            raise ValueError(f"Mnemonic {replace_instr.mnemonic} not found!")
        
        action_lines.append(f"lh->op = {replace_lir[0]};")
        var_map = self._setup_var_map_for_instruction(pattern.match[0], "lh")
        
        for i, operand in enumerate(replace_instr.operands):
            if operand and i < 3:
                arg_ptr = f"lh->{self.gen_info.get("arg_names")[i]}"
                if operand.var_name and operand.var_name in var_map:
                    if arg_ptr != var_map[operand.var_name]:
                        action_lines.append(f"{self.gen_info.get("functions").get("free")}({arg_ptr});")
                        action_lines.append(f"{arg_ptr} = {var_map[operand.var_name]};")
                elif operand.type == OperandType.CONST and operand.value:
                    try:
                        value = int(operand.value)
                        action_lines.append(f"{self.gen_info.get("functions").get("free")}({arg_ptr});")
                        action_lines.append(f"{arg_ptr} = LIR_SUBJ_CONST({value});")
                    except ValueError:
                        pass
        
        for i in range(1, len(pattern.match)):
            action_lines.append(f"{self.gen_info.get("functions").get("next")}(lh, bb->lmap.exit, {i})->unused = 1;")
        
        action_lines.extend(self._apply_actions(pattern))
        return action_lines
    
    def _generate_single_pattern(self, pattern: Pattern) -> dict[str, list[str]]:
        result: dict[str, list] = {}
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
            if lir_opcode not in result:
                result[lir_opcode] = []
                
            result[lir_opcode].append(code)
        
        return result
    
    def generate(self) -> str:
        self.generated_code.clear()

        for pattern in self.patterns:
            pattern_code = self._generate_single_pattern(pattern)
            for opcode, codes in pattern_code.items():
                if opcode not in self.generated_code:
                    self.generated_code[opcode] = []
                self.generated_code[opcode].extend(codes)

        b = CodeBuilder()

        b.line("/* This is a generated code. Don't change it, use the main.py instead. */")
        b.line("#include <lir/peephole/peephole.h>")
        b.open("int peephole_first_pass(cfg_block_t* bb)")
        b.line("lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);")
        b.open("while (lh)")
        b.open("switch (lh->op)")

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
        b.line("return 1;")
        b.close()

        return b.render()