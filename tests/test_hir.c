#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include "ast_helper.h"

static int _depth = 0;
static const char* hir_op_to_string(hir_operation_t op) {
    switch(op) {
        case STARGLD:   return "STARGLD";
        case FARGST:    return "FARGST";
        case FARGLD:    return "FARGLD";
        case FCLL:      return "FCLL";
        case ECLL:      return "ECLL";
        case STRT:      return "STRT";
        case SYSC:      return "SYSC";
        case FRET:      return "FRET";
        case MKLB:      return "MKLB";
        case FDCL:      return "FDCL";
        case FEND:      return "FEND";
        case OEXT:      return "OEXT";
        case RESV:      return "RESV";
        case VDCL:      return "VDCL";
        case JMP:       return "JMP";
        case JE:        return "JE";
        case JNE:       return "JNE";
        case JL:        return "JL";
        case JG:        return "JG";
        case iADD:      return "iADD";
        case iSUB:      return "iSUB";
        case iMUL:      return "iMUL";
        case DIV:       return "DIV";
        case iDIV:      return "iDIV";
        case iMOD:      return "iMOD";
        case iLRG:      return "iLRG";
        case iLGE:      return "iLGE";
        case iLWR:      return "iLWR";
        case iLRE:      return "iLRE";
        case iCMP:      return "iCMP";
        case iNMP:      return "iNMP";
        case iAND:      return "iAND";
        case iOR:       return "iOR";
        case fADD:      return "fADD";
        case fSUB:      return "fSUB";
        case fMUL:      return "fMUL";
        case fDIV:      return "fDIV";
        case fCMP:      return "fCMP";
        case iBLFT:     return "iBLFT";
        case iBRHT:     return "iBRHT";
        case bAND:      return "bAND";
        case bOR:       return "bOR";
        case bXOR:      return "bXOR";
        case bSHL:      return "bSHL";
        case bSHR:      return "bSHR";
        case bSAR:      return "bSAR";
        case RAW:       return "RAW";
        case RSVSTK:    return "RSVSTK";
        case ADDOP:     return "ADDOP";
        case fADDOP:    return "fADDOP";
        case SUBOP:     return "SUBOP";
        case fSUBOP:    return "fSUBOP";
        case DIVOP:     return "DIVOP";
        case fDIVOP:    return "fDIVOP";
        case MODOP:     return "MODOP";
        case IFOP:      return "IFOP";
        case WHILEOP:   return "WHILEOP";
        case SWITCHOP:  return "SWITCHOP";
        case MKCASE:    return "MKCASE";
        case MKDEFCASE: return "MKDEFCASE";
        case MKENDCASE: return "MKENDCASE";
        case NOT:       return "NOT";
        case LOADOP:    return "LOADOP";
        case LDLINK:    return "LDLINK";
        case STORE:     return "STORE";
        case STLINK:    return "STLINK";
        case VARDECL:   return "VARDECL";
        case ARRDECL:   return "ARRDECL";
        case STRDECL:   return "STRDECL";
        case PRMST:     return "PRMST";
        case PRMLD:     return "PRMLD";
        case PRMPOP:    return "PRMPOP";
        case STASM:     return "STASM";
        case ENDASM:    return "ENDASM";
        case GINDEX:    return "GINDEX";
        case LINDEX:    return "LINDEX";
        case GDREF:     return "GDREF";
        case LDREF:     return "LDREF";
        case REF:       return "REF";
        case ALLCH:     return "ALLCH";
        case DEALLH:    return "DEALLH";
        case EXITOP:    return "EXITOP";
        case MKSCOPE:   _depth++; return "MKSCOPE";
        case ENDSCOPE:  _depth--; return "ENDSCOPE";
        default:        return "UNKNOWN_OP";
    }
}

