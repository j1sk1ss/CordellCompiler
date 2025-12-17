import re
import json

from src.pattern import (
    Operand, OperandType, Instruction, Pattern
)

class PTRNParser:
    def __init__(self, config: str = "commands.json") -> None:
        with open(config) as f:
            self.parse_info: dict = json.load(f)
    
    def _extract_brackets(self, line: str) -> tuple[str, list[dict], str]:
        line: str = line.split(';')[0].strip()
        match: re.Match[str] | None = re.search(r'\[(if|do):([^\]]+)\]\s*$', line)
        if not match:
            return line, [], ""
        
        bracket_type = match.group(1)
        content = match.group(2).strip()
        line = line[:match.start()].rstrip()
        
        items = []
        
        for item in content.split(","):
            item = item.strip()
            if ":" in item:
                target, value = item.split(":", 1)
                items.append({
                    'type': bracket_type,
                    'target': target.strip(),
                    'value': value.strip()
                })
            else:
                items.append({
                    'type': bracket_type,
                    'target': None,
                    'value': item
                })
        
        return line, items, bracket_type
    
    def _parse_operand(self, text: str) -> Operand:
        text = text.strip()
        var_match = re.match(r'^(\w+)_(\d+)$', text)
        if var_match:
            type_part, var_name = var_match.groups()
            try:
                op_type = OperandType(type_part)
                return Operand(type=op_type, var_name=var_name)
            except ValueError:
                pass
        
        parts = text.split()
        if len(parts) == 2:
            type_part, value = parts
            try:
                op_type = OperandType(type_part)
                return Operand(type=op_type, value=value)
            except ValueError:
                pass
        elif len(parts) == 1:
            try:
                op_type = OperandType(text)
                return Operand(type=op_type)
            except ValueError:
                pass
        
        const_match = re.match(r'^const\s+(.+)$', text)
        if const_match:
            return Operand(type=OperandType.CONST, value=const_match.group(1))
        
        raise ValueError(f"Unknown operand: {text}")
    
    def _parse_instruction_with_conditions(self, line: str, side: str) -> tuple[Instruction, list[dict]]:
        cleaned_line, items, bracket_type = self._extract_brackets(line)
        if bracket_type and (
            (side == 'lhs' and bracket_type == 'do') or 
            (side == 'rhs' and bracket_type == 'if')
        ):
            raise ValueError(f"{bracket_type} not allowed in {side}")
        
        instr = self.parse_instruction(cleaned_line)
        annotated_items = []
        
        for item in items:
            if item['target']:
                if item['target'].startswith('arg'):
                    try:
                        arg_num = int(item['target'][3:]) - 1
                        if 0 <= arg_num < len(instr.operands):
                            annotated_items.append({
                                'type': item['type'],
                                'operand_idx': arg_num,
                                'value': item['value']
                            })
                        else:
                            raise ValueError(f"Invalid operand index: {item['target']}")
                    except ValueError:
                        raise ValueError(f"Invalid target format: {item['target']}")
                else:
                    found = False
                    for idx, operand in enumerate(instr.operands):
                        if operand.var_name == item['target']:
                            annotated_items.append({
                                'type': item['type'],
                                'operand_idx': idx,
                                'value': item['value']
                            })
                            found = True
                            break
                    if not found:
                        raise ValueError(f"Unknown target: {item['target']}")
            else:
                for idx in range(len(instr.operands)):
                    annotated_items.append({
                        'type': item['type'],
                        'operand_idx': idx,
                        'value': item['value']
                    })
        
        return instr, annotated_items
    
    def _parse_instruction(self, line: str) -> Instruction:
        line = line.split(";")[0].strip()
        if not line:
            return None
        
        parts = line.split(None, 1)
        if len(parts) != 2:
            raise ValueError(f"Invalid instruction: {line}, Parts: {parts}")
        
        mnemonic, ops_part = parts
        mnemonic = mnemonic.lower()
        
        operands = []
        for _ in range(self.parse_info.get("used_args").get(mnemonic)):
            operands.append(None)
        
        for op_text in ops_part.split(","):
            operands.append(self._parse_operand(op_text.strip()))
        
        return Instruction(mnemonic=mnemonic, operands=operands)
    
    def _safe_split(self, source: str, dil: str = '/') -> list[str]:
        buf: list = []
        parts: list = []

        for line in source.splitlines(keepends=True):
            stripped = line.lstrip()
            if stripped.startswith(";"):
                continue

            segments = line.split(dil)
            buf.append(segments[0])

            for seg in segments[1:]:
                parts.append("".join(buf))
                buf = [seg]

        tail = "".join(buf).strip()
        if tail:
            parts.append(tail)

        return parts

    def _parse_brackets(self, items: list[dict], instruction: Instruction, is_do: bool) -> None:
        for item in items:
            if item.get("target", None):
                found: bool = False
                for idx, operand in enumerate(instruction.operands):
                    if not operand:
                        continue
                    
                    if operand.var_name == item['target'] or f"arg{idx + 1}" == item['target']:
                        if is_do:
                            if not operand.actions:
                                operand.actions = []
                            operand.actions.append({ 'value': item.get("value") })
                        else:
                            if not operand.conditions:
                                operand.conditions = []
                            operand.conditions.append({ 'value': item.get("value") })
                        
                        found = True
                        break
                    
                if not found:
                    raise ValueError(f"Unknown target in condition: {item['target']}")
            else:
                for idx in range(len(instruction.operands)):
                    operand = instruction.operands[idx]
                    if not operand:
                        continue
                    
                    if is_do:
                        if not operand.actions:
                            operand.actions = []
                        operand.actions.append({ 'value': item.get("value") })
                    else:
                        if not operand.conditions:
                            operand.conditions = []
                        operand.conditions.append({ 'value': item.get("value") })

    def parse_patterns(self, source: str) -> list[Pattern]:
        """Main parse patter pipeline

        Args:
            source (str): Source PTRN code from a file

        Raises:
            ValueError: If some patterns don't have a '->' symbol
            ValueError: If some conditions linked to unknown variables
            ValueError: If there is an empty pattern
            ValueError: If some actions linked to unknown variables

        Returns:
            list[Pattern]: List of patterns
        """
        
        patterns: list = []
        pattern_texts = self._safe_split(source.strip(), dil='/')

        for pattern_text in pattern_texts:
            pattern_text = pattern_text.strip()
            if not pattern_text:
                continue
            
            if "->" not in pattern_text:
                raise ValueError(f"Pattern without '->': {pattern_text[:50]}...")
            
            lhs_text, rhs_text = pattern_text.split("->", 1)
            match_instrs: list = []
            
            lhs_lines = lhs_text.strip().split("\n")
            for i, line in enumerate(lhs_lines):
                line = line.strip()
                cleaned_line, cond_items, bracket_type = self._extract_brackets(line)
                instr: Instruction = self._parse_instruction(cleaned_line)
                if instr:
                    match_instrs.append(instr)
                
                if instr and bracket_type == "if":
                    self._parse_brackets(cond_items, instr, False)                        
                    
            if not match_instrs:
                raise ValueError("Empty pattern")

            replace_instrs: list[Instruction] = []
            actions: list[dict] = []
            
            rhs_lines = rhs_text.strip().split("\n")
            for i, line in enumerate(rhs_lines):
                line = line.strip()
                if i == len(rhs_lines) - 1:
                    cleaned_line, action_items, bracket_type = self._extract_brackets(line)
                    if line == 'delete':
                        replace_instrs.append(Instruction(mnemonic='delete', operands=[]))
                        continue
                    else:
                        instr: Instruction = self._parse_instruction(cleaned_line)
                        
                    if instr and bracket_type == "do":
                        self._parse_brackets(action_items, instr, True)
                        
                    if instr:
                        replace_instrs.append(instr)
                        
            patterns.append(Pattern(
                match=match_instrs,
                replace=replace_instrs
            ))
        
        return patterns