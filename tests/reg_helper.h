#ifndef REG_HELPER_H_
#define REG_HELPER_H_
#include <lir/lir_types.h>
static const char* register_to_string(int reg) {
    switch(reg) {
        case XMM0: return "xmm0";
        case XMM1: return "xmm1";

        case RAX:  return "rax";
        case RBX:  return "rbx";
        case RCX:  return "rcx";
        case RDX:  return "rdx";
        case RSI:  return "rsi";
        case RDI:  return "rdi";
        case RBP:  return "rbp";
        case RSP:  return "rsp";

        case R8:   return "r8";
        case R9:   return "r9";
        case R10:  return "r10";
        case R11:  return "r11";
        case R12:  return "r12";
        case R13:  return "r13";
        case R14:  return "r14";
        case R15:  return "r15";

        case R8D:  return "r8d";
        case R9D:  return "r9d";
        case R10D: return "r10d";
        case R11D: return "r11d";
        case R12D: return "r12d";
        case R13D: return "r13d";
        case R14D: return "r14d";
        case R15D: return "r15d";

        case R8W:  return "r8w";
        case R9W:  return "r9w";
        case R10W: return "r10w";
        case R11W: return "r11w";
        case R12W: return "r12w";
        case R13W: return "r13w";
        case R14W: return "r14w";
        case R15W: return "r15w";

        case R8B:  return "r8b";
        case R9B:  return "r9b";
        case R10B: return "r10b";
        case R11B: return "r11b";
        case R12B: return "r12b";
        case R13B: return "r13b";
        case R14B: return "r14b";
        case R15B: return "r15b";

        case EAX: return "eax";
        case EBX: return "ebx";
        case ECX: return "ecx";
        case EDX: return "edx";
        case ESI: return "esi";
        case EDI: return "edi";
        case EBP: return "ebp";
        case ESP: return "esp";

        case AX: return "ax";
        case BX: return "bx";
        case CX: return "cx";
        case DX: return "dx";

        case AL: return "al";
        case BL: return "bl";
        case CL: return "cl";
        case DL: return "dl";

        case AH: return "ah";
        case BH: return "bh";
        case CH: return "ch";
        case DH: return "dh";

        default: return "??";
    }
}
#endif