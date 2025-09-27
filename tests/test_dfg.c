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
#include <hir/opt/dfg.h>
#include "ast_helper.h"
#include "hir_helper.h"
static int _depth = 0;

static const char* hir_op_to_fmtstring(hir_operation_t op, int state) {
    switch(op) {
        case HIR_STARGLD:   return "load_starg(%s);\n";
        case HIR_FARGST:    return "store_arg(%s);\n";
        case HIR_FARGLD:    return "load_arg(%s);\n";
        
        case HIR_FCLL:       return "call %s, argc %s;\n";
        case HIR_STORE_FCLL: return "%s = call %s, argc %s;\n";
        case HIR_ECLL:       return "excall %s, argc %s;\n";
        case HIR_STORE_ECLL: return "%s = excall %s, argc %s;\n";
        case HIR_SYSC:       return "syscall, argc: %s;\n";
        case HIR_STORE_SYSC: return "%s = syscall, argc: %s;\n";

        case HIR_STRT:      return "start {\n";
        case HIR_FRET:      return "return %s;\n";
        case HIR_MKLB:      return "%s:\n";
        case HIR_FDCL:      return "fn %s {\n";
        case HIR_FEND:      return "}\n";
        case HIR_OEXT:      return "extern %s;\n";
        case HIR_JMP:       return "goto %s;\n";
        case HIR_iADD:      return "%s = %s + %s;\n";
        case HIR_iSUB:      return "%s = %s - %s;\n";
        case HIR_iMUL:      return "%s = %s * %s;\n";
        case HIR_iDIV:      return "%s = %s / %s;\n";
        case HIR_iMOD:      return "%s = %s mod %s;\n";
        case HIR_iLRG:      return "%s = %s > %s;\n";
        case HIR_iLGE:      return "%s = %s >= %s;\n";
        case HIR_iLWR:      return "%s = %s < %s;\n";
        case HIR_iLRE:      return "%s = %s <= %s;\n";
        case HIR_iCMP:      return "%s = %s == %s;\n";
        case HIR_iNMP:      return "%s = %s != %s;\n";
        case HIR_iAND:      return "%s = %s && %s;\n";
        case HIR_iOR:       return "%s = %s || %s;\n";
        case HIR_iBLFT:     return "%s = %s << %s;\n";
        case HIR_iBRHT:     return "%s = %s >> %s;\n";
        case HIR_bAND:      return "%s = %s & %s;\n";
        case HIR_bOR:       return "%s = %s | %s;\n";
        case HIR_bXOR:      return "%s = %s ^ %s;\n";
        case HIR_RAW:       return "[raw, \"%s\"]\n";
        case HIR_IFOP:      return "if %s, goto %s;\n";
        case HIR_NOT:       return "%s = not %s;\n";
        case HIR_STORE:     return "%s = %s;\n";

        case HIR_VARDECL:
        case HIR_ARRDECL:
        case HIR_STRDECL: {
            switch (state) {
                case 2:  return "alloc %s;\n";
                default: return "alloc %s = %s;\n";
            }
        }

        case HIR_PRMST:     return "prm_st(%s);\n";
        case HIR_PRMLD:     return "prm_ld();\n";
        case HIR_PRMPOP:    return "prm_pop();\n";
        case HIR_STASM:     return "asm {\n";
        case HIR_ENDASM:    return "}\n";
        case HIR_GINDEX:    return "%s = %s[%s];\n";
        case HIR_LINDEX:    return "%s[%s] = %s;\n";
        case HIR_GDREF:     return "%s = *(%s);\n";
        case HIR_LDREF:     return "*(%s) = %s;\n";
        case HIR_REF:       return "%s = &(%s);\n";
        case HIR_CLNVRS:    return "delete(%s);\n";
        case HIR_IFLWOP:    return "if %s < %s, goto %s;\n";
        case HIR_IFLGOP:    return "if %s > %s, goto %s;\n";
        case HIR_EXITOP:    return "exit %s;\n";
        case HIR_STEND:     return "}\n";
        case HIR_PHI:       return "[%s] %s = phi(%s);\n";
        case HIR_MKSCOPE:   return "{\n";
        case HIR_ENDSCOPE:  return "}\n";
        default: return "\n";
    }
}

