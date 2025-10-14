#ifndef HIR_HELPER_H_
#define HIR_HELPER_H_

#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/hir_types.h>

static const char* hir_op_to_string(hir_operation_t op) {
    switch(op) {
        case HIR_PHI:          return "PHI";
        case HIR_VRDEALL:      return "VRDEALL";
        case HIR_STARGLD:      return "STARGLD";
        case HIR_FARGST:       return "FARGST";
        case HIR_FARGLD:       return "FARGLD";
        case HIR_FCLL:         return "FCLL";
        case HIR_STORE_FCLL:   return "HIR_STORE_FCLL";
        case HIR_ECLL:         return "ECLL";
        case HIR_STORE_ECLL:   return "HIR_STORE_ECLL";
        case HIR_STRT:         return "STRT";
        case HIR_SYSC:         return "SYSC";
        case HIR_STORE_SYSC:   return "HIR_STORE_SYSC";
        case HIR_FRET:         return "FRET";
        case HIR_MKLB:         return "MKLB";
        case HIR_FDCL:         return "FDCL";
        case HIR_FEND:         return "FEND";
        case HIR_OEXT:         return "OEXT";
        case HIR_JMP:          return "JMP";
        case HIR_iADD:         return "iADD";
        case HIR_iSUB:         return "iSUB";
        case HIR_iMUL:         return "iMUL";
        case HIR_iDIV:         return "iDIV";
        case HIR_iMOD:         return "iMOD";
        case HIR_iLRG:         return "iLRG";
        case HIR_iLGE:         return "iLGE";
        case HIR_iLWR:         return "iLWR";
        case HIR_iLRE:         return "iLRE";
        case HIR_iCMP:         return "iCMP";
        case HIR_iNMP:         return "iNMP";
        case HIR_iAND:         return "iAND";
        case HIR_iOR:          return "iOR";
        case HIR_iBLFT:        return "iBLFT";
        case HIR_iBRHT:        return "iBRHT";
        case HIR_bAND:         return "bAND";
        case HIR_bOR:          return "bOR";
        case HIR_bXOR:         return "bXOR";
        case HIR_RAW:          return "RAW";
        case HIR_IFOP:         return "IFOP";
        case HIR_NOT:          return "NOT";
        case HIR_STORE:        return "STORE";
        case HIR_VARDECL:      return "VARDECL";
        case HIR_ARRDECL:      return "ARRDECL";
        case HIR_VRUSE:        return "VRUSE";
        case HIR_STRDECL:      return "STRDECL";
        case HIR_PRMST:        return "PRMST";
        case HIR_PRMLD:        return "PRMLD";
        case HIR_PRMPOP:       return "PRMPOP";
        case HIR_STASM:        return "STASM";
        case HIR_ENDASM:       return "ENDASM";
        case HIR_GINDEX:       return "GINDEX";
        case HIR_LINDEX:       return "LINDEX";
        case HIR_GDREF:        return "GDREF";
        case HIR_LDREF:        return "LDREF";
        case HIR_REF:          return "REF";
        case HIR_EXITOP:       return "EXITOP";
        case HIR_CLNVRS:       return "HIR_CLNVRS";
        case HIR_IFLWOP:       return "HIR_IFLWOP";
        case HIR_IFLGOP:       return "HIR_IFLGOP";
        case HIR_IFNCPOP:      return "HIR_IFNCPOP";
        case HIR_IFCPOP:       return "HIR_IFCPOP";
        case HIR_STEND:        return "HIR_STEND";
        case HIR_MKSCOPE:      return "MKSCOPE";
        case HIR_ENDSCOPE:     return "ENDSCOPE";
        case HIR_TF64:         return "HIR_TF64";
        case HIR_TF32:         return "HIR_TF32"; // x = (f32)y
        case HIR_TI64:         return "HIR_TI64"; // x = (i64)y
        case HIR_TI32:         return "HIR_TI32"; // x = (i32)y
        case HIR_TI16:         return "HIR_TI16"; // x = (i16)y
        case HIR_TI8:          return "HIR_TI8";  // x = (i8)y
        case HIR_TU64:         return "HIR_TU64"; // x = (u64)y
        case HIR_TU32:         return "HIR_TU32"; // x = (u32)y
        case HIR_TU16:         return "HIR_TU16"; // x = (u16)y
        case HIR_TU8:          return "HIR_TU8";  // x = (u8)y
        case HIR_PHI_PREAMBLE: return "PHI_PREAMBLE";
        default: return "";
    }
}

