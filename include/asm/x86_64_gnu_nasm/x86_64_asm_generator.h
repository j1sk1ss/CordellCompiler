#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <ir/ir.h>
#include <ir/irctx.h>
#include <ast/ast.h>
#include <ast/synctx.h>

#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)

static const char* _ir_reg_names[] = {
    "xmm0","xmm1",
    "rax","rbx","rcx","rdx","rsi","rdi","rbp","rsp","r8","r9","r10",
    "eax","ebx","ecx","edx","esi","edi","ebp","esp",
    "ax","bx","cx","dx",
    "al","bl","cl","dl",
    "ah","bh","ch","dh"
};

static inline const char* __format_ir_variable(ir_subject_t* v) {
    static char* buffer[128] = { 0 };
    if (v->isreg) {
        int id = v->storage.rinfo.reg_id;
        const char* rname = "";
        size_t rn = sizeof(_ir_reg_names )/ sizeof(_ir_reg_names[0]);
        if (id >= 0 && id < (int)rn) rname = _ir_reg_names[id];
        if (v->storage.rinfo.dref) snprintf(buffer, 128, "[%s]", rname);
        else snprintf(buffer, 128, "%s", rname);
        return buffer;
    } 

    int cnst = v->storage.vinfo.cnstvl;
    char instack = v->storage.vinfo.instack;
    if (!instack && v->storage.vinfo.pos.value[0]) snprintf(buffer, 128, "[rel %s]", v->storage.vinfo.pos.value);
    else if (v->storage.vinfo.pos.offset != 0) {
        if (v->storage.vinfo.pos.offset > 0) snprintf(buffer, 128, "[rbp - %i]", v->storage.vinfo.pos.offset);
        else snprintf(buffer, 128, "[rbp + %i]", -1 * v->storage.vinfo.pos.offset);
    }
    else snprintf(buffer, 128, "%d", cnst);
    return buffer;
}

#define GET_IRVAR(n) __format_ir_variable(n)

int x86_64_generate_asm(ir_block_t* h, FILE* output);
int x86_64_generate_data(ast_node_t* node, FILE* output);

#endif