static char* sprintf_hir_subject(char* dst, hir_subject_t* s, sym_table_t* smt) {
    if (!s) return dst;
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) {
            switch (s->t) {
                case HIR_STKVARSTR:  dst += sprintf(dst, "strs ");   break;
                case HIR_STKVARARR:  dst += sprintf(dst, "arrs ");   break;
                case HIR_STKVARF64:  dst += sprintf(dst, "f64s ");   break;
                case HIR_STKVARU64:  dst += sprintf(dst, "u64s ");   break;
                case HIR_STKVARI64:  dst += sprintf(dst, "i64s ");   break;
                case HIR_STKVARF32:  dst += sprintf(dst, "f32s ");   break;
                case HIR_STKVARU32:  dst += sprintf(dst, "u32s ");   break;
                case HIR_STKVARI32:  dst += sprintf(dst, "i32s ");   break;
                case HIR_STKVARU16:  dst += sprintf(dst, "u16s ");   break;
                case HIR_STKVARI16:  dst += sprintf(dst, "i16s ");   break;
                case HIR_STKVARU8:   dst += sprintf(dst, "u8s ");    break;
                case HIR_STKVARI8:   dst += sprintf(dst, "i8s ");    break;
                case HIR_TMPVARSTR:  dst += sprintf(dst, "strt ");   break;
                case HIR_TMPVARARR:  dst += sprintf(dst, "arrt ");   break;
                case HIR_TMPVARF64:  dst += sprintf(dst, "f64t ");   break;
                case HIR_TMPVARU64:  dst += sprintf(dst, "u64t ");   break;
                case HIR_TMPVARI64:  dst += sprintf(dst, "i64t ");   break;
                case HIR_TMPVARF32:  dst += sprintf(dst, "f32t ");   break;
                case HIR_TMPVARU32:  dst += sprintf(dst, "u32t ");   break;
                case HIR_TMPVARI32:  dst += sprintf(dst, "i32t ");   break;
                case HIR_TMPVARU16:  dst += sprintf(dst, "u16t ");   break;
                case HIR_TMPVARI16:  dst += sprintf(dst, "i16t ");   break;
                case HIR_TMPVARU8:   dst += sprintf(dst, "u8t ");    break;
                case HIR_TMPVARI8:   dst += sprintf(dst, "i8t ");    break;
                case HIR_GLBVARSTR:  dst += sprintf(dst, "strg ");   break;
                case HIR_GLBVARARR:  dst += sprintf(dst, "arrg ");   break;
                case HIR_GLBVARF64:  dst += sprintf(dst, "f64g ");   break;
                case HIR_GLBVARU64:  dst += sprintf(dst, "u64g ");   break;
                case HIR_GLBVARI64:  dst += sprintf(dst, "i64g ");   break;
                case HIR_GLBVARF32:  dst += sprintf(dst, "f32g ");   break;  
                case HIR_GLBVARU32:  dst += sprintf(dst, "u32g ");   break;
                case HIR_GLBVARI32:  dst += sprintf(dst, "i32g ");   break;
                case HIR_GLBVARU16:  dst += sprintf(dst, "u16g ");   break;
                case HIR_GLBVARI16:  dst += sprintf(dst, "i16g ");   break;
                case HIR_GLBVARU8:   dst += sprintf(dst, "u8g ");    break;
                case HIR_GLBVARI8:   dst += sprintf(dst, "i8g ");    break;
                default: break;
            }

            dst += sprintf(dst, "%s%i", vi.name, vi.v_id);
        }
    }
    else {
        switch (s->t) {
            case HIR_NUMBER:     dst += sprintf(dst, "%s", s->storage.num.value);              break;
            case HIR_CONSTVAL:   dst += sprintf(dst, "%ld", s->storage.cnst.value);            break;
            case HIR_LABEL:      dst += sprintf(dst, "%d", s->id);                            break;

            case HIR_RAWASM:
            case HIR_STRING: {
                str_info_t si;
                if (STTB_get_info_id(s->storage.str.s_id, &si, &smt->s)) {
                    dst += sprintf(dst, "%s", si.value);
                }

                break;
            }

            case HIR_FNAME: {
                func_info_t fi;
                if (FNTB_get_info_id(s->storage.str.s_id, &fi, &smt->f)) {
                    dst += sprintf(dst, "%s(", fi.name);
                }

                if (fi.args) {
                    for (ast_node_t* t = fi.args->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
                        ast_node_t* type = t;
                        ast_node_t* name = t->child;
                        dst += sprintf(dst, "%s %s", fmt_tkn_type(type->token), name->token->value);
                        if (t->sibling && t->sibling->token->t_type != SCOPE_TOKEN) dst += sprintf(dst, ", ");
                    }
                }

                dst += sprintf(dst, ")");

                if (fi.rtype) {
                    dst += sprintf(dst, " -> %s", fmt_tkn_type(fi.rtype->token));
                }

                break;
            }

            case HIR_SET: {
                set_iter_t it;
                set_iter_init(&s->storage.set.h, &it);
                int_tuple_t* tpl = NULL;
                dst += sprintf(dst, "set: ");
                while ((tpl = set_iter_next_addr(&it))) {
                    variable_info_t pvi;
                    if (VRTB_get_info_id(tpl->y, &pvi, &smt->v)) {
                        dst += sprintf(dst, "[%s%i, bb%d]", pvi.name, pvi.v_id, tpl->x);
                    }
                }
                break;
            }

            default: dst += sprintf(dst, "unknw"); break;
        }
    }

    return dst;
}

