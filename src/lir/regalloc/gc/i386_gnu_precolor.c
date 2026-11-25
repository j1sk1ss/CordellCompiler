#include <lir/regalloc/i386_gnu_precolor.h>

long i386_gnu_precolored_reg_to_color(lir_registers_t reg) {
    switch (LIR_format_register(reg, 4)) {
        case EBX:  return 0;
        case EDX:  return 1;
        case XMM0: return 0;
        case XMM1: return 1;
        case XMM2: return 2;
        case XMM3: return 3;
        case XMM4: return 4;
        case XMM5: return 5;
        case XMM6: return 6;
        case XMM7: return 7;
        default:   return reg;
    }
}