#include <hir/dump.h>

#define HIR_DUMP_SUBJECT_BUFFER 4096
#define HIR_DUMP_LINE_BUFFER    (HIR_DUMP_SUBJECT_BUFFER * 3 + 256)

static char* _dump_appendf(char* dst, char* end, const char* fmt, ...) {
    if (!dst || !end || dst >= end) return dst;

    int available = end - dst;
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(dst, available, fmt, args);
    va_end(args);

    if (written < 0) return dst;
    if (written >= available) return end - 1;
    return dst + written;
}

static char* _dump_append_escaped_string(char* dst, char* end, const char* value) {
    if (!value) return dst;
    for (const unsigned char* c = (const unsigned char*)value; *c; c++) {
        switch (*c) {
            case '\\': dst = _dump_appendf(dst, end, "\\\\"); break;
            case '"':  dst = _dump_appendf(dst, end, "\\\""); break;
            case '\n': dst = _dump_appendf(dst, end, "\\n");  break;
            case '\r': dst = _dump_appendf(dst, end, "\\r");  break;
            case '\t': dst = _dump_appendf(dst, end, "\\t");  break;
            default:   dst = _dump_appendf(dst, end, "%c", *c); break;
        }
    }

    return dst;
}

static inline int _dump_always_show_scope(hir_operation_t op) {
    switch (op) {
        case HIR_MKSCOPE: case HIR_ENDSCOPE: case HIR_STASM: case HIR_ENDASM: return 1;
        default:                                                              return 0;
    }
}

/* Get a printf-style template for a HIR operation dump line.
Params:
    - `op` - HIR operation to format.

Returns a format string for the operation. */
static const char* _get_operation_template(hir_operation_t op) {
    switch(op) {
        case HIR_STARGLD:      return "%s = load_starg();";
        case HIR_FARGLD:       return "%s = load_arg();";
        case HIR_TF64:         return "%s = %s as f64;";
        case HIR_TF32:         return "%s = %s as f32;";
        case HIR_TI64:         return "%s = %s as i64;";
        case HIR_TI32:         return "%s = %s as i32;";
        case HIR_TI16:         return "%s = %s as i16;";
        case HIR_TI8:          return "%s = %s as i8;";
        case HIR_TU64:         return "%s = %s as u64;";
        case HIR_TU32:         return "%s = %s as u32;";
        case HIR_TU16:         return "%s = %s as u16;";
        case HIR_TU8:          return "%s = %s as u8;";
        case HIR_TPTR:         return "%s = %s as *ptr;";
        case HIR_BREAKPOINT:   return "breakpoint(%s);";
        case HIR_UFCLL:
        case HIR_FCLL:         return "%s%s(%s);";
        case HIR_STORE_UFCLL:
        case HIR_STORE_FCLL:   return "%s = %s(%s);";
        case HIR_ECLL:         return "%s%s(%s);";
        case HIR_STORE_ECLL:   return "%s = %s(%s);";
        case HIR_SYSC:         return "syscall(%s%s%s);";
        case HIR_STORE_SYSC:   return "%s = syscall(%s%s);";
        case HIR_FRET:         return "return %s;";
        case HIR_MKLB:         return "%s:";
        case HIR_STRT:
        case HIR_FDCL:         return "fn %s";
        case HIR_FEXT:         return "(fun) extern(%s);";
        case HIR_OEXT:         return "(var) extern(%s);";
        case HIR_BREAK:        return "// break;";
        case HIR_JMP:          return "goto %s;";
        case HIR_iADD:         return "%s = %s + %s;";
        case HIR_iSUB:         return "%s = %s - %s;";
        case HIR_iMUL:         return "%s = %s * %s;";
        case HIR_iDIV:         return "%s = %s / %s;";
        case HIR_iMOD:         return "%s = %s %% %s;";
        case HIR_iLRG:         return "%s = %s > %s;";
        case HIR_iLGE:         return "%s = %s >= %s;";
        case HIR_iLWR:         return "%s = %s < %s;";
        case HIR_iLRE:         return "%s = %s <= %s;";
        case HIR_iCMP:         return "%s = %s == %s;";
        case HIR_iNMP:         return "%s = %s != %s;";
        case HIR_iAND:         return "%s = %s && %s;";
        case HIR_iOR:          return "%s = %s || %s;";
        case HIR_iBLFT:        return "%s = %s << %s;";
        case HIR_iBRHT:        return "%s = %s >> %s;";
        case HIR_bAND:         return "%s = %s & %s;";
        case HIR_bOR:          return "%s = %s | %s;";
        case HIR_bXOR:         return "%s = %s ^ %s;";
        case HIR_RAW:          return "[raw, \"%s\"]";
        case HIR_IFOP2:        return "if %s, goto %s, else goto %s;";
        case HIR_NEG:          return "%s = neg %s;";
        case HIR_NOT:          return "%s = not %s;";
        case HIR_STORE:        return "%s = %s;";
        case HIR_VRUSE:        return "use %s;";
        case HIR_REF_ARGS:     return "%s = &(*)";
        case HIR_ARRDECL:      return "%s = arr_alloc(%s);";
        case HIR_STRDECL:      return "%s = str_alloc(%s);";
        case HIR_VARDECL:      return "%s = alloc;";
        case HIR_STASM:        return "asm(%s%s%s) {";
        case HIR_ENDASM:       return "}";
        case HIR_GDREF:        return "%s = *(%s);";
        case HIR_LDREF:        return "*(%s) = %s;";
        case HIR_REF:          return "%s = &(%s);";
        case HIR_EXITOP:       return "exit %s;";
        case HIR_PHI:          return "[base: %s] %s = phi(%s);";
        case HIR_MKSCOPE:      return "{";
        case HIR_ENDSCOPE:     return "}";
        case HIR_PHI_PREAMBLE: return "[SSA] future: %s <<== previous: %s;";
        default:               return "";
    }
}

