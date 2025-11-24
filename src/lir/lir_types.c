#include <lir/lir_types.h>

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

int LIR_movop(lir_operation_t op) {
    switch (op) {
        case LIR_CDQ:
        case LIR_XCHG:
        case LIR_STARGLD:
        case LIR_STARGRF:
        case LIR_LOADFRET:
        case LIR_LOADFARG:
        case LIR_NOT:
        case LIR_CVTTSS2SI:
        case LIR_CVTTSD2SI:
        case LIR_CVTSI2SS:
        case LIR_CVTSI2SD:
        case LIR_CVTSS2SD:
        case LIR_CVTSD2SS:
        case LIR_aMOV:
        case LIR_iMOV:
        case LIR_MOVSX:
        case LIR_MOVSXD:
        case LIR_MOVZX:
        case LIR_fMOV: return 1;
        default: return 0;
    }
}

int LIR_writeop(lir_operation_t op) {
    switch (op) {
        case LIR_POP:
        case LIR_bXOR:
        case LIR_bSHL:
        case LIR_bSHR:
        case LIR_bSAR:
        case LIR_fADD: 
        case LIR_fSUB: 
        case LIR_fMUL: 
        case LIR_fDIV: 
        case LIR_iADD: 
        case LIR_iSUB: 
        case LIR_iMUL: 
        case LIR_DIV:  
        case LIR_iDIV: 
        case LIR_GDREF:
        case LIR_REF: return 1;
        default: return LIR_movop(op);
    }
}

int LIR_readop(lir_operation_t op) {
    switch (op) {
        case LIR_TST:
        case LIR_CMP:
        case LIR_FRET:
        case LIR_PUSH:
        case LIR_aMOV:
        case LIR_VRUSE:
        case LIR_EXITOP: return 1;
        default: return LIR_writeop(op);
    }
}
