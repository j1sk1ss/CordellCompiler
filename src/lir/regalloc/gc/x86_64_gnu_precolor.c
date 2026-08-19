#include <lir/regalloc/x86_64_gnu_precolor.h>

long x86_64_gnu_precolored_reg_to_color(lir_registers_t reg) {
    switch (LIR_format_register(reg, 8)) {
        case RCX: return 0;
        case RDX: return 1;
        case RBX: return 2;
        case RSI: return 3;
        case RDI: return 4;
        case R8:  return 5;
        case R9:  return 6;
        case R10: return 7;
        case R11: return 8;
        case R12: return 9;
        case R13: return 10;
        case R14: return 11;
        default:  return reg;
    }
}