/* Format a HIR subject into the dump buffer.
Params:
    - `dst` - Output buffer cursor.
    - `end` - One-past-end output buffer pointer.
    - `s` - Subject to format.
    - `smt` - Symtable used to resolve symbol names.
    - `style` - Formatting style for variable names.

Returns the advanced output buffer cursor. */
static char* _get_formatted_subject(char* dst, char* end, hir_subject_t* s, sym_table_t* smt, int style) {
    if (!s) return dst;
#define APPEND(...) dst = _dump_appendf(dst, end, __VA_ARGS__)
    if (HIR_is_vartype(s->t)) {
        switch (s->t) {
            case HIR_STKVARARR:  APPEND("arrs"); break;
            case HIR_STKVARF64:  APPEND("f64s"); break;
            case HIR_STKVARU64:  APPEND("u64s"); break;
            case HIR_STKVARI64:  APPEND("i64s"); break;
            case HIR_STKVARF32:  APPEND("f32s"); break;
            case HIR_STKVARU32:  APPEND("u32s"); break;
            case HIR_STKVARI32:  APPEND("i32s"); break;
            case HIR_STKVARU16:  APPEND("u16s"); break;
            case HIR_STKVARI16:  APPEND("i16s"); break;
            case HIR_STKVARU8:   APPEND("u8s");  break;
            case HIR_STKVARI8:   APPEND("i8s");  break;
            case HIR_STKVARI0:   APPEND("i0s");  break;
            case HIR_TMPVARARR:  APPEND("arrt"); break;
            case HIR_TMPVARF64:  APPEND("f64t"); break;
            case HIR_TMPVARU64:  APPEND("u64t"); break;
            case HIR_TMPVARI64:  APPEND("i64t"); break;
            case HIR_TMPVARF32:  APPEND("f32t"); break;
            case HIR_TMPVARU32:  APPEND("u32t"); break;
            case HIR_TMPVARI32:  APPEND("i32t"); break;
            case HIR_TMPVARU16:  APPEND("u16t"); break;
            case HIR_TMPVARI16:  APPEND("i16t"); break;
            case HIR_TMPVARU8:   APPEND("u8t");  break;
            case HIR_TMPVARI8:   APPEND("i8t");  break;
            case HIR_TMPVARI0:   APPEND("i0t");  break;
            case HIR_GLBVARARR:  APPEND("arrg"); break;
            case HIR_GLBVARF64:  APPEND("f64g"); break;
            case HIR_GLBVARU64:  APPEND("u64g"); break;
            case HIR_GLBVARI64:  APPEND("i64g"); break;
            case HIR_GLBVARF32:  APPEND("f32g"); break;
            case HIR_GLBVARU32:  APPEND("u32g"); break;
            case HIR_GLBVARI32:  APPEND("i32g"); break;
            case HIR_GLBVARU16:  APPEND("u16g"); break;
            case HIR_GLBVARI16:  APPEND("i16g"); break;
            case HIR_GLBVARU8:   APPEND("u8g");  break;
            case HIR_GLBVARI8:   APPEND("i8g");  break;
            case HIR_GLBVARI0:   APPEND("i0g");  break;
            default: break;
        }
        
        for (int i = 0; i < s->ptr; i++) APPEND("*");
        variable_info_t vi;
        if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) {
            APPEND(" %%%li", vi.v_id);
        }
    }
    else {
        switch (s->t) {
            case HIR_F64NUMBER:   APPEND("f64n %s", s->storage.num.value->body); break;
            case HIR_I64NUMBER:   APPEND("i64n %s", s->storage.num.value->body); break;
            case HIR_U64NUMBER:   APPEND("u64n %s", s->storage.num.value->body); break;
            case HIR_F32NUMBER:   APPEND("f32n %s", s->storage.num.value->body); break;
            case HIR_I32NUMBER:   APPEND("i32n %s", s->storage.num.value->body); break;
            case HIR_U32NUMBER:   APPEND("u32n %s", s->storage.num.value->body); break;
            case HIR_I16NUMBER:   APPEND("i16n %s", s->storage.num.value->body); break;
            case HIR_U16NUMBER:   APPEND("u16n %s", s->storage.num.value->body); break;
            case HIR_I8NUMBER:    APPEND("i8n %s", s->storage.num.value->body);  break;
            case HIR_U8NUMBER:    APPEND("u8n %s", s->storage.num.value->body);  break;
            case HIR_NUMBER:      APPEND("num? %s", s->storage.num.value->body); break;
            case HIR_U8CONSTVAL:  APPEND("csi8 %ld", s->storage.cnst.value);     break;
            case HIR_I8CONSTVAL:  APPEND("csu8 %ld", s->storage.cnst.value);     break;
            case HIR_U16CONSTVAL: APPEND("csu16 %ld", s->storage.cnst.value);    break;
            case HIR_I16CONSTVAL: APPEND("csi16 %ld", s->storage.cnst.value);    break;
            case HIR_U32CONSTVAL: APPEND("csu32 %ld", s->storage.cnst.value);    break;
            case HIR_I32CONSTVAL: APPEND("csi32 %ld", s->storage.cnst.value);    break;
            case HIR_U64CONSTVAL: APPEND("csu64 %ld", s->storage.cnst.value);    break;
            case HIR_I64CONSTVAL: APPEND("csi64 %ld", s->storage.cnst.value);    break;

            case HIR_LABEL: APPEND("lb%li", s->id); break;
            case HIR_RAWASM: {
                str_info_t si;
                if (STTB_get_info_id(s->storage.str.s_id, &si, &smt->s)) {
                    APPEND("%s", si.value->body);
                }

                break;
            }
            case HIR_STRING: {
                str_info_t si;
                if (STTB_get_info_id(s->storage.str.s_id, &si, &smt->s)) {
                    dst = _dump_append_escaped_string(dst, end, si.value->body);
                }

                break;
            }
            case HIR_FNAME: {
                func_info_t fi;
                if (FNTB_get_info_id(s->storage.str.s_id, &fi, &smt->f)) {
                    if (style != 1) APPEND("%s", fi.virt->body);
                    else {
                        APPEND("%s(", fi.virt->body);
                        if (fi.args) {
                            for (ast_node_t* t = fi.args->c; t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
                                if (t->t->t_type == VAR_ARGUMENTS_TOKEN) APPEND("...");
                                else APPEND("%s %s", DUMP_format_token(t->t), t->c->t->body->body);
                                if (t->siblings.n && t->siblings.n->t->t_type != SCOPE_TOKEN) {
                                    APPEND(", ");
                                }
                            }
                        }

                        APPEND(")");
                        if (fi.rtype) {
                            APPEND(" -> %s", DUMP_format_token(fi.rtype->t));
                        }
                    }
                }

                break;
            }
            case HIR_PHISET: {
                APPEND("set ");
                set_foreach (int_tuple_t* tpl, &s->storage.set.h) {
                    variable_info_t pvi;
                    if (VRTB_get_info_id(tpl->y, &pvi, &smt->v)) {
                        APPEND("[%%%li, bb%li]", pvi.v_id, tpl->x);
                    }
                }
                
                break;
            }
            case HIR_ARGLIST: {
                list_iter_t it;
                list_iter_hinit(&s->storage.list.h, &it);
                hir_subject_t* s;
                while (list_iter_next(&it, (void**)&s)) {
                    dst = _get_formatted_subject(dst, end, s, smt, style);
                    if (list_iter_current(&it)) {
                        APPEND(", ");
                    }
                }

                break;
            }
            default: APPEND("nop(%i)", s->t); break;
        }
    }

