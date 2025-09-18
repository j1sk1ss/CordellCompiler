#ifndef IR_HELPER_H_
#define IR_HELPER_H_
#include <ir/ir.h>
#include <stdio.h>

static const char* _ir_op_names[] = {
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

static const char* _ir_reg_names[] = {
    "XMM0","XMM1",
    "RAX","RBX","RCX","RDX","RSI","RDI","RBP","RSP","R8","R9","R10",
    "EAX","EBX","ECX","EDX","ESI","EDI","EBP","ESP",
    "AX","BX","CX","DX",
    "AL","BL","CL","DL",
    "AH","BH","CH","DH"
};

static inline const char* __format_ir_variable(ir_subject_t* v) {
    static char buffer[128] = { 0 };
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
    if (!instack && v->storage.vinfo.pos.value[0]) snprintf(buffer, 128, "%s", v->storage.vinfo.pos.value);
    else if (v->storage.vinfo.pos.offset != 0) {
        if (v->storage.vinfo.pos.offset > 0) snprintf(buffer, 128, "[rbp - %i]", v->storage.vinfo.pos.offset);
        else snprintf(buffer, 128, "[rbp + %i]", -1 * v->storage.vinfo.pos.offset);
    }
    else snprintf(buffer, 128, "%d", cnst);
    return buffer;
}

#define GET_IRVAR(n) __format_ir_variable(n)

void print_irsub(const ir_subject_t* s) {
    if (!s) { printf("null"); return; }
    printf("%s", GET_IRVAR(s));
}

void print_irblock(const ir_block_t* b) {
    if (!b) { printf("<null block>\n"); return; }

    const char* opname = "??";
    size_t on = sizeof(_ir_op_names)/sizeof(_ir_op_names[0]);
    if (b->op >= 0 && b->op < (int)on) opname = _ir_op_names[b->op];

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