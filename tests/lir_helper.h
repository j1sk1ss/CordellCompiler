#ifndef LIR_HELPER_H_
#define LIR_HELPER_H_

#include <lir/lir.h>
#include <lir/lir_types.h>
#include "reg_helper.h"

static const char* lir_op_to_string(lir_operation_t op) {
    switch(op) {
        case LIR_iMVSX: return "iMVSX";
        case LIR_MKGLB: return "MKGLB";
        case LIR_FCLL:  return "FCLL";
        case LIR_ECLL:  return "ECLL";
        case LIR_STRT:  return "STRT";
        case LIR_SYSC:  return "SYSC";
        case LIR_FRET:  return "FRET";
        case LIR_TINT:  return "TINT";
        case LIR_TDBL:  return "TDBL";
        case LIR_TST:   return "TST";
        case LIR_XCHG:  return "XCHG";
        case LIR_CDQ:   return "CDQ";
        case LIR_MKLB:  return "MKLB";
        case LIR_FDCL:  return "FDCL";
        case LIR_OEXT:  return "OEXT";

        case LIR_SETL: return "SETL";
        case LIR_SETG: return "SETG";
        case LIR_STLE: return "STLE";
        case LIR_STGE: return "STGE";
        case LIR_SETE: return "SETE";
        case LIR_STNE: return "STNE";
        case LIR_SETB: return "SETB";
        case LIR_SETA: return "SETA";
        case LIR_STBE: return "STBE";
        case LIR_STAE: return "STAE";

        case LIR_JL:  return "JL";
        case LIR_JG:  return "JG";
        case LIR_JLE: return "JLE";
        case LIR_JGE: return "JGE";
        case LIR_JE:  return "JE";
        case LIR_JNE: return "JNE";
        case LIR_JB:  return "JB";
        case LIR_JA:  return "JA";
        case LIR_JBE: return "JBE";
        case LIR_JAE: return "JAE";

        case LIR_RESV: return "RESV";
        case LIR_VDCL: return "VDCL";

        case LIR_JMP: return "JMP";

        case LIR_iMOV:  return "iMOV";
        case LIR_iMOVb: return "iMOVb";
        case LIR_iMOVw: return "iMOVw";
        case LIR_iMOVd: return "iMOVd";
        case LIR_iMOVq: return "iMOVq";
        case LIR_iMVZX: return "iMVZX";

        case LIR_fMOV:  return "fMOV";
        case LIR_fMVf:  return "fMVf";
        case LIR_REF:   return "REF";
        case LIR_GDREF: return "GDREF";
        case LIR_LDREF: return "LDREF";
        case LIR_PUSH:  return "PUSH";
        case LIR_POP:   return "POP";

        case LIR_iADD: return "iADD";
        case LIR_iSUB: return "iSUB";
        case LIR_iMUL: return "iMUL";
        case LIR_DIV:  return "DIV";
        case LIR_iDIV: return "iDIV";
        case LIR_iMOD: return "iMOD";
        case LIR_iLRG: return "iLRG";
        case LIR_iLGE: return "iLGE";
        case LIR_iLWR: return "iLWR";
        case LIR_iLRE: return "iLRE";
        case LIR_iCMP: return "iCMP";
        case LIR_iNMP: return "iNMP";

        case LIR_iAND: return "iAND";
        case LIR_iOR:  return "iOR";

        case LIR_fADD: return "fADD";
        case LIR_fSUB: return "fSUB";
        case LIR_fMUL: return "fMUL";
        case LIR_fDIV: return "fDIV";
        case LIR_fCMP: return "fCMP";

        case LIR_iBLFT: return "iBLFT";
        case LIR_iBRHT: return "iBRHT";
        case LIR_bAND:  return "bAND";
        case LIR_bOR:   return "bOR";
        case LIR_bXOR:  return "bXOR";
        case LIR_bSHL:  return "bSHL";
        case LIR_bSHR:  return "bSHR";
        case LIR_bSAR:  return "bSAR";

        case LIR_RAW: return "RAW";

        case LIR_RSVSTK: return "RSVSTK";
        case LIR_ADDOP:  return "ADDOP";
        case LIR_fADDOP: return "fADDOP";
        case LIR_SUBOP:  return "SUBOP";
        case LIR_fSUBOP: return "fSUBOP";
        case LIR_DIVOP:  return "DIVOP";
        case LIR_fDIVOP: return "fDIVOP";
        case LIR_MODOP:  return "MODOP";
        case LIR_LOADOP: return "LOADOP";
        case LIR_LDLINK: return "LDLINK";
        case LIR_STOP:   return "STOP";
        case LIR_STLINK: return "STLINK";
        case LIR_DECL:   return "DECL";
        case LIR_ALLCH:  return "ALLCH";
        case LIR_DEALLH: return "DEALLH";
        case LIR_EXITOP: return "EXITOP";

        default: return "[unkn]";
    }
}

static void print_lir_subject(const lir_subject_t* s) {
    if (!s) return;
    switch (s->t) {
        case LIR_REGISTER:   printf("%s", register_to_string(s->storage.reg.reg)); break;
        case LIR_STVARIABLE: printf("stk [id=%i]", s->storage.var.v_id);           break;
        case LIR_GLVARIABLE: printf("glb [id=%i]", s->storage.var.v_id);           break;
        case LIR_CONSTVAL:   printf("%i", s->storage.cnst.value);                  break;
        case LIR_NUMBER:     printf("%s", s->storage.num.value);                   break;
        case LIR_LABEL:      printf("lb: [vid=%d]", s->storage.str);               break;
        case LIR_RAWASM:     printf("raw [id=%d]", s->storage.str.sid);            break;
        case LIR_MEMORY:     s->storage.var.offset >= 0 ? 
                                printf("[RBP - %i]", s->storage.var.offset) :
                                printf("[RBP + %i]", -1 * s->storage.var.offset);  break;
        case LIR_FNAME:      printf("func [id=%d]", s->storage.str.sid);           break;
        case LIR_STRING:     printf("str [id=%d]", s->storage.str.sid);            break;
        default: return;
    }
}

static void print_lir_block(lir_block_t* b) {
    printf("%s", lir_op_to_string(b->op)); if (b->farg) printf(" ");
    print_lir_subject(b->farg); if (b->sarg) printf(", ");
    print_lir_subject(b->sarg); if (b->targ) printf(", ");
    print_lir_subject(b->targ);
    printf("\n");
}

#endif