void print_hir_block(const hir_block_t* block, int ud, sym_table_t* smt) {
    if (!block) return;

    char arg1[256] = { 0 };
    char arg2[256] = { 0 };
    char arg3[256] = { 0 };

    int args = 3;
    if (block->farg) {
        sprintf_hir_subject(arg1, block->farg, smt);
        args--;
    }

    if (block->sarg) {
        sprintf_hir_subject(arg2, block->sarg, smt);
        args--;
    }

    if (block->targ) {
        sprintf_hir_subject(arg3, block->targ, smt);
        args--;
    }

    char line[512] = { 0 };

    if (
        block->op == HIR_ENDSCOPE ||
        block->op == HIR_FEND     ||
        block->op == HIR_STEND
    ) _depth--;

    if (ud) for (int i = 0; i < _depth; i++) printf("    ");
    const char* fmt = hir_op_to_fmtstring(block->op, args);
    sprintf(line, fmt, arg1, arg2, arg3);

    if (
        block->op == HIR_MKSCOPE ||
        block->op == HIR_FDCL    ||
        block->op == HIR_STRT
    ) _depth++;

    printf("%s", line);
}

/* https://dreampuf.github.io/GraphvizOnline/?engine=dot */
static void _dump_domtree_dot_rec(cfg_block_t* b) {
    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        printf("  B%i -> B%i;\n", b->id, c->id);
        _dump_domtree_dot_rec(c);
    }
}

void _dump_domtree_dot(cfg_func_t* func) {
    printf("digraph DomTree {\n");
    printf("  rankdir=TB;\n");

    list_iter_t bit;
    list_iter_hinit(&func->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        printf("  B%i [label=\"B%i\"];\n", cb->id, cb->id);
    }

    list_iter_hinit(&func->blocks, &bit);
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (!cb->sdom) {
            _dump_domtree_dot_rec(cb);
        }
    }

    printf("}\n");
}

void _dump_all_dom_dot(cfg_func_t* func) {
    printf("digraph AllDom {\n");
    printf("  rankdir=TB;\n");

    list_iter_t bit;
    list_iter_hinit(&func->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        printf("  B%i [label=\"B%i\"];\n", cb->id, cb->id);
    }

    list_iter_hinit(&func->blocks, &bit);
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        set_iter_t it;
        set_iter_init(&cb->dom, &it);
        cfg_block_t* d;
        while ((d = set_iter_next_addr(&it))) {
            if (d == cb) continue;
            printf("  B%i -> B%i;\n", d->id, cb->id);
        }
    }

    printf("}\n");
}

