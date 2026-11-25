#include <lir/lir_types.h>

lir_operation_t LIR_format_float_operation(lir_operation_t op, int is_float) {
    switch (op) {
        case LIR_CMP:  case LIR_FCMP: return is_float ? LIR_FCMP : LIR_CMP;
        case LIR_iMOV: case LIR_fMOV: return is_float ? LIR_fMOV : LIR_iMOV;
        case LIR_iADD: case LIR_fADD: return is_float ? LIR_fADD : LIR_iADD;
        case LIR_iSUB: case LIR_fSUB: return is_float ? LIR_fSUB : LIR_iSUB;
        case LIR_iMUL: case LIR_fMUL: return is_float ? LIR_fMUL : LIR_iMUL;
        case LIR_iDIV: case LIR_fDIV: return is_float ? LIR_fDIV : LIR_iDIV;
        case LIR_iLRG: case LIR_fLRG: return is_float ? LIR_fLRG : LIR_iLRG;
        case LIR_iLGE: case LIR_fLGE: return is_float ? LIR_fLGE : LIR_iLGE;
        case LIR_iLWR: case LIR_fLWR: return is_float ? LIR_fLWR : LIR_iLWR;
        case LIR_iLRE: case LIR_fLRE: return is_float ? LIR_fLRE : LIR_iLRE;
        case LIR_iCMP: case LIR_fCMP: return is_float ? LIR_fCMP : LIR_iCMP;
        case LIR_iNMP: case LIR_fNMP: return is_float ? LIR_fNMP : LIR_iNMP;
        default: return op;
    }
}

/* Returns 1 if the register belongs to the x86 SSE register bank. */
int LIR_is_xmm_register(lir_registers_t reg) {
    return reg >= XMM0 && reg <= XMM7;
}

lir_registers_t LIR_format_register(lir_registers_t reg, int size) {
#define CONVERTER(e, h, q, d)          \
    do {                               \
        if (size == 8) return e;       \
        if (size == 4) return h;       \
        if (size == 2) return q;       \
        return d;                      \
    } while (0);
    switch (reg) {
        /* x86_64/32/16 */
        case RAX: case EAX: case AX: case AL: case AH: CONVERTER(RAX, EAX, AX, AL);
        case RBX: case EBX: case BX: case BL: case BH: CONVERTER(RBX, EBX, BX, BL);
        case RCX: case ECX: case CX: case CL: case CH: CONVERTER(RCX, ECX, CX, CL);
        case RDX: case EDX: case DX: case DL: case DH: CONVERTER(RDX, EDX, DX, DL);
        case RSI: case ESI: case SI: case SIL:         CONVERTER(RSI, ESI, SI, SIL);
        case RDI: case EDI: case DI: case DIL:         CONVERTER(RDI, EDI, DI, DIL);
        case RBP: case EBP: case BP: case BPL:         CONVERTER(RBP, EBP, BP, BPL);
        case RSP: case ESP: case SP: case SPL:         CONVERTER(RSP, ESP, SP, SPL);
        case R8: case R8D: case R8W: case R8B:         CONVERTER(R8, R8D, R8W, R8B);
        case R9: case R9D: case R9W: case R9B:         CONVERTER(R9, R9D, R9W, R9B);
        case R10: case R10D: case R10W: case R10B:     CONVERTER(R10, R10D, R10W, R10B);
        case R11: case R11D: case R11W: case R11B:     CONVERTER(R11, R11D, R11W, R11B);
        case R12: case R12D: case R12W: case R12B:     CONVERTER(R12, R12D, R12W, R12B);
        case R13: case R13D: case R13W: case R13B:     CONVERTER(R13, R13D, R13W, R13B);
        case R14: case R14D: case R14W: case R14B:     CONVERTER(R14, R14D, R14W, R14B);
        case R15: case R15D: case R15W: case R15B:     CONVERTER(R15, R15D, R15W, R15B);
        case XMM0: return XMM0;
        case XMM1: return XMM1;
        case XMM2: return XMM2;
        case XMM3: return XMM3;
        case XMM4: return XMM4;
        /* RISC-V TODO */
        default: break;
    }
#undef CONVERTER
    return reg;
}

