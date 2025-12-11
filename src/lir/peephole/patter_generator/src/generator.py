import json

from typing import List, Dict
from src.pattern import (
    Operand, OperandType, Instruction, Pattern
)

class CodeGenerator:    
    def __init__(self, patterns: list[Pattern], path: str = "commands.json"):
        self.patterns = patterns
        self.generated_code: Dict[str, List[str]] = {}
        self.var_map: Dict[str, str] = {}
        
        with open(path, 'r') as f:
            self.gen_info: dict = json.load(f)
            
        self.PTRN_TO_LIR: dict[str, list[str]] = self.gen_info.get("mapping")
        self.LIR_TO_PTRN: dict[str, str] = {}
        for ptrn, lirs in self.PTRN_TO_LIR.items():
            for lir in lirs:
                self.LIR_TO_PTRN[lir] = ptrn
    
    def _setup_var_map_for_instruction(self, instr: Instruction, base_ptr: str = "lh") -> Dict[str, str]:
        var_map = {}
        for i, operand in enumerate(instr.operands):
            if operand.var_name:
                if i == 0:
                    var_map[operand.var_name] = f"{base_ptr}->farg"
                elif i == 1:
                    var_map[operand.var_name] = f"{base_ptr}->sarg"
                elif i == 2:
                    var_map[operand.var_name] = f"{base_ptr}->targ"
                    
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
        elif operand.type == OperandType.OBJ:
            return None
        elif operand.type == OperandType.MEM:
            return f"{arg_name}->t == LIR_MEMORY"
        else:
            raise KeyError("Unknown operation type!")
    
    def _generate_instruction_condition(self, instr: Instruction, base_ptr: str = "lh") -> str:
        conditions = []
        for i, operand in enumerate(instr.operands):
            if i == 0:
                arg_name = f"{base_ptr}->farg"
            elif i == 1:
                arg_name = f"{base_ptr}->sarg"
            elif i == 2:
                arg_name = f"{base_ptr}->targ"
            else:
                break
            
            cond: str | None = self._operand_to_condition(operand, arg_name)
            if cond:
                conditions.append(f"{arg_name}")
                conditions.append(cond)
        
        return " && ".join(conditions) if conditions else "1"
    
    def _apply_condact_template(self, template: str, *args: str) -> str:
        result = template
        for arg in args:
            result = result.replace("%", arg)
        return result
    
    def _generate_pattern_condition(self, pattern: Pattern) -> str:
        conditions: list = []
        if not pattern.match:
            return ""
        
        for i, instr in enumerate(pattern.match):
            if i == 0:
                base_ptr = "lh"
            else:
                base_ptr = f"lh->{'->next->' * (i-1)}next"
                conditions.append(f"{base_ptr}")
            
            instr_cond = self._generate_instruction_condition(instr, base_ptr)
            if instr_cond != "1":
                conditions.append(f"({instr_cond})")
        
        all_var_maps: list[dict[str, str]] = []
        for i, instr in enumerate(pattern.match):
            base_ptr = "lh" if i == 0 else f"lh->{'->next->' * (i - 1)}next"
            all_var_maps.append(self._setup_var_map_for_instruction(instr, base_ptr))
            for op in instr.operands:
                if op.conditions:
                    for conds in op.conditions:
                        base: str = self.gen_info.get("conditions").get(conds.get("value"))
                        conditions.append(self._apply_condact_template(base, all_var_maps[-1].get(op.var_name)))
        
        var_to_ptr: dict = {}
        for i, var_map in enumerate(all_var_maps):
            for var_name, ptr in var_map.items():
                if var_name in var_to_ptr:
                    conditions.append(f"{self.gen_info.get("functions").get("equals")}({ptr}, {var_to_ptr[var_name]})")
                else:
                    var_to_ptr[var_name] = ptr
        
        return " && ".join(conditions) if conditions else "1"
    
    def _apply_actions(self, pattern: Pattern) -> None:
        action_lines: list = []
        for i, instr in enumerate(pattern.replace):
            base_ptr = "lh" if i == 0 else f"lh->{'->next->' * (i - 1)}next"
            vars_map: dict[str, str] = self._setup_var_map_for_instruction(instr, base_ptr)
            for op in instr.operands:
                if op.actions:
                    for acts in op.actions:
                        base: str = self.gen_info.get("actions").get(acts.get("value"))
                        action_lines.append(self._apply_condact_template(base, vars_map.get(op.var_name)))
    
        return action_lines
    
    def _generate_pattern_action(self, pattern: Pattern) -> list[str]:
        action_lines = []
        if not pattern.replace:
            return action_lines
        
        if pattern.replace[0].mnemonic == "delete":
            for i in range(len(pattern.match)):
                if i == 0:
                    action_lines.append("lh->unused = 1;")
                else:
                    action_lines.append(f"lh->{'->next->' * (i - 1)}next->unused = 1;")
            return action_lines
        
        replace_instr: Instruction = pattern.replace[0]
        replace_lir: str = self.PTRN_TO_LIR.get(replace_instr.mnemonic, [])
        if not replace_lir:
            raise ValueError(f"Mnemonic {replace_instr.mnemonic} not found!")
        
        action_lines.append(f"lh->op = {replace_lir[0]};")
        var_map = self._setup_var_map_for_instruction(pattern.match[0], "lh")
        
        for i, operand in enumerate(replace_instr.operands):
            if i == 0:
                arg_ptr = "lh->farg"
            elif i == 1:
                arg_ptr = "lh->sarg"
            elif i == 2:
                arg_ptr = "lh->targ"
            else:
                break
            
            if operand.var_name and operand.var_name in var_map:
                action_lines.append(f"{arg_ptr} = {var_map[operand.var_name]};")
            elif operand.type == OperandType.CONST and operand.value:
                try:
                    value = int(operand.value)
                    action_lines.append(f"{arg_ptr} = LIR_SUBJ_CONST({value});")
                except ValueError:
                    pass
        
        for i in range(1, len(pattern.match)):
            action_lines.append(f"lh->{'->next->' * (i - 1)}next->unused = 1;")
        
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
        
        condition = self._generate_pattern_condition(pattern)
        action = self._generate_pattern_action(pattern)
        
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
        for pattern in self.patterns:
            pattern_code = self._generate_single_pattern(pattern)
            for opcode, codes in pattern_code.items():
                if opcode not in self.generated_code:
                    self.generated_code[opcode] = []
                self.generated_code[opcode].extend(codes)

        result: str = """
/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
static int _first_pass(cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        switch (lh->op) {
"""

        sorted_opcodes = sorted(self.generated_code.keys())
        for opcode in sorted_opcodes:
            result += f"            case {opcode}: {{\n"
            codes: list = self.generated_code[opcode]
            if len(codes) == 1:
                code: str = codes[0]
                lines = code.split("\n")
                for line in lines:
                    result += f"                {line}\n"
            else:
                for i, code in enumerate(codes):
                    is_else: bool = False
                    if i > 0:
                        result += "                else "
                        is_else = True
                    
                    lines = code.split("\n")
                    for line in lines:
                        if is_else:
                            result += f"{line}\n"
                            is_else = False
                        else:
                            result += f"                {line}\n"
            
            result += "                break;\n"
            result += "            }\n\n"
        
        result += """            default: break;
        }

        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    return 1;
}"""
        
        return result