#undef APPEND
    return dst;
}

/* Format a HIR block and append it to the dump buffer.
Params:
    - `dst` - Output buffer.
    - `dst_size` - Output buffer size.
    - `block` - HIR block to format.
    - `smt` - Symtable used to resolve symbols.
    - `pos` - Whether to include position markers.
    - `unused` - Whether to include unused blocks.
    - `depth` - Current indentation depth.

Returns the updated indentation depth. */
static int _get_formatted_block(char* dst, size_t dst_size, hir_block_t* block, sym_table_t* smt, int pos, int unused, int depth) {
    if (!block) return depth;
    char* end = dst + dst_size;
#define APPEND(...) dst = _dump_appendf(dst, end, __VA_ARGS__)
    if (block->op == HIR_SETPOS) {
        if (pos) {
            APPEND(
                "setpos, line=%li, column=%li, file=%s\n",
                block->farg->storage.pos.line,  block->farg->storage.pos.column,
                block->farg->storage.pos.file ? block->farg->storage.pos.file->body : "<unknown>"
            );
        }

        return depth;
    }

    if (!unused && block->unused && !_dump_always_show_scope(block->op)) return depth;

    int style = HIR_is_funccall(block->op) ? 2 : 1;
    char arg1[HIR_DUMP_SUBJECT_BUFFER] = { 0 };
    if (block->farg) _get_formatted_subject(arg1, arg1 + sizeof(arg1), block->farg, smt, style);
    char arg2[HIR_DUMP_SUBJECT_BUFFER] = { 0 };
    if (block->sarg) _get_formatted_subject(arg2, arg2 + sizeof(arg2), block->sarg, smt, style);
    char arg3[HIR_DUMP_SUBJECT_BUFFER] = { 0 };
    if (block->targ) _get_formatted_subject(arg3, arg3 + sizeof(arg3), block->targ, smt, style);
    
    if (block->op == HIR_ENDSCOPE || block->op == HIR_ENDASM) depth--;

    if (block->op == HIR_FEND || block->op == HIR_STEND) goto _force_end;

    for (int i = 0; i < depth; i++) APPEND("    ");
    if (unused && block->unused) APPEND("[unused] ");
    const char* fmt = _get_operation_template(block->op);
    APPEND(fmt, arg1, arg2, arg3);
    
_force_end: {}
    if (block->op == HIR_MKSCOPE || block->op == HIR_STASM) depth++;
#undef APPEND
    return depth;
}

