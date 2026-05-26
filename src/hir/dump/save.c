#include <hir/dump.h>

static const char* _get_operation_template(hir_operation_t op) {
    switch(op) {
        case HIR_STARGLD:      return "%s = load_starg();\n";
        case HIR_FARGLD:       return "%s = load_arg();\n";
        case HIR_TF64:         return "%s = %s as f64;\n";
        case HIR_TF32:         return "%s = %s as f32;\n";
        case HIR_TI64:         return "%s = %s as i64;\n";
        case HIR_TI32:         return "%s = %s as i32;\n";
        case HIR_TI16:         return "%s = %s as i16;\n";
        case HIR_TI8:          return "%s = %s as i8;\n";
        case HIR_TU64:         return "%s = %s as u64;\n";
        case HIR_TU32:         return "%s = %s as u32;\n";
        case HIR_TU16:         return "%s = %s as u16;\n";
        case HIR_TU8:          return "%s = %s as u8;\n";
        case HIR_TPTR:         return "%s = %s as *ptr;\n";
        case HIR_BREAKPOINT:   return "breakpoint(%s);\n";
        case HIR_UFCLL:
        case HIR_FCLL:         return "%s%s(%s);\n";
        case HIR_STORE_UFCLL:
        case HIR_STORE_FCLL:   return "%s = %s(%s);\n";
        case HIR_ECLL:         return "%s(%s%s);\n";
        case HIR_STORE_ECLL:   return "%s = %s(%s);\n";
        case HIR_SYSC:         return "syscall(%s%s%s);\n";
        case HIR_STORE_SYSC:   return "%s = syscall(%s%s);\n";
        case HIR_FRET:         return "return %s;\n";
        case HIR_MKLB:         return "%s:\n";
        case HIR_STRT:
        case HIR_FDCL:         return "fn %s\n";
        case HIR_STEND:
        case HIR_FEND:         return "\n";
        case HIR_FEXT:         return "(fun) extern(%s);\n";
        case HIR_OEXT:         return "(var) extern(%s);\n";
        case HIR_BREAK:        return "// break;\n";
        case HIR_JMP:          return "goto %s;\n";
        case HIR_iADD:         return "%s = %s + %s;\n";
        case HIR_iSUB:         return "%s = %s - %s;\n";
        case HIR_iMUL:         return "%s = %s * %s;\n";
        case HIR_iDIV:         return "%s = %s / %s;\n";
        case HIR_iMOD:         return "%s = %s %% %s;\n";
        case HIR_iLRG:         return "%s = %s > %s;\n";
        case HIR_iLGE:         return "%s = %s >= %s;\n";
        case HIR_iLWR:         return "%s = %s < %s;\n";
        case HIR_iLRE:         return "%s = %s <= %s;\n";
        case HIR_iCMP:         return "%s = %s == %s;\n";
        case HIR_iNMP:         return "%s = %s != %s;\n";
        case HIR_iAND:         return "%s = %s && %s;\n";
        case HIR_iOR:          return "%s = %s || %s;\n";
        case HIR_iBLFT:        return "%s = %s << %s;\n";
        case HIR_iBRHT:        return "%s = %s >> %s;\n";
        case HIR_bAND:         return "%s = %s & %s;\n";
        case HIR_bOR:          return "%s = %s | %s;\n";
        case HIR_bXOR:         return "%s = %s ^ %s;\n";
        case HIR_RAW:          return "[raw, \"%s\"]\n";
        case HIR_IFOP2:        return "if %s, goto %s, else goto %s;\n";
        case HIR_NEG:          return "%s = neg %s;\n";
        case HIR_NOT:          return "%s = not %s;\n";
        case HIR_STORE:        return "%s = %s;\n";
        case HIR_VRUSE:        return "use %s;\n";
        case HIR_REF_ARGS:     return "%s = &(*)\n";
        case HIR_ARRDECL:      return "%s = arr_alloc(%s);\n";
        case HIR_STRDECL:      return "%s = str_alloc(%s);\n";
        case HIR_VARDECL:      return "%s = alloc;\n";
        case HIR_STASM:        return "asm(%s%s%s) {\n";
        case HIR_ENDASM:       return "}\n";
        case HIR_GDREF:        return "%s = *(%s);\n";
        case HIR_LDREF:        return "*(%s) = %s;\n";
        case HIR_REF:          return "%s = &(%s);\n";
        case HIR_EXITOP:       return "exit %s;\n";
        case HIR_PHI:          return "[base: %s] %s = phi(%s);\n";
        case HIR_MKSCOPE:      return "{\n";
        case HIR_ENDSCOPE:     return "}\n";
        case HIR_PHI_PREAMBLE: return "[SSA] future: %s <<== previous: %s;\n";
        default:               return "\n";
    }
}

