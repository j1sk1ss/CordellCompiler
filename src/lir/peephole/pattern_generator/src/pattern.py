from enum import Enum
from typing import Optional
from dataclasses import dataclass

class OperandType(Enum):
    CONST = "const"
    ACONST = "aconst" 
    REG = "reg"
    AREG = "areg"
    MEM = "mem"
    OBJ = "obj"

@dataclass
class Operand:
    type: OperandType
    value: Optional[str] = None
    var_name: Optional[str] = None
    conditions: list[str] | None = None
    actions: list[str] | None = None
    
    def __str__(self):
        result = []
        if self.var_name:
            result.append(f"{self.type.value}_{self.var_name}")
        else:
            result.append(f"{self.type.value} {self.value}" if self.value else self.type.value)
        
        if self.conditions:
            result.append(f"[if:{self.conditions}]")
        if self.actions:
            result.append(f"[do:{self.actions}]")
        
        return " ".join(result)

@dataclass
class Instruction:
    mnemonic: str
    operands: list[Operand]
    def __str__(self):
        ops = ", ".join(str(op) for op in self.operands)
        return f"{self.mnemonic} {ops}"

@dataclass
class Pattern:
    match: list[Instruction]
    replace: list[Instruction]
    