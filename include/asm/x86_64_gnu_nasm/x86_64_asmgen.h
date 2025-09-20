#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <lir/lir.h>
#include <lir/irctx.h>
#include <ast/ast.h>
#include <ast/synctx.h>

#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)

static const char* _x86_64_ir_reg_names[] = {
    "xmm0","xmm1",
    "rax","rbx","rcx","rdx","rsi","rdi","rbp","rsp","r8","r9","r10",
    "eax","ebx","ecx","edx","esi","edi","ebp","esp",
    "ax","bx","cx","dx",
    "al","bl","cl","dl",
    "ah","bh","ch","dh"
};

static inline const char* __format_x86_64_ir_variable(lir_subject_t* v) {
    static char buffers[4][128];
    static int idx = 0;
    char* irh_buffer = buffers[idx];
    idx = (idx + 1) % 4;

    if (!v) {
        snprintf(irh_buffer, 128, "(null)");
        return irh_buffer;
    }

    if (v->isreg) {
        int id = v->storage.rinfo.reg_id;
        const char* rname = _x86_64_ir_reg_names[0];
        if (id >= 0 && id < (int)sizeof(_x86_64_ir_reg_names ) / sizeof(_x86_64_ir_reg_names[0])) rname = _x86_64_ir_reg_names[id];
        if (!v->storage.rinfo.dref) snprintf(irh_buffer, 128, "%s", rname);
        else {
            switch (v->size) {
                case 1: snprintf(irh_buffer, 128, "byte [%s]", rname);  break;
                case 2: snprintf(irh_buffer, 128, "word [%s]", rname);  break;
                case 4: snprintf(irh_buffer, 128, "dword [%s]", rname); break;
                case 8: snprintf(irh_buffer, 128, "qword [%s]", rname); break;
                default: snprintf(irh_buffer, 128, "[%s]", rname);      break;
            }
        }

        return irh_buffer;
    } 

    int cnst     = v->storage.vinfo.cnstvl;
    char instack = v->storage.vinfo.instack;
    char isglob  = v->storage.vinfo.glob;
    int offset   = v->storage.vinfo.pos.offset;

    if (!instack && isglob && cnst == -1) snprintf(irh_buffer, 128, "[rel __%s__]", v->storage.vinfo.pos.value);
    else if (!instack && !isglob && cnst == -1) snprintf(irh_buffer, 128, "%s", v->storage.vinfo.pos.value);
    else if (offset != 0) {
        const char* modifier = "";
        switch (v->size) {
            case 1: modifier = "byte ";  break;
            case 2: modifier = "word ";  break;
            case 4: modifier = "dword "; break;
            case 8: modifier = "qword "; break;
            default: break;
        }

        if (offset > 0) snprintf(irh_buffer, 128, "%s[rbp - %i]", modifier, offset);
        else snprintf(irh_buffer, 128, "%s[rbp + %i]", modifier, -1 * offset);
    }
    else {
        snprintf(irh_buffer, 128, "%d", cnst);
    }

    return irh_buffer;
}

#define GET_X86_64_IRVAR(n) __format_x86_64_ir_variable(n)

int x86_64_generate_asm(lir_block_t* h, FILE* output);
int x86_64_generate_data(ast_node_t* node, FILE* output);

#endif