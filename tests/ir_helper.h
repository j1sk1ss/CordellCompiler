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

void print_irsub(const ir_subject_t* s) {
    if (!s) { printf("null"); return; }

    if (s->isreg) {
        int id = s->storage.rinfo.reg_id;
        const char* rname = "??";
        size_t rn = sizeof(_ir_reg_names)/sizeof(_ir_reg_names[0]);
        if (id >= 0 && id < (int)rn) rname = _ir_reg_names[id];
        printf("%s", rname);
        if (s->storage.rinfo.dref) printf("[*]");
    } 
    else {
        long id = s->storage.vinfo.obj_id;
        int size = s->storage.vinfo.size;
        int cnst = s->storage.vinfo.cnstvl;
        char instack = s->storage.vinfo.instack;
        printf("val(id=%ld", id);
        if (instack) printf(",stk");
        printf(",size=%d,cnst=%d)", size, cnst);
    }
}

void print_irblock(const ir_block_t* b) {
    if (!b) { printf("<null block>\n"); return; }

    const char* opname = "??";
    size_t on = sizeof(_ir_op_names)/sizeof(_ir_op_names[0]);
    if (b->op >= 0 && b->op < (int)on) opname = _ir_op_names[b->op];

    printf("[%s]", opname);
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