static char* _get_formatted_subject(char* dst, hir_subject_t* s, sym_table_t* smt, int style) {
    if (!s) return dst;
    if (HIR_is_vartype(s->t)) {
        switch (s->t) {
            case HIR_STKVARARR:  dst += sprintf(dst, "arrs"); break;
            case HIR_STKVARF64:  dst += sprintf(dst, "f64s"); break;
            case HIR_STKVARU64:  dst += sprintf(dst, "u64s"); break;
            case HIR_STKVARI64:  dst += sprintf(dst, "i64s"); break;
            case HIR_STKVARF32:  dst += sprintf(dst, "f32s"); break;
            case HIR_STKVARU32:  dst += sprintf(dst, "u32s"); break;
            case HIR_STKVARI32:  dst += sprintf(dst, "i32s"); break;
            case HIR_STKVARU16:  dst += sprintf(dst, "u16s"); break;
            case HIR_STKVARI16:  dst += sprintf(dst, "i16s"); break;
            case HIR_STKVARU8:   dst += sprintf(dst, "u8s");  break;
            case HIR_STKVARI8:   dst += sprintf(dst, "i8s");  break;
            case HIR_STKVARI0:   dst += sprintf(dst, "i0s");  break;
            case HIR_TMPVARARR:  dst += sprintf(dst, "arrt"); break;
            case HIR_TMPVARF64:  dst += sprintf(dst, "f64t"); break;
            case HIR_TMPVARU64:  dst += sprintf(dst, "u64t"); break;
            case HIR_TMPVARI64:  dst += sprintf(dst, "i64t"); break;
            case HIR_TMPVARF32:  dst += sprintf(dst, "f32t"); break;
            case HIR_TMPVARU32:  dst += sprintf(dst, "u32t"); break;
            case HIR_TMPVARI32:  dst += sprintf(dst, "i32t"); break;
            case HIR_TMPVARU16:  dst += sprintf(dst, "u16t"); break;
            case HIR_TMPVARI16:  dst += sprintf(dst, "i16t"); break;
            case HIR_TMPVARU8:   dst += sprintf(dst, "u8t");  break;
            case HIR_TMPVARI8:   dst += sprintf(dst, "i8t");  break;
            case HIR_TMPVARI0:   dst += sprintf(dst, "i0t");  break;
            case HIR_GLBVARARR:  dst += sprintf(dst, "arrg"); break;
            case HIR_GLBVARF64:  dst += sprintf(dst, "f64g"); break;
            case HIR_GLBVARU64:  dst += sprintf(dst, "u64g"); break;
            case HIR_GLBVARI64:  dst += sprintf(dst, "i64g"); break;
            case HIR_GLBVARF32:  dst += sprintf(dst, "f32g"); break;  
            case HIR_GLBVARU32:  dst += sprintf(dst, "u32g"); break;
            case HIR_GLBVARI32:  dst += sprintf(dst, "i32g"); break;
            case HIR_GLBVARU16:  dst += sprintf(dst, "u16g"); break;
            case HIR_GLBVARI16:  dst += sprintf(dst, "i16g"); break;
            case HIR_GLBVARU8:   dst += sprintf(dst, "u8g");  break;
            case HIR_GLBVARI8:   dst += sprintf(dst, "i8g");  break;
            case HIR_GLBVARI0:   dst += sprintf(dst, "i0g");  break;
            default: break;
        }
        
        for (int i = 0; i < s->ptr; i++) dst += sprintf(dst, "*");
        variable_info_t vi;
        if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) {
            dst += sprintf(dst, " %%%li", vi.v_id);
        }
    }
    else {
        switch (s->t) {
            case HIR_F64NUMBER:   dst += sprintf(dst, "f64n %s", s->storage.num.value->body); break;
            case HIR_I64NUMBER:   dst += sprintf(dst, "i64n %s", s->storage.num.value->body); break;
            case HIR_U64NUMBER:   dst += sprintf(dst, "u64n %s", s->storage.num.value->body); break;
            case HIR_F32NUMBER:   dst += sprintf(dst, "f32n %s", s->storage.num.value->body); break;
            case HIR_I32NUMBER:   dst += sprintf(dst, "i32n %s", s->storage.num.value->body); break;
            case HIR_U32NUMBER:   dst += sprintf(dst, "u32n %s", s->storage.num.value->body); break;
            case HIR_I16NUMBER:   dst += sprintf(dst, "i16n %s", s->storage.num.value->body); break;
            case HIR_U16NUMBER:   dst += sprintf(dst, "u16n %s", s->storage.num.value->body); break;  
            case HIR_I8NUMBER:    dst += sprintf(dst, "i8n %s", s->storage.num.value->body);  break;
            case HIR_U8NUMBER:    dst += sprintf(dst, "u8n %s", s->storage.num.value->body);  break;
            case HIR_NUMBER:      dst += sprintf(dst, "num? %s", s->storage.num.value->body); break;
            case HIR_U8CONSTVAL:  dst += sprintf(dst, "csi8 %ld", s->storage.cnst.value);     break;
            case HIR_I8CONSTVAL:  dst += sprintf(dst, "csu8 %ld", s->storage.cnst.value);     break;
            case HIR_U16CONSTVAL: dst += sprintf(dst, "csu16 %ld", s->storage.cnst.value);    break;
            case HIR_I16CONSTVAL: dst += sprintf(dst, "csi16 %ld", s->storage.cnst.value);    break;
            case HIR_U32CONSTVAL: dst += sprintf(dst, "csu32 %ld", s->storage.cnst.value);    break;
            case HIR_I32CONSTVAL: dst += sprintf(dst, "csi32 %ld", s->storage.cnst.value);    break;
            case HIR_U64CONSTVAL: dst += sprintf(dst, "csu64 %ld", s->storage.cnst.value);    break;
            case HIR_I64CONSTVAL: dst += sprintf(dst, "csi64 %ld", s->storage.cnst.value);    break;

            case HIR_LABEL: dst += sprintf(dst, "lb%li", s->id); break;
            case HIR_RAWASM:
            case HIR_STRING: {
                str_info_t si;
                if (STTB_get_info_id(s->storage.str.s_id, &si, &smt->s)) {
                    dst += sprintf(dst, "%s", si.value->body);
                }

                break;
            }
            case HIR_FNAME: {
                func_info_t fi;
                if (FNTB_get_info_id(s->storage.str.s_id, &fi, &smt->f)) {
                    if (style != 1) dst += sprintf(dst, "%s", fi.virt->body);
                    else {
                        dst += sprintf(dst, "%s(", fi.virt->body);
                        if (fi.args) {
                            for (ast_node_t* t = fi.args->c; t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
                                if (t->t->t_type == VAR_ARGUMENTS_TOKEN) dst += sprintf(dst, "...");
                                else dst += sprintf(dst, "%s %s", DUMP_format_token(t->t), t->c->t->body->body);
                                if (t->siblings.n && t->siblings.n->t->t_type != SCOPE_TOKEN) {
                                    dst += sprintf(dst, ", ");
                                }
                            }
                        }

                        dst += sprintf(dst, ")");
                        if (fi.rtype) {
                            dst += sprintf(dst, " -> %s", DUMP_format_token(fi.rtype->t));
                        }
                    }
                }

                break;
            }
            case HIR_PHISET: {
                dst += sprintf(dst, "set ");
                set_foreach (int_tuple_t* tpl, &s->storage.set.h) {
                    variable_info_t pvi;
                    if (VRTB_get_info_id(tpl->y, &pvi, &smt->v)) {
                        dst += sprintf(dst, "[%%%li, bb%li]", pvi.v_id, tpl->x);
                    }
                }
                
                break;
            }
            case HIR_ARGLIST: {
                list_iter_t it;
                list_iter_hinit(&s->storage.list.h, &it);
                hir_subject_t* s;
                while (list_iter_next(&it, (void**)&s)) {
                    dst = _get_formatted_subject(dst, s, smt, style);
                    if (list_iter_current(&it)) {
                        dst += sprintf(dst, ", ");
                    }
                }

                break;
            }
            default: dst += sprintf(dst, "nop(%i)", s->t); break;
        }
    }

    return dst;
}

