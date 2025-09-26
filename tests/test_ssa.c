#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/opt/cfg.h>
#include <hir/opt/ssa.h>
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
        case HIR_RAW:       return "RAW";
        case HIR_IFOP:      return "IFOP";
        case HIR_NOT:       return "NOT";
        case HIR_STORE:     return "STORE";
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
        case HIR_CLNVRS:    return "CLNVRS";
        case HIR_IFLWOP:    return "IFLWOP";
        case HIR_IFLGOP:    return "IFLGOP";
        case HIR_STEND:     return "STEND";
        case HIR_PHI:       return "PHI";
        case HIR_MKSCOPE:   _depth++; return "MKSCOPE";
        case HIR_ENDSCOPE:  _depth--; return "ENDSCOPE";
        default: return "unknwop";
    }
}

static void print_hir_subject(hir_subject_t* s) {
    if (!s) return;
    switch (s->t) {
        case HIR_STKVARSTR:  printf("strs: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARARR:  printf("arrs: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARF64:  printf("f64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU64:  printf("u64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI64:  printf("i64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARF32:  printf("f32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU32:  printf("u32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI32:  printf("i32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU16:  printf("u16s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI16:  printf("i16s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU8:   printf("u8s: [vid=%d]", s->storage.var.v_id);    break;
        case HIR_STKVARI8:   printf("i8s: [vid=%d]", s->storage.var.v_id);    break;
        case HIR_TMPVARSTR:  printf("strt: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARARR:  printf("arrt: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARF64:  printf("f64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU64:  printf("u64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI64:  printf("i64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARF32:  printf("f32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU32:  printf("u32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI32:  printf("i32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU16:  printf("u16t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI16:  printf("i16t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU8:   printf("u8t: [tid=%d]", s->storage.var.v_id);    break;
        case HIR_TMPVARI8:   printf("i8t: [tid=%d]", s->storage.var.v_id);    break;
        case HIR_GLBVARSTR:  printf("strg: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARARR:  printf("arrg: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARF64:  printf("f64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU64:  printf("u64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI64:  printf("i64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARF32:  printf("f32g: [gid=%i]", s->storage.var.v_id);   break;  
        case HIR_GLBVARU32:  printf("u32g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI32:  printf("i32g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU16:  printf("u16g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI16:  printf("i16g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU8:   printf("i8g: [git=%i]", s->storage.var.v_id);    break;
        case HIR_GLBVARI8:   printf("u8g: [git=%i]", s->storage.var.v_id);    break;
        case HIR_NUMBER:     printf("%s", s->storage.num.value);              break;
        case HIR_CONSTVAL:   printf("%ld", s->storage.cnst.value);            break;
        case HIR_LABEL:      printf("lb: [id=%d]", s->id);                    break;
        case HIR_RAWASM:     printf("asm: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_STRING:     printf("str: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_FNAME:      printf("func: [fid=%i]", s->storage.str.s_id);   break;

        case HIR_SET: {
            set_iter_t it;
            set_iter_init(&s->storage.set.h, &it);
            int_tuple_t* tpl = NULL;
            printf("set: ");
            while ((tpl = set_iter_next_addr(&it))) printf("b: %i, v: %i; ", tpl->x, tpl->y);
            break;
        }

        default: printf("unknw"); break;
    }
}

void print_hir_block(const hir_block_t* block, int ud) {
    if (!block) return;
    if (ud) for (int i = 0; i < _depth; i++) printf("    ");
    printf("%s ", hir_op_to_string(block->op), block->args);
    print_hir_subject(block->farg); if (block->sarg) printf(", ");
    print_hir_subject(block->sarg); if (block->targ) printf(", ");
    print_hir_subject(block->targ); printf("\n");
}

/* https://dreampuf.github.io/GraphvizOnline/?engine=dot */
void _dump_domtree_dot(cfg_func_t* func) {
    printf("digraph DomTree {\n");
    printf("  rankdir=TB;\n");
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        printf("  B%i [label=\"B%i\"];\n", b->id, b->id);
    }

    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        if (!b->sdom) continue;
        printf("  B%i -> B%i;\n", b->sdom->id, b->id);
    }

    printf("}\n");
}

void _dump_all_dom_dot(cfg_func_t* func) {
    printf("digraph AllDom {\n");
    printf("  rankdir=TB;\n");

    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        printf("  B%i [label=\"B%i\"];\n", b->id, b->id);
    }

    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        set_iter_t it;
        set_iter_init(&b->dom, &it);
        cfg_block_t* d;
        while ((d = set_iter_next_addr(&it))) {
            if (d == b) continue;
            printf("  B%i -> B%i;\n", d->id, b->id);
        }
    }

    printf("}\n");
}

static int _export_dot_func(cfg_func_t* f) {
    printf("digraph CFG_func%d {\n", f->id);
    printf("  rankdir=TB;\n");
    cfg_block_t* b = f->cfg_head;
    while (b) {
        printf("  B%ld [label=\"B%ld:\\nentry=%s%i\\nexit=%s\"];\n",
               b->id, b->id,
               b->entry ? hir_op_to_string(b->entry->op) : "NULL", b->entry->op == HIR_MKLB ? b->entry->farg->id : -1,
               b->exit  ? hir_op_to_string(b->exit->op)  : "NULL");
        if (b->l)   printf("  B%ld -> B%ld [label=\"fall\"];\n", b->id, b->l->id);
        if (b->jmp) printf("  B%ld -> B%ld [label=\"jump\"];\n", b->id, b->jmp->id);
        b = b->next;
    }
    
    printf("}\n");
    return 1;
}

void cfg_print(cfg_ctx_t* ctx) {
    cfg_func_t* f = ctx->h;
    printf("==== CFG DUMP ====\n");

    while (f) {
        printf("==== CFG DOT ====\n");
        _export_dot_func(f);
        printf("==== DOM DOT ====\n");
        _dump_all_dom_dot(f);
        printf("==== SDOM DOT ====\n");
        _dump_domtree_dot(f);
        f = f->next;
    }

    printf("==================\n");
}

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        return 1;
    }

    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        close(fd);
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
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);

    cfg_ctx_t cfgctx = { .h = NULL };
    HIR_build_cfg(&irctx, &cfgctx);
    
    ssa_ctx_t ssactx = { .h = NULL };
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    cfg_print(&cfgctx);

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    if (smt.v.h) printf("==========   VARS  ==========\n");
    variable_info_t* vh = smt.v.h;
    while (vh) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vh->v_id, vh->name, vh->type, vh->s_id);
        vh = vh->next;
    }

    if (smt.a.h) printf("==========   ARRS  ==========\n");
    array_info_t* ah = smt.a.h;
    while (ah) {
        printf("id: %i, name: %s, scope: %i\n", ah->v_id, ah->name, ah->s_id);
        ah = ah->next;
    }

    if (smt.f.h) printf("==========  FUNCS  ==========\n");
    func_info_t* fh = smt.f.h;
    while (fh) {
        printf("id: %i, name: %s\n", fh->id, fh->name);
        fh = fh->next;
    }

    if (smt.s.h) printf("========== STRINGS ==========\n");
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

