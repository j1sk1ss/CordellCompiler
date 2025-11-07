#include <lir/instplan/x86_64_gnu_nasm.h>

static const int _abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9      };
static const int _sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };

lir_block_t* x86_64_gnu_nasm_planner_get_next_func_abi(lir_block_t* entry, lir_block_t* exit, int offset) {
    if (entry == exit) return NULL;
    while (entry && entry->op != LIR_FCLL && entry->op != LIR_ECLL) {
        if (entry->farg && entry->farg->t == LIR_REGISTER && entry->farg->storage.reg.reg == _abi_regs[offset]) return entry;
        if (entry == exit) break;
        entry = entry->prev;
    }

    return NULL;
}

lir_block_t* x86_64_gnu_nasm_planner_get_next_sysc_abi(lir_block_t* entry, lir_block_t* exit, int offset) {
    if (entry == exit) return NULL;
    while (entry && entry->op != LIR_SYSC) {
        if (entry->farg && entry->farg->t == LIR_REGISTER && entry->farg->storage.reg.reg == _sys_regs[offset]) return entry;
        if (entry == exit) break;
        entry = entry->prev;
    }

    return NULL;
}

lir_block_t* x86_64_gnu_nasm_planner_get_func_res(lir_block_t* fn, lir_block_t* exit) {
    if (fn == exit) return NULL;
    lir_block_t* candidate = fn->next;
    if (
        candidate->op == LIR_iMOV && candidate->sarg->t == LIR_REGISTER && 
        LIR_format_register(candidate->sarg->storage.reg.reg, 1) == LIR_format_register(RAX, 1)
    ) {
        return candidate;
    }

    return NULL;
}