static void print_hir_subject(const hir_subject_t* s) {
    if (!s) return;
    switch (s->t) {
        case REGISTER:   printf("reg: [%d]", s->storage.reg.reg);         break;
        case STKVARSTR:  printf("strs: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARARR:  printf("arrs: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARF64:  printf("f64s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARU64:  printf("u64s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARI64:  printf("i64s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARF32:  printf("f32s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARU32:  printf("u32s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARI32:  printf("i32s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARU16:  printf("u16s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARI16:  printf("i16s: [vid=%d]", s->storage.var.v_id);   break;
        case STKVARU8:   printf("u8s: [vid=%d]", s->storage.var.v_id);    break;
        case STKVARI8:   printf("i8s: [vid=%d]", s->storage.var.v_id);    break;
        case TMPVARSTR:  printf("strt: [tid=%d]", s->id);                 break;
        case TMPVARARR:  printf("arrt: [tid=%d]", s->id);                 break;
        case TMPVARF64:  printf("f64t: [tid=%d]", s->id);                 break;
        case TMPVARU64:  printf("u64t: [tid=%d]", s->id);                 break;
        case TMPVARI64:  printf("i64t: [tid=%d]", s->id);                 break;
        case TMPVARF32:  printf("f32t: [tid=%d]", s->id);                 break;
        case TMPVARU32:  printf("u32t: [tid=%d]", s->id);                 break;
        case TMPVARI32:  printf("i32t: [tid=%d]", s->id);                 break;
        case TMPVARU16:  printf("u16t: [tid=%d]", s->id);                 break;
        case TMPVARI16:  printf("i16t: [tid=%d]", s->id);                 break;
        case TMPVARU8:   printf("u8t: [tid=%d]", s->id);                  break;
        case TMPVARI8:   printf("i8t: [tid=%d]", s->id);                  break;
        case GLBVARSTR:  printf("strg: [%s]", s->storage.gvar.name);      break;
        case GLBVARARR:  printf("arrg: [%s]", s->storage.gvar.name);      break;
        case GLBVARF64:  printf("f64g: [%s]", s->storage.gvar.name);      break;
        case GLBVARU64:  printf("u64g: [%s]", s->storage.gvar.name);      break;
        case GLBVARI64:  printf("i64g: [%s]", s->storage.gvar.name);      break;
        case GLBVARF32:  printf("f32g: [%s]", s->storage.gvar.name);      break;  
        case GLBVARU32:  printf("u32g: [%s]", s->storage.gvar.name);      break;
        case GLBVARI32:  printf("i32g: [%s]", s->storage.gvar.name);      break;
        case GLBVARU16:  printf("u16g: [%s]", s->storage.gvar.name);      break;
        case GLBVARI16:  printf("i16g: [%s]", s->storage.gvar.name);      break;
        case GLBVARU8:   printf("i8g: [%s]", s->storage.gvar.name);       break;
        case GLBVARI8:   printf("u8g: [%s]", s->storage.gvar.name);       break;
        case NUMBER:     printf("%s", s->storage.num.value);              break;
        case CONSTVAL:   printf("%ld", s->storage.cnst.value);            break;
        case LABEL:      printf("lb: [id=%d]", s->id);                    break;
        case RAWASM:     printf("asm: [std_id=%i]", s->storage.str.s_id); break;
        case STRING:     printf("str: [std_id=%i]", s->storage.str.s_id); break;
        case FNAME:      printf("func: [fid=%i]", s->storage.str.s_id);   break;
        default: printf("unknw"); break;
    }
}

void print_hir_block(const hir_block_t* block) {
    if (!block) return;
    for (int i = 0; i < _depth; i++) printf("   ");
    printf("%s ", hir_op_to_string(block->op), block->args);
    print_hir_subject(block->farg); if (block->sarg) printf(", ");
    print_hir_subject(block->sarg); if (block->targ) printf(", ");
    print_hir_subject(block->targ); printf("\n");
}

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(tkn);
    MRKP_variables(tkn);

    sym_table_t smt = {
        .a = { .h = NULL },
        .v = { .h = NULL },
        .f = { .h = NULL }
    };
    
    syntax_ctx_t sctx  = { .r = NULL };

    STX_create(tkn, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .cid = 0, .h = NULL, .lid = 0, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);
    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    printf("==========   VARS  ==========\n");
    variable_info_t* vh = smt.v.h;
    while (vh) {
        printf("id: %i, name: %s, scope: %i\n", vh->v_id, vh->name, vh->s_id);
        vh = vh->next;
    }

    printf("==========   ARRS  ==========\n");
    array_info_t* ah = smt.a.h;
    while (ah) {
        printf("id: %i, name: %s, scope: %i\n", ah->v_id, ah->name, ah->s_id);
        ah = ah->next;
    }

    printf("==========  FUNCS  ==========\n");
    func_info_t* fh = smt.f.h;
    while (fh) {
        printf("id: %i, name: %s\n", fh->id, fh->name);
        fh = fh->next;
    }

    printf("========== STRINGS ==========\n");
    str_info_t* sh = smt.s.h;
    while (sh) {
        printf("id: %i, val: %s\n", sh->id, sh->value);
        sh = sh->next;
    }

    HIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