int DUMP_format_hirctx(hir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output) {
    int curr_tab = 0;
    hir_block_t* hh = ctx->hot.h;
    while (hh) {
        char line[HIR_DUMP_LINE_BUFFER] = { 0 };
        curr_tab = _get_formatted_block(line, sizeof(line), hh, smt, pos, unused, curr_tab);
        fprintf(output, "%s\n", line);
        hh = hh->next;
    }

    return 1;
}

static int _print_hir_cfg_function(cfg_func_t* fb, func_info_t* fi, sym_table_t* smt, FILE* output) {
    fprintf(output, "digraph %s {\n", fi->virt->body);
    fprintf(output, "  rankdir=TB;\n");
    fprintf(output,
            "  node ["
            "shape=box, "
            "fontname=\"DejaVu Sans Mono\", "
            "fontsize=9, "
            "margin=\"0.10,0.06\", "
            "fixedsize=false"
            "];\n");

    foreach (cfg_block_t* cb, &fb->blocks) {
        fprintf(output, "  B%ld [ label=\"", cb->id);
        iterate_hir_instructions (cb) {
            if (
                hh->op == HIR_FDCL         || 
                hh->op == HIR_FEND         || 
                hh->op == HIR_PHI_PREAMBLE || 
                hh->op == HIR_MKSCOPE      ||
                hh->op == HIR_ENDSCOPE
            ) continue; 
            char line[HIR_DUMP_LINE_BUFFER] = { 0 };
            _get_formatted_block(line, sizeof(line), hh, smt, 0, 0, 0);
            fprintf(output, "%s\\l", line);
        }

        fprintf(output, "\"];\n");
        if (cb->l)   fprintf(output, "  B%ld -> B%ld [label=\"fall\"];\n", cb->id, cb->l->id);
        if (cb->jmp) fprintf(output, "  B%ld -> B%ld [label=\"jump\"];\n", cb->id, cb->jmp->id);
    }
    
    fprintf(output, "}\n");
    return 1;
}

int DUMP_format_hir_cfg(cfg_ctx_t* cctx, sym_table_t* smt, const char* name, FILE* output) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        func_info_t fi;
        if (FNTB_get_info_id(fb->f_id, &fi, &smt->f) && fi.name->requals(fi.name, name)) {
            _print_hir_cfg_function(fb, &fi, smt, output);
        }
    }

    return 1;
}
