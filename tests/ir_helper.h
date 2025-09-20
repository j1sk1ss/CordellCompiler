#ifndef IR_HELPER_H_
#define IR_HELPER_H_
#include <lir/lir.h>
#include <stdio.h>

static const char* _irh_ir_op_names[] = {
    "FCLL","ECLL","STRT","SYSC","FRET","TDBL","TST","XCHG","CDQ",
    "MKLB","FDCL","OEXT",
    "SETL","SETG","STLE","STGE","SETE","STNE","SETB","SETA","STBE","STAE",
    "RESV","VDCL",
    "JMP","JE","JNE","JL","JG",
    "iMOV","iMOVb","iMOVw","iMOVd","iMOVq","iMVZX",
    "fMOV","fMVf","LEA","PUSH","POP",
    "iADD","iSUB","iMUL","DIV","iDIV","iMOD","iLRG","iLGE","iLWR","iLRE","iCMP",
    "iAND","iOR",
    "fADD","fSUB","fMUL","fDIV","fCMP",
    "bAND","bOR","bXOR","bSHL","bSHR","bSAR",
    "RAW"
};

static const char* _irh_ir_reg_names[] = {
    "XMM0","XMM1",
    "RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP","R8","R9","R10",
    "EAX","EBX","ECX","EDX","ESI","EDI","EBP","ESP",
    "AX","BX","CX","DX",
    "AL","BL","CL","DL",
    "AH","BH","CH","DH"
};

static inline const char* __irh_format_ir_variable(lir_subject_t* v) {
    static char irh_buffer[128] = { 0 };
    if (!v) return irh_buffer;
    if (v->isreg) {
        int id = v->storage.rinfo.reg_id;
        const char* rname = "";
        size_t rn = sizeof(_irh_ir_reg_names )/ sizeof(_irh_ir_reg_names[0]);
        if (id >= 0 && id < (int)rn) rname = _irh_ir_reg_names[id];
        if (!v->storage.rinfo.dref) snprintf(irh_buffer, 128, "%s", rname);
        else {
            switch (v->size) {
                case 1: snprintf(irh_buffer, 128, "byte [%s]", rname); break;
                case 2: snprintf(irh_buffer, 128, "word [%s]", rname); break;
                case 4: snprintf(irh_buffer, 128, "dword [%s]", rname); break;
                case 8: snprintf(irh_buffer, 128, "qword [%s]", rname); break;
                default: snprintf(irh_buffer, 128, "[%s]", rname);      break;
            }
        }

        return irh_buffer;
    } 

    int cnst = v->storage.vinfo.cnstvl;
    char instack = v->storage.vinfo.instack;
    if (!instack && v->storage.vinfo.glob) snprintf(irh_buffer, 128, "[rel %s]", v->storage.vinfo.pos.value);
    else if (!instack && !v->storage.vinfo.glob && cnst == -1) snprintf(irh_buffer, 128, "%s", v->storage.vinfo.pos.value);
    else if (v->storage.vinfo.pos.offset != 0) {
        const char* modifier = "";
        switch (v->size) {
            case 1: modifier = "byte ";  break;
            case 2: modifier = "word ";  break;
            case 4: modifier = "dword "; break;
            case 8: modifier = "qword "; break;
            default: break;
        }

        if (v->storage.vinfo.pos.offset > 0) snprintf(irh_buffer, 128, "%s[rbp - %i]", modifier, v->storage.vinfo.pos.offset);
        else snprintf(irh_buffer, 128, "%s[rbp + %i]", modifier, -1 * v->storage.vinfo.pos.offset);
    }
    else {
        snprintf(irh_buffer, 128, "%d", cnst);
    }

    return irh_buffer;
}

void print_irsub(lir_subject_t* s) {
    if (!s) { printf("null"); return; }
    printf("%s", __irh_format_ir_variable(s));
}

void print_irblock(lir_block_t* b) {
    if (!b) { printf("<null block>\n"); return; }

    const char* opname = "??";
    size_t on = sizeof(_irh_ir_op_names)/sizeof(_irh_ir_op_names[0]);
    if (b->op >= 0 && b->op < (int)on) opname = _irh_ir_op_names[b->op];

    printf("%s", opname);
    if (b->args <= 0) {
        printf("\n");
        return;
    }

    printf(" ");
    print_irsub(b->farg);

    if (b->args > 1) {
        printf(", ");
        print_irsub(b->sarg);
    }

    if (b->args > 2) {
        printf(", ");
        print_irsub(b->targ);
    }
    
    printf("\n");
}
#endif