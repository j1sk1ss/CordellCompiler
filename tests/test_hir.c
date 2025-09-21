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
        case HIR_STARGLD:   return "STARGLD";
        case HIR_FARGST:    return "FARGST";
        case HIR_FARGLD:    return "FARGLD";
        case HIR_FCLL:      return "FCLL";
        case HIR_ECLL:      return "ECLL";
        case HIR_STRT:      return "STRT";
        case HIR_SYSC:      return "SYSC";
        case HIR_FRET:      return "FRET";
        case HIR_MKLB:      return "MKLB";
        case HIR_FDCL:      return "FDCL";
        case HIR_FEND:      return "FEND";
        case HIR_OEXT:      return "OEXT";
        case HIR_JMP:       return "JMP";
        case HIR_iADD:      return "iADD";
        case HIR_iSUB:      return "iSUB";
        case HIR_iMUL:      return "iMUL";
        case HIR_iDIV:      return "iDIV";
        case HIR_iMOD:      return "iMOD";
        case HIR_iLRG:      return "iLRG";
        case HIR_iLGE:      return "iLGE";
        case HIR_iLWR:      return "iLWR";
        case HIR_iLRE:      return "iLRE";
        case HIR_iCMP:      return "iCMP";
        case HIR_iNMP:      return "iNMP";
        case HIR_iAND:      return "iAND";
        case HIR_iOR:       return "iOR";
        case HIR_iBLFT:     return "iBLFT";
        case HIR_iBRHT:     return "iBRHT";
        case HIR_bAND:      return "bAND";
        case HIR_bOR:       return "bOR";
        case HIR_bXOR:      return "bXOR";
        case HIR_bSHL:      return "bSHL";
        case HIR_bSHR:      return "bSHR";
        case HIR_bSAR:      return "bSAR";
        case HIR_RAW:       return "RAW";
        case HIR_IFOP:      return "IFOP";
        case HIR_SWITCHOP:  return "SWITCHOP";
        case HIR_MKCASE:    return "MKCASE";
        case HIR_MKDEFCASE: return "MKDEFCASE";
        case HIR_MKENDCASE: return "MKENDCASE";
        case HIR_NOT:       return "NOT";
        case HIR_LOADOP:    return "LOADOP";
        case HIR_LDLINK:    return "LDLINK";
        case HIR_STORE:     return "STORE";
        case HIR_STLINK:    return "STLINK";
        case HIR_VARDECL:   return "VARDECL";
        case HIR_ARRDECL:   return "ARRDECL";
        case HIR_STRDECL:   return "STRDECL";
        case HIR_PRMST:     return "PRMST";
        case HIR_PRMLD:     return "PRMLD";
        case HIR_PRMPOP:    return "PRMPOP";
        case HIR_STASM:     return "STASM";
        case HIR_ENDASM:    return "ENDASM";
        case HIR_GINDEX:    return "GINDEX";
        case HIR_LINDEX:    return "LINDEX";
        case HIR_GDREF:     return "GDREF";
        case HIR_LDREF:     return "LDREF";
        case HIR_REF:       return "REF";
        case HIR_EXITOP:    return "EXITOP";
        case HIR_MKSCOPE:   _depth++; return "MKSCOPE";
        case HIR_ENDSCOPE:  _depth--; return "ENDSCOPE";
        default: return "unknwop";
    }
}

static void print_hir_subject(const hir_subject_t* s) {
    if (!s) return;
    switch (s->t) {
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
        case GLBVARSTR:  printf("strg: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARARR:  printf("arrg: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARF64:  printf("f64g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARU64:  printf("u64g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARI64:  printf("i64g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARF32:  printf("f32g: [gid=%i]", s->storage.var.v_id);   break;  
        case GLBVARU32:  printf("u32g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARI32:  printf("i32g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARU16:  printf("u16g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARI16:  printf("i16g: [gid=%i]", s->storage.var.v_id);   break;
        case GLBVARU8:   printf("i8g: [git=%i]", s->storage.var.v_id);    break;
        case GLBVARI8:   printf("u8g: [git=%i]", s->storage.var.v_id);    break;
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
    for (int i = 0; i < _depth; i++) printf("    ");
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