static int _get_formatted_block(char* dst, hir_block_t* block, sym_table_t* smt, int pos, int unused, int depth) {
    if (!block) return depth;
    if (pos && block->op == HIR_SETPOS) {
        dst += sprintf(
            dst,
            "setpos, line=%li, column=%li, file=%s\n", 
            block->farg->storage.pos.line, 
            block->farg->storage.pos.column, 
            block->farg->storage.pos.file ? block->farg->storage.pos.file->body : "<unknown>"
        );
        return depth;
    }

    int style = HIR_is_funccall(block->op) ? 2 : 1;
    char arg1[256] = { 0 };
    if (block->farg) _get_formatted_subject(arg1, block->farg, smt, style);
    char arg2[256] = { 0 };
    if (block->sarg) _get_formatted_subject(arg2, block->sarg, smt, style);
    char arg3[256] = { 0 };
    if (block->targ) _get_formatted_subject(arg3, block->targ, smt, style);
    
    if (block->op == HIR_ENDSCOPE) depth--;

    for (int i = 0; i < depth; i++) dst += sprintf(dst, "    ");
    if (!unused && block->unused) goto _force_end;
    else if (unused && block->unused) dst += sprintf(dst, "[unused] ");
    const char* fmt = _get_operation_template(block->op);
    sprintf(dst, fmt, arg1, arg2, arg3);
    
_force_end: {}
    if (block->op == HIR_MKSCOPE) depth++;
    return depth;
}

int DUMP_format_hirctx(hir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output) {
    int curr_tab = 0;
    hir_block_t* hh = ctx->hot.h;
    while (hh) {
        char line[512] = { 0 };
        curr_tab = _get_formatted_block(line, hh, smt, pos, unused, curr_tab);
        fprintf(output, "%s", line);
        hh = hh->next;
    }

    return 1;
}