static void print_hir_subject(const hir_subject_t* s) {
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
        case HIR_NUMBER:     printf("n%s", s->storage.num.value);             break;
        case HIR_CONSTVAL:   printf("c%ld", s->storage.cnst.value);           break;
        case HIR_LABEL:      printf("lb: [id=%d]", s->id);                    break;
        case HIR_RAWASM:     printf("asm: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_STRING:     printf("str: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_FNAME:      printf("func: [fid=%i]", s->storage.str.s_id);   break;
        default: printf("unknw"); break;
    }
}

static int _depth = 0;
static const char* hir_op_to_fmtstring(hir_operation_t op, int state) {
    switch(op) {
        case HIR_STARGLD:    return "%s = load_starg();\n";
        case HIR_FARGST:     return "store_arg(%s);\n";
        case HIR_FARGLD:     return "%s = load_arg();\n";
        
        case HIR_TF64:       return "%s = %s as f64;\n";
        case HIR_TF32:       return "%s = %s as f32;\n";
        case HIR_TI64:       return "%s = %s as i64;\n";
        case HIR_TI32:       return "%s = %s as i32;\n";
        case HIR_TI16:       return "%s = %s as i16;\n";
        case HIR_TI8:        return "%s = %s as i8;\n";
        case HIR_TU64:       return "%s = %s as u64;\n";
        case HIR_TU32:       return "%s = %s as u32;\n";
        case HIR_TU16:       return "%s = %s as u16;\n";
        case HIR_TU8:        return "%s = %s as u8;\n";

        case HIR_FCLL:       return "call %s, argc %s%s;\n";
        case HIR_STORE_FCLL: return "%s = call %s, argc %s;\n";
        case HIR_ECLL:       return "excall %s, argc %s%s;\n";
        case HIR_STORE_ECLL: return "%s = excall %s, argc %s;\n";
        case HIR_SYSC:       return "syscall, argc: %s%s%s;\n";
        case HIR_STORE_SYSC: return "%s = syscall, argc: %s%s;\n";

        case HIR_STRT:       return "start {\n";
        case HIR_FRET:       return "return %s;\n";
        case HIR_MKLB:       return "%s:\n";
        case HIR_FDCL:       return "fn %s\n";
        case HIR_FEND:       return "\n";
        case HIR_OEXT:       return "extern %s;\n";
        case HIR_JMP:        return "goto %s;\n";
        case HIR_iADD:       return "%s = %s + %s;\n";
        case HIR_iSUB:       return "%s = %s - %s;\n";
        case HIR_iMUL:       return "%s = %s * %s;\n";
        case HIR_iDIV:       return "%s = %s / %s;\n";
        case HIR_iMOD:       return "%s = %s mod %s;\n";
        case HIR_iLRG:       return "%s = %s > %s;\n";
        case HIR_iLGE:       return "%s = %s >= %s;\n";
        case HIR_iLWR:       return "%s = %s < %s;\n";
        case HIR_iLRE:       return "%s = %s <= %s;\n";
        case HIR_iCMP:       return "%s = %s == %s;\n";
        case HIR_iNMP:       return "%s = %s != %s;\n";
        case HIR_iAND:       return "%s = %s && %s;\n";
        case HIR_iOR:        return "%s = %s || %s;\n";
        case HIR_iBLFT:      return "%s = %s << %s;\n";
        case HIR_iBRHT:      return "%s = %s >> %s;\n";
        case HIR_bAND:       return "%s = %s & %s;\n";
        case HIR_bOR:        return "%s = %s | %s;\n";
        case HIR_bXOR:       return "%s = %s ^ %s;\n";
        case HIR_RAW:        return "[raw, \"%s\"]\n";
        case HIR_IFOP:       return "if %s, goto %s;\n";
        case HIR_NOT:        return "%s = not %s;\n";
        case HIR_STORE:      return "%s = %s;\n";
        case HIR_VRUSE:      return "use %s;\n";
        case HIR_ARRDECL:    return "%s = alloc(%s * 8);\n";
        case HIR_STRDECL:    return "%s = alloc(%s);\n";
        case HIR_VRDEALL:    return "kill %s\n";

        case HIR_VARDECL: {
            switch (state) {
                case 2:  return "%s = alloc(8);\n";
                default: return "\n";
            }
        }

        case HIR_PRMST:     return "prm_st(%s);\n";
        case HIR_PRMLD:     return "prm_ld();\n";
        case HIR_PRMPOP:    return "prm_pop();\n";
        case HIR_STASM:     return "asm, %s%s%s {\n";
        case HIR_ENDASM:    return "}\n";
        case HIR_GINDEX:    return "%s = %s[%s];\n";
        case HIR_LINDEX:    return "%s[%s] = %s;\n";
        case HIR_GDREF:     return "%s = *(%s);\n";
        case HIR_LDREF:     return "*(%s) = %s;\n";
        case HIR_REF:       return "%s = &(%s);\n";
        case HIR_IFLWOP:    return "if %s < %s, goto %s;\n";
        case HIR_IFLWEOP:   return "if %s <= %s, goto %s;\n";
        case HIR_IFLGOP:    return "if %s > %s, goto %s;\n";
        case HIR_IFLGEOP:   return "if %s >= %s, goto %s;\n";
        case HIR_IFNCPOP:   return "if %s != %s, goto %s;\n";
        case HIR_IFCPOP:    return "if %s == %s, goto %s;\n";
        case HIR_EXITOP:    return "exit %s;\n";
        case HIR_STEND:     return "}\n";
        case HIR_PHI:       return "[%s] %s = phi(%s);\n";
        case HIR_MKSCOPE:   return "{\n";
        case HIR_ENDSCOPE:  return "}\n";
        case HIR_PHI_PREAMBLE: return "future: %s = previous: %s;\n";
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
            case HIR_NUMBER:   dst += sprintf(dst, "num: %s", s->storage.num.value);     break;
            case HIR_CONSTVAL: dst += sprintf(dst, "const: %ld", s->storage.cnst.value); break;
            case HIR_LABEL:    dst += sprintf(dst, "lb%d", s->id);                       break;

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
                dst += sprintf(dst, "set: ");

                set_iter_t it;
                set_iter_init(&s->storage.set.h, &it);
                int_tuple_t* tpl;
                while (set_iter_next(&it, (void**)&tpl)) {
                    variable_info_t pvi;
                    if (VRTB_get_info_id(tpl->y, &pvi, &smt->v)) {
                        dst += sprintf(dst, "[%s%i, bb%d]", pvi.name, pvi.v_id, tpl->x);
                    }
                }
                
                break;
            }

            case HIR_LIST: {
                dst += sprintf(dst, "list: ");

                list_iter_t it;
                list_iter_hinit(&s->storage.list.h, &it);
                hir_subject_t* s;
                while ((s = list_iter_next(&it))) {
                    dst = sprintf_hir_subject(dst, s, smt);
                    dst += sprintf(dst, " ");
                }
                
                break;
            }

            default: dst += sprintf(dst, "unknw"); break;
        }
    }

    // dst += sprintf(dst, " addr=%p", s);
    return dst;
}

void print_hir_block(const hir_block_t* block, int ud, sym_table_t* smt) {
    if (!block || block->unused) return;
    // if (!block) return;
    // print_debug("%i", block->unused);

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
        // block->op == HIR_FEND     ||
        block->op == HIR_STEND
    ) _depth--;

    if (ud) for (int i = 0; i < _depth; i++) printf("    ");
    const char* fmt = hir_op_to_fmtstring(block->op, args);
    sprintf(line, fmt, arg1, arg2, arg3);

    if (
        block->op == HIR_MKSCOPE ||
        // block->op == HIR_FDCL    ||
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
        while (set_iter_next(&it, (void**)&d)) {
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
    while (set_iter_next(&it, (void**)&v)) {
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
#endif