/* Check whether an operation writes its destination by moving a value into it.
Params:
    - `op` - LIR operation.

Returns 1 if operation is a value-moving write, otherwise 0. */
static int _is_move_write_by_value(lir_operation_t op) {
    switch (op) {
        case LIR_fNEG:     case LIR_fMOVQ:     case LIR_MOVSS:     case LIR_MOVSD:
        case LIR_NEG:      case LIR_NOT:
        case LIR_aMOV:     case LIR_iMOV:      case LIR_MOVZX:     case LIR_MOVSX:
        case LIR_phiMOV:   case LIR_MOVSXD:    case LIR_fMOV:      case LIR_fMVf:
        case LIR_MOVDQU:
        case LIR_XCHG:
        case LIR_STARGLD:  case LIR_STARGRF:
        case LIR_LOADFRET: case LIR_LOADFARG:
        case LIR_CVTSI2SS: case LIR_CVTSI2SD:  case LIR_CVTSS2SD: 
        case LIR_CVTSD2SS: case LIR_CVTTSS2SI: case LIR_CVTTSD2SI: return 1;
        default: return 0;
    }
}

int LIR_is_movop(lir_operation_t op) {
    switch (op) {
        case LIR_fLDREF:  case LIR_fSTARG:
        case LIR_LDREF: return 1;
        default: return _is_move_write_by_value(op);
    }
}

int LIR_is_writeop(lir_operation_t op) {
    switch (op) {
        case LIR_SETL: case LIR_SETG: case LIR_STLE: case LIR_STGE:
        case LIR_SETE: case LIR_STNE: case LIR_SETB: case LIR_SETA:
        case LIR_STBE: case LIR_STAE:
        case LIR_TF64: case LIR_TF32:
        case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8: 
        case LIR_TU64: case LIR_TU32: case LIR_TU16: case LIR_TU8:
        case LIR_POP:  case LIR_fPOP:
        case LIR_bXOR: case LIR_iBRHT: case LIR_iBLFT: case LIR_bSHL: case LIR_bSHR: case LIR_bSAR: case LIR_bAND: case LIR_bOR:
        case LIR_fADD: case LIR_fSUB:  case LIR_fMUL:  case LIR_fDIV: 
        case LIR_ADDSS: case LIR_ADDSD: case LIR_SUBSS: case LIR_SUBSD:
        case LIR_MULSS: case LIR_MULSD: case LIR_DIVSS: case LIR_DIVSD:
        case LIR_fLRG: case LIR_fLGE:  case LIR_fLWR:  case LIR_fLRE:
        case LIR_fCMP: case LIR_fNMP:
        case LIR_iADD: case LIR_iSUB:  case LIR_iMUL:  case LIR_iDIV: case LIR_iMOD:
        case LIR_DIV:  
        case LIR_fGDREF:  case LIR_XORPS:
        case LIR_GDREF:
        case LIR_REF_GDREF:
        case LIR_REF: return 1;
        default:      return _is_move_write_by_value(op);
    }
}

int LIR_is_readop(lir_operation_t op) {
    switch (op) {
        case LIR_TST:  case LIR_CMP:    case LIR_FCMP: case LIR_FRET:
        case LIR_UCOMISS: case LIR_UCOMISD:
        case LIR_FCLL: case LIR_ECLL:   case LIR_JMP:
        case LIR_PUSH: case LIR_fPUSH: case LIR_phiMOV: case LIR_aMOV:
        case LIR_fLDREF:  case LIR_fSTARG:
        case LIR_LDREF:
        case LIR_VRUSE:
        case LIR_EXITOP: return 1;
        default: return LIR_is_writeop(op);
    }
}

int LIR_has_sideeffect(lir_operation_t op) {
    switch (op) {
        case LIR_PUSH:   case LIR_fPUSH: case LIR_POP:   case LIR_fPOP:
        case LIR_FRET:
        case LIR_EXITOP: case LIR_LDREF: case LIR_fLDREF: case LIR_fSTARG: case LIR_FCLL:
        case LIR_ECLL:   case LIR_SYSC:  case LIR_phiMOV:
        case LIR_aMOV:   case LIR_MOVDQU: return 1;
        default:       return 0;
    }
}

int LIR_is_jumpop(lir_operation_t op) {
    switch (op) {
        case LIR_JMP: case LIR_JZ: case LIR_JNZ:
        case LIR_JL:  case LIR_JG: case LIR_JLE:
        case LIR_JGE: case LIR_JE: case LIR_JNE:
        case LIR_JB:  case LIR_JA: case LIR_JBE:
        case LIR_JAE: return 1;
        default:      return 0;
    }
}

int LIR_is_defined_type(lir_subject_type_t t) {
    switch (t) {
        case LIR_CONSTVAL: case LIR_NUMBER: return 1;
        default: return 0;
    }
}
