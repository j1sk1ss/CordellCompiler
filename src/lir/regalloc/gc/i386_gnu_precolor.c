#include <lir/regalloc/i386_gnu_precolor.h>

long i386_gnu_precolored_reg_to_color(lir_registers_t reg) {
    switch (LIR_format_register(reg, 4)) {
        case EBX: return 0;
        case EDX: return 1;
        default:  return reg;
    }
}