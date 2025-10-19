#include <lir/lir_types.h>

int LIR_is_global_hirtype(hir_subject_type_t t) {
    switch (t) {
        case HIR_GLBVARSTR:
        case HIR_GLBVARARR:
        case HIR_GLBVARF64:
        case HIR_GLBVARI64:
        case HIR_GLBVARU64:
        case HIR_GLBVARF32:
        case HIR_GLBVARI32:
        case HIR_GLBVARU32:
        case HIR_GLBVARI16:
        case HIR_GLBVARU16:
        case HIR_GLBVARI8:
        case HIR_GLBVARU8:  return 1;
        default: return 0;
    }
}

int LIR_get_asttype_size(token_type_t t) {
    switch (t) {
        case STR_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case I64_TYPE_TOKEN: return DEFAULT_TYPE_SIZE;
        case F32_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case I32_TYPE_TOKEN: return 4;
        case U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN: return 2;
        case U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:  return 1;
        default: return DEFAULT_TYPE_SIZE;
    }
}

int LIR_get_hirtype_size(hir_subject_type_t t) {
    switch (t) {
        case HIR_NUMBER:
        case HIR_CONSTVAL:
        case HIR_TMPVARF64:
        case HIR_TMPVARI64:
        case HIR_TMPVARU64:
        case HIR_STKVARF64:
        case HIR_STKVARI64:
        case HIR_STKVARU64:
        case HIR_GLBVARF64:
        case HIR_GLBVARI64:
        case HIR_GLBVARU64: return DEFAULT_TYPE_SIZE;
        case HIR_TMPVARF32:
        case HIR_TMPVARI32:
        case HIR_TMPVARU32:
        case HIR_STKVARF32:
        case HIR_STKVARI32:
        case HIR_STKVARU32:
        case HIR_GLBVARF32:
        case HIR_GLBVARI32:
        case HIR_GLBVARU32: return 4;
        case HIR_TMPVARI16:
        case HIR_TMPVARU16:
        case HIR_STKVARI16:
        case HIR_STKVARU16:
        case HIR_GLBVARI16:
        case HIR_GLBVARU16: return 2;
        case HIR_TMPVARI8:
        case HIR_TMPVARU8:
        case HIR_STKVARI8:
        case HIR_STKVARU8:
        case HIR_GLBVARI8:
        case HIR_GLBVARU8:  return 1;
        default: return DEFAULT_TYPE_SIZE;
    }
}

int LIR_move_instruction(lir_operation_t op) {
    switch (op) {
        case LIR_iMOV:
        case LIR_fMOV:
        case LIR_REF: return 1;
        default: return 0;
    }
}

int LIR_jmp_instruction(lir_operation_t op) {
    switch (op) {
        case LIR_JMP:
        case LIR_JA:
        case LIR_JAE:
        case LIR_JB:
        case LIR_JBE:
        case LIR_JE:
        case LIR_JNE:
        case LIR_JL:
        case LIR_JLE:
        case LIR_JG:
        case LIR_JGE: return 1;
        default: return 0;
    }
}

int LIR_sysc_reg(lir_registers_t reg) {
    switch (reg) {
        case RAX: 
        case RDI: 
        case RSI: 
        case RDX: 
        case R10: 
        case R8: 
        case R9: return 1;
        default: return 0;
    }
}

int LIR_funccall_reg(lir_registers_t reg) {
    switch (reg) {
        case RDI:
        case RSI:
        case RDX:
        case RCX:
        case R8:
        case R9: return 1;
        default: return 0;
    }
}

lir_registers_t LIR_format_register(lir_registers_t reg, int size) {
    switch (reg) {
        case RAX: case EAX: case AX: case AL: case AH:
            if (size == 8) return RAX;
            if (size == 4) return EAX;
            if (size == 2) return AX;
            return AL;

        case RBX: case EBX: case BX: case BL: case BH:
            if (size == 8) return RBX;
            if (size == 4) return EBX;
            if (size == 2) return BX;
            return BL;

        case RCX: case ECX: case CX: case CL: case CH:
            if (size == 8) return RCX;
            if (size == 4) return ECX;
            if (size == 2) return CX;
            return CL;

        case RDX: case EDX: case DX: case DL: case DH:
            if (size == 8) return RDX;
            if (size == 4) return EDX;
            if (size == 2) return DX;
            return DL;

        case RSI: case ESI: case SI: case SIL:
            if (size == 8) return RSI;
            if (size == 4) return ESI;
            if (size == 2) return SI;
            return SIL;

        case RDI: case EDI: case DI: case DIL:
            if (size == 8) return RDI;
            if (size == 4) return EDI;
            if (size == 2) return DI;
            return DIL;

        case RBP: case EBP: case BP: case BPL:
            if (size == 8) return RBP;
            if (size == 4) return EBP;
            if (size == 2) return BP;
            return BPL;

        case RSP: case ESP: case SP: case SPL:
            if (size == 8) return RSP;
            if (size == 4) return ESP;
            if (size == 2) return SP;
            return SPL;

        case R8: case R8D: case R8W: case R8B:
            if (size == 8) return R8; 
            if (size == 4) return R8D;
            if (size == 2) return R8W;
            if (size == 1) return R8B;
            return R8B;

        case R9: case R9D: case R9W: case R9B:
            if (size == 8) return R9;
            if (size == 4) return R9D;
            if (size == 2) return R9W;
            if (size == 1) return R9B;
            return R9B;

        case R10: case R10D: case R10W: case R10B:
            if (size == 8) return R10;
            if (size == 4) return R10D;
            if (size == 2) return R10W;
            if (size == 1) return R10B;
            return R10B;

        case R11: case R11D: case R11W: case R11B:
            if (size == 8) return R11; 
            if (size == 4) return R11D;
            if (size == 2) return R11W;
            if (size == 1) return R11B;
            return R11B;

        case R12: case R12D: case R12W: case R12B:
            if (size == 8) return R12;
            if (size == 4) return R12D;
            if (size == 2) return R12W;
            if (size == 1) return R12B;
            return R12B;

        case R13: case R13D: case R13W: case R13B:
            if (size == 8) return R13;
            if (size == 4) return R13D;
            if (size == 2) return R13W;
            if (size == 1) return R13B;
            return R13B;

        case R14: case R14D: case R14W: case R14B:
            if (size == 8) return R14;
            if (size == 4) return R14D;
            if (size == 2) return R14W;
            if (size == 1) return R14B;
            return R14B;

        case R15: case R15D: case R15W: case R15B:
            if (size == 8) return R15;
            if (size == 4) return R15D;
            if (size == 2) return R15W;
            if (size == 1) return R15B;
            return R15B;

        case XMM0: return XMM0;
        case XMM1: return XMM1;
        case XMM2: return XMM2;
        case XMM3: return XMM3;
        case XMM4: return XMM4;
    }

    return reg;
}
