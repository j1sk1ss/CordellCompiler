#ifndef REG_HELPER_H_
#define REG_HELPER_H_
#include <lir/lir_types.h>
static const char* register_to_string(int reg) {
    switch(reg) {
        case XMM0: return "XMM0";
        case XMM1: return "XMM1";

        case RAX:  return "RAX";
        case RBX:  return "RBX";
        case RCX:  return "RCX";
        case RDX:  return "RDX";
        case RSI:  return "RSI";
        case RDI:  return "RDI";
        case RBP:  return "RBP";
        case RSP:  return "RSP";

        case R8:   return "R8";
        case R9:   return "R9";
        case R10:  return "R10";
        case R11:  return "R11";
        case R12:  return "R12";
        case R13:  return "R13";
        case R14:  return "R14";
        case R15:  return "R15";

        case R8D:  return "R8D";
        case R9D:  return "R9D";
        case R10D: return "R10D";
        case R11D: return "R11D";
        case R12D: return "R12D";
        case R13D: return "R13D";
        case R14D: return "R14D";
        case R15D: return "R15D";

        case R8W:  return "R8W";
        case R9W:  return "R9W";
        case R10W: return "R10W";
        case R11W: return "R11W";
        case R12W: return "R12W";
        case R13W: return "R13W";
        case R14W: return "R14W";
        case R15W: return "R15W";

        case R8B:  return "R8B";
        case R9B:  return "R9B";
        case R10B: return "R10B";
        case R11B: return "R11B";
        case R12B: return "R12B";
        case R13B: return "R13B";
        case R14B: return "R14B";
        case R15B: return "R15B";

        case EAX: return "EAX";
        case EBX: return "EBX";
        case ECX: return "ECX";
        case EDX: return "EDX";
        case ESI: return "ESI";
        case EDI: return "EDI";
        case EBP: return "EBP";
        case ESP: return "ESP";

        case AX: return "AX";
        case BX: return "BX";
        case CX: return "CX";
        case DX: return "DX";

        case AL: return "AL";
        case BL: return "BL";
        case CL: return "CL";
        case DL: return "DL";

        case AH: return "AH";
        case BH: return "BH";
        case CH: return "CH";
        case DH: return "DH";

        default: return "UNKNOWN_REGISTER";
    }
}
#endif