static void _print_set_int(FILE* out, set_t* s) {
    set_iter_t it;
    set_iter_init(s, &it);
    long v;
    int first = 1;
    fprintf(out, "{");
    while ((v = set_iter_next_int(&it)) != -1) {
        if (!first) fprintf(out, ",");
        fprintf(out, "%ld", v);
        first = 0;
    }
    fprintf(out, "}");
}

static int _export_dot_func(cfg_func_t* f) {
    printf("digraph CFG_func%d {\n", f->id);
    printf("  rankdir=TB;\n");

    list_iter_t bit;
    list_iter_hinit(&f->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        printf("  B%ld [label=\"B%ld:\\nentry=%s%i\\nexit=%s",
               cb->id, cb->id,
               cb->entry ? hir_op_to_string(cb->entry->op) : "NULL", 
               cb->entry && cb->entry->op == HIR_MKLB ? cb->entry->farg->id : -1,
               cb->exit  ? hir_op_to_string(cb->exit->op)  : "NULL");

        printf("\\nIN=");
        _print_set_int(stdout, &cb->curr_in);
        printf("\\nDEF=");
        _print_set_int(stdout, &cb->def);
        printf("\\nUSE=");
        _print_set_int(stdout, &cb->use);
        printf("\\nOUT=");
        _print_set_int(stdout, &cb->curr_out);

        printf("\"];\n");

        if (cb->l)   printf("  B%ld -> B%ld [label=\"fall\"];\n", cb->id, cb->l->id);
        if (cb->jmp) printf("  B%ld -> B%ld [label=\"jump\"];\n", cb->id, cb->jmp->id);
    }
    
    printf("}\n");
    return 1;
}

void cfg_print(cfg_ctx_t* ctx) {
    printf("==== CFG DUMP ====\n");

    list_iter_t fit;
    list_iter_hinit(&ctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        printf("==== CFG DOT ====\n");
        _export_dot_func(fb);
        printf("==== DOM DOT ====\n");
        _dump_all_dom_dot(fb);
        printf("==== SDOM DOT ====\n");
        _dump_domtree_dot(fb);
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

    sym_table_t smt;
    syntax_ctx_t sctx = { .r = NULL };

    STX_create(tkn, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_build_cfg(&irctx, &cfgctx);
    
    ssa_ctx_t ssactx = { .h = NULL };
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    HIR_DFG_collect_defs(&cfgctx);
    HIR_DFG_collect_uses(&cfgctx);
    HIR_DFG_compute_inout(&cfgctx);
    HIR_DFG_make_allias(&cfgctx, &smt);

    cfg_print(&cfgctx);

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    list_iter_t it;

    if (!list_isempty(&smt.v.lst)) printf("==========   VARS  ==========\n");
    list_iter_hinit(&smt.v.lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vi->v_id, vi->name, vi->type, vi->s_id);
    }

    if (!list_isempty(&smt.a.lst)) printf("==========   ARRS  ==========\n");
    list_iter_hinit(&smt.a.lst, &it);
    array_info_t* ai;
    while ((ai = (array_info_t*)list_iter_next(&it))) {
        printf("id: %i, name: %s, scope: %i\n", ai->v_id, ai->name, ai->s_id);
    }

    if (!list_isempty(&smt.f.lst)) printf("==========  FUNCS  ==========\n");
    list_iter_hinit(&smt.f.lst, &it);
    func_info_t* fi;
    while ((fi = (func_info_t*)list_iter_next(&it))) {
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (!list_isempty(&smt.s.lst)) printf("========== STRINGS ==========\n");
    list_iter_hinit(&smt.s.lst, &it);
    str_info_t* si;
    while ((si = (str_info_t*)list_iter_next(&it))) {
        printf("id: %i, val: %s\n", si->id, si->value);
    }

    if (!list_isempty(&smt.m.lst)) printf("========== ALLIAS ==========\n");
    list_iter_hinit(&smt.m.lst, &it);
    allias_t* mi;
    while ((mi = (allias_t*)list_iter_next(&it))) {
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while ((own_id = set_iter_next_int(&sit)) >= 0) printf("%i ", own_id);
    }

    HIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
