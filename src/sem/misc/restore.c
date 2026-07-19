#include <sem/misc/restore.h>

const char* RST_restore_type(token_t* t) {
    if (!t) return "";
    switch (t->t_type) {
        case VAR_ARGUMENTS_TOKEN:   return VAR_ARGUMENTS_COMMAND;
        case ARRAY_TYPE_TOKEN:
        case ARR_VARIABLE_TOKEN:    return ARR_VARIABLE;
        case CUSTOM_TYPE_TOKEN:
        case CUSTOM_VARIABLE_TOKEN:
        case GENERIC_TYPE_TOKEN:
        case GENERIC_VARIABLE_TOKEN: {
            static char buf[256];
            if (!t->flags.ptr) return t->body ? t->body->body : "";
            snprintf(buf, sizeof(buf), "ptr %s", t->body ? t->body->body : "");
            return buf;
        }
        case I0_VARIABLE_TOKEN:
        case I0_TYPE_TOKEN:         return !t->flags.ptr ? I0_VARIABLE  : "ptr " I0_VARIABLE;
        case I8_VARIABLE_TOKEN:
        case I8_TYPE_TOKEN:         return !t->flags.ptr ? I8_VARIABLE  : "ptr " I8_VARIABLE;
        case U8_VARIABLE_TOKEN:
        case U8_TYPE_TOKEN:         return !t->flags.ptr ? U8_VARIABLE  : "ptr " U8_VARIABLE;
        case I16_VARIABLE_TOKEN:
        case I16_TYPE_TOKEN:        return !t->flags.ptr ? I16_VARIABLE : "ptr " I16_VARIABLE;
        case U16_VARIABLE_TOKEN:
        case U16_TYPE_TOKEN:        return !t->flags.ptr ? U16_VARIABLE : "ptr " U16_VARIABLE;
        case I32_VARIABLE_TOKEN:
        case I32_TYPE_TOKEN:        return !t->flags.ptr ? I32_VARIABLE : "ptr " I32_VARIABLE;
        case U32_VARIABLE_TOKEN:
        case U32_TYPE_TOKEN:        return !t->flags.ptr ? U32_VARIABLE : "ptr " U32_VARIABLE;
        case F32_VARIABLE_TOKEN:
        case F32_TYPE_TOKEN:        return !t->flags.ptr ? F32_VARIABLE : "ptr " F32_VARIABLE;
        case I64_VARIABLE_TOKEN:
        case I64_TYPE_TOKEN:        return !t->flags.ptr ? I64_VARIABLE : "ptr " I64_VARIABLE;
        case U64_VARIABLE_TOKEN:
        case U64_TYPE_TOKEN:        return !t->flags.ptr ? U64_VARIABLE : "ptr " U64_VARIABLE;
        case F64_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:        return !t->flags.ptr ? F64_VARIABLE : "ptr " F64_VARIABLE;
        default: return "";
    }
}

/*
Give a number's character count.
Params:
    - `v` - Number.

Returns a number of characters that are used for
a printing of the provided number.
*/
static inline int _rst_digits(int v) {
    int d = 1;
    while (v >= 10) { v /= 10; ++d; }
    return d;
}

/*  Get the largest .line value from the node and its children.
Params:
    - `nd` - AST node.
    - `mx` - Current largest line.

Returns 1 if succeeds */
static int _rst_max_line(ast_node_t* nd, int* mx) {
    for (ast_node_t* p = nd; p; p = p->siblings.n) {
        if (p->t) {
            int l = p->t->finfo.line;
            if (l > *mx) *mx = l;
        }

        if (p->c) {
            _rst_max_line(p->c, mx);
        }
    }

    return 1;
}

typedef struct { 
    int start;  /* Span column start */ 
    int end;    /* Span column end   */
} rst_span_t;

typedef struct {
    FILE*       fd;               /* Output location      */
    int         width;            /* Max line number size */
    int         at_line_start;    /* Start row number     */
    int         ul_active;        /* Underline flag       */

    int         col;
    int         hl_depth;
    int         hl_start;

    rst_span_t* spans;
    int         sp_n;
    int         sp_cap;
} rst_ln_ctx_t;

static int _rst_span_push(rst_ln_ctx_t* x, int s, int e) {
    if (e <= s) e = s + 1;
    if (s < 1) s = 1;

    if (x->sp_n == x->sp_cap) {
        x->sp_cap = x->sp_cap ? x->sp_cap * 2 : 8;
        x->spans = (rst_span_t*)mm_realloc(x->spans, (size_t)x->sp_cap * sizeof(*x->spans));
    }

    x->spans[x->sp_n].start = s;
    x->spans[x->sp_n++].end = e;
    return 1;
}

static inline void _rst_hl_begin(rst_ln_ctx_t* x) {
    if (!(x->hl_depth++)) x->hl_start = (x->col > 0 ? x->col : 1);
}

static inline void _rst_hl_end(rst_ln_ctx_t* x) {
    if (x->hl_depth > 0 && !(--x->hl_depth)) {
        _rst_span_push(x, x->hl_start, (x->col > 0 ? x->col : 1));
    }
}

static void _rst_print_markers_for_line(rst_ln_ctx_t* x) {
    if (x->sp_n <= 0) return;

    int maxe = 0;
    for (int i = 0; i < x->sp_n; ++i) {
        if (x->spans[i].end > maxe) maxe = x->spans[i].end;
    }

    int mlen = (maxe > 1 ? maxe - 1 : 0);
    if (mlen <= 0) { x->sp_n = 0; return; }

    char* m = (char*)mm_malloc((size_t)mlen + 1);
    str_memset(m, ' ', (size_t)mlen);
    m[mlen] = 0;

    for (int i = 0; i < x->sp_n; ++i) {
        int s = x->spans[i].start;
        int e = x->spans[i].end;
        if (s < 1) s = 1;
        if (e < s + 1) e = s + 1;
        for (int k = s; k < e && k <= mlen; ++k) {
            m[k - 1] = '^';
        }
    }

    fprintf(x->fd, "%*s | %s\n", x->width, "", m);

    mm_free(m);
    x->sp_n = 0;
}

/*
Get the node line.
Params
    - `nd` - AST node.

Returns the node's line number.
*/
static inline int _rst_line(ast_node_t* nd) {
    if (!nd || !nd->t) return 0;
    return nd->t->finfo.line;
}

/*
Print the line's prefix with a number of a line.
Params:
    - `x` - Line writer context.
    - `line` - Line number.
*/
static inline void _rst_ln_prefix(rst_ln_ctx_t* x, int line) {
    if (x->ul_active) fputs(UL_OFF, x->fd);

    if (line > 0) fprintf(x->fd, "%*d ", x->width, line);
    else          fprintf(x->fd, "%*s ", x->width, "");
    fputs("| ", x->fd);

    if (x->ul_active) fputs(UL_ON, x->fd);
}

static int _rst_ln_write(rst_ln_ctx_t* x, int line, const char* s, size_t n) {
    size_t i = 0;
    while (i < n) {
        if (x->at_line_start) {
            _rst_ln_prefix(x, line);
            x->at_line_start = 0;
            x->col = 1;
            if (x->hl_depth > 0) x->hl_start = 1;
        }

        size_t j = i;
        for (; j < n && s[j] != '\n'; ++j);

        if (j > i) {
            fwrite(s + i, 1, j - i, x->fd);
            x->col += (int)(j - i);
        }

        if (j < n && s[j] == '\n') {
            if (x->hl_depth > 0) {
                _rst_span_push(x, x->hl_start, x->col);
                x->hl_start = 1;
            }

            fputc('\n', x->fd);
            _rst_print_markers_for_line(x);

            x->at_line_start = 1;
            i = j + 1;
        } 
        else {
            i = j;
        }
    }

    return 1;
}

static int _rst_ln_puts(rst_ln_ctx_t* x, int line, const char* s) {
    if (!s) return 0;
    const char* p = s;
    while (*p) ++p;
    _rst_ln_write(x, line, s, (size_t)(p - s));
    return 1;
}

static int _rst_ln_puts_escaped(rst_ln_ctx_t* x, int line, const char* s) {
    if (!s) return 0;
    for (const char* p = s; *p; ++p) {
        switch (*p) {
            case '\n': _rst_ln_puts(x, line, "\\n"); break;
            case '\r': _rst_ln_puts(x, line, "\\r"); break;
            default:   _rst_ln_write(x, line, p, 1);  break;
        }
    }

    return 1;
}

static int _rst_ln_printf(rst_ln_ctx_t* x, int line, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    va_list ap2;
    va_copy(ap2, ap);
    
    int need = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (need <= 0) { 
        va_end(ap); 
        return 0; 
    }

    char* buf = (char*)mm_malloc((size_t)need + 1);
    if (!buf) { 
        va_end(ap); 
        return 0; 
    }

    vsnprintf(buf, (size_t)need + 1, fmt, ap);
    va_end(ap);

    _rst_ln_write(x, line, buf, (size_t)need);
    mm_free(buf);
    return 1;
}

static inline void _rst_ln_indent(rst_ln_ctx_t* x, int line, int indent) {
    for (int i = 0; i < indent; ++i) _rst_ln_puts(x, line, "    ");
}

static int _restore_annotation(rst_ln_ctx_t* x, int line, annotation_t* annot) {
    if (!annot) return 0;

    switch (annot->t) {
        case ALIGN_ANNOTATION:    _rst_ln_printf(x, line, "%s(%i)", ALIGN_ANNOTATION_COMMAND, annot->data.align);                  break;
        case ADDRESS_ANNOTATION:  _rst_ln_printf(x, line, "%s(%ld)", ADDRS_ANNOTATION_COMMAND, annot->data.address);               break;
        case COUNTER_ANNOTATION:  _rst_ln_printf(x, line, "%s(%ld)", COUNT_ANNOTATION_COMMAND, annot->data.counter);               break;
        case REGISTER_ANNOTATION: _rst_ln_printf(x, line, "%s(%i)", REGST_ANNOTATION_COMMAND, (int)annot->data.regval);            break;
        case ENTRY_ANNOTATION: {
            _rst_ln_puts(x, line, ENTRY_ANNOTATION_COMMAND);
            if (annot->data.fname) _rst_ln_printf(x, line, "(\"%s\")", annot->data.fname->body);
            break;
        }
        case VNAME_ANNOTATION: {
            _rst_ln_puts(x, line, VNAME_ANNOTATION_COMMAND);
            if (annot->data.fname) _rst_ln_printf(x, line, "(\"%s\")", annot->data.fname->body);
            break;
        }
        case SECTION_ANNOTATION: {
            _rst_ln_printf(
                x, line, "%s(\"%s\"",
                SECTN_ANNOTATION_COMMAND,
                annot->data.section.section ? annot->data.section.section->body : ""
            );
            if (annot->data.section.align != FIELD_NO_CHANGE) {
                _rst_ln_printf(x, line, ", %i", annot->data.section.align);
            }
            _rst_ln_puts(x, line, ")");
            break;
        }
        case INLINE_ANNOTATION: {
            _rst_ln_puts(x, line, INLNE_ANNOTATION_COMMAND);
            if (annot->data.inline_opt) _rst_ln_printf(x, line, "(%s)", annot->data.inline_opt->body);
            break;
        }
        case NOSECTION_ANNOTATION: _rst_ln_puts(x, line, NOSEC_ANNOTATION_COMMAND); break;
        case NAKED_ANNOTATION:     _rst_ln_puts(x, line, NAKED_ANNOTATION_COMMAND); break;
        case NOFALL_ANNOTATION:    _rst_ln_puts(x, line, NOFAL_ANNOTATION_COMMAND); break;
        case NOTLAZY_ANNOTATION:   _rst_ln_puts(x, line, NTLAZ_ANNOTATION_COMMAND); break;
        case STRAIGHT_ANNOTATION:  _rst_ln_puts(x, line, STRGH_ANNOTATION_COMMAND); break;
        case HOT_ANNOTATION:       _rst_ln_puts(x, line, HOTSC_ANNOTATION_COMMAND); break;
        case COLD_ANNOTATION:      _rst_ln_puts(x, line, COLDS_ANNOTATION_COMMAND); break;
        case POPARG_ANNOTATION:    _rst_ln_puts(x, line, POPRG_ANNOTATION_COMMAND); break;
        case SELF_ANNOTATION:      _rst_ln_puts(x, line, SSELF_ANNOTATION_COMMAND); break;
        case LIKEC_ANNOTATION:     _rst_ln_puts(x, line, LIKEC_ANNOTATION_COMMAND); break;
        case UNION_ANNOTATION:     _rst_ln_puts(x, line, UNION_ANNOTATION_COMMAND); break;
        case WEAK_ANNOTATION:      _rst_ln_puts(x, line, WEAKS_ANNOTATION_COMMAND); break;
        case ABI_ANNOTATION:       _rst_ln_puts(x, line, ABICC_ANNOTATION_COMMAND); break;
        case ONLYBODY_ANNOTATION:  _rst_ln_puts(x, line, BODYO_ANNOTATION_COMMAND); break;
        default:                   _rst_ln_puts(x, line, "unknown");                break;
    }

    return 1;
}

static int _restore_annotations(rst_ln_ctx_t* x, ast_node_t* nd, int line) {
    if (!nd || !list_size(&nd->annots)) return 0;

    list_iter_t it;
    annotation_t* annot = NULL;
    list_iter_hinit(&nd->annots, &it);
    while (list_iter_next(&it, (void**)&annot)) {
        _rst_ln_puts(x, line, "@[");
        _restore_annotation(x, line, annot);
        _rst_ln_puts(x, line, "] ");
    }

    return 1;
}

static int _restore_code_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent);

static inline void _simple_restore_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent, const char* entry) {
    _rst_ln_puts(x, _rst_line(nd), entry);
    _restore_code_lines(x, nd, u, indent);
}

static int _restore_body_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent) {
    if (!nd) return 0;

    int line = _rst_line(nd);
    _rst_ln_indent(x, line, indent);
    int r = _restore_code_lines(x, nd, u, indent);
    if (r < 0) _rst_ln_puts(x, line, ";\n");
    return r;
}

static int _restore_inline_scope_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent) {
    if (!nd || !nd->t || nd->t->t_type != SCOPE_TOKEN) {
        return _restore_code_lines(x, nd, u, indent);
    }

    int line = _rst_line(nd);
    _rst_ln_puts(x, line, "{\n");
    for (ast_node_t* c = nd->c; c; c = c->siblings.n) {
        int cl = _rst_line(c);
        _rst_ln_indent(x, cl, indent + 1);
        if (_restore_code_lines(x, c, u, indent + 1) < 0) _rst_ln_puts(x, cl, ";\n");
    }

    _rst_ln_indent(x, line, indent);
    _rst_ln_puts(x, line, "}");
    return 1;
}

static int _restore_code_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent) {
    if (!nd || !nd->t) return 0;

    int line = _rst_line(nd);
    if (u && set_has(u, nd)) _rst_hl_begin(x);
    _restore_annotations(x, nd, line);

    int complex = -1;
    if (
        TKN_is_builtin_type(nd->t) ||
        nd->t->t_type == CUSTOM_TYPE_TOKEN ||
        nd->t->t_type == GENERIC_TYPE_TOKEN ||
        nd->t->t_type == VAR_ARGUMENTS_TOKEN
    ) {
        if (nd->t->t_type != ARRAY_TYPE_TOKEN) {
            _rst_ln_printf(
                x, line, "%s%s%s%s%s",
                nd->t->flags.glob ? "glob " : "",
                nd->t->flags.ro ? "ro " : "",
                RST_restore_type(nd->t),
                nd->c ? " " : "",
                nd->c ? nd->c->t->body->body : ""
            );

            if (nd->c && nd->c->siblings.n) {
                _rst_ln_puts(x, line, " = ");
                _restore_code_lines(x, nd->c->siblings.n, u, indent);
            }
        }
        else {
            _rst_ln_printf(
                x, line, "%s%sarr %s[%s, %s]",
                nd->t->flags.glob ? "glob " : "",
                nd->t->flags.ro ? "ro " : "",
                nd->c->t->body->body,
                nd->c->siblings.n->t->body->body,
                nd->c->siblings.n->siblings.n->t->body->body
            );

            if (nd->c->siblings.n->siblings.n->siblings.n) {
                _rst_ln_puts(x, line, " = { ");
                for (ast_node_t* arg = nd->c->siblings.n->siblings.n->siblings.n; arg; arg = arg->siblings.n) {
                    _restore_code_lines(x, arg, u, indent);
                    if (arg->siblings.n) _rst_ln_puts(x, line, ", ");
                }
                _rst_ln_puts(x, line, " }");
            }
        }
    }
    else if (TKN_is_operand(nd->t)) {
        if (nd->c) _restore_code_lines(x, nd->c, u, indent);
        if (nd->c && nd->c->siblings.n) {
            _rst_ln_printf(x, line, " %s ", nd->t->body->body);
            _restore_code_lines(x, nd->c->siblings.n, u, indent);
        }
    }
    else if (
        TKN_is_numeric(nd->t)  ||
        nd->t->t_type == UNKNOWN_STRING_TOKEN ||
        TKN_is_variable(nd->t) ||
        nd->t->t_type == CUSTOM_VARIABLE_TOKEN ||
        nd->t->t_type == GENERIC_VARIABLE_TOKEN ||
        nd->t->t_type == ADDR_CALL_TOKEN ||
        nd->t->t_type == CALL_ADDR_TOKEN ||
        nd->t->t_type == FUNC_NAME_TOKEN
    ) _rst_ln_puts(x, line, nd->t->body->body);
    else if (
        nd->t->t_type == STRING_VALUE_TOKEN
    ) {
        _rst_ln_puts(x, line, "\"");
        _rst_ln_puts_escaped(x, line, nd->t->body->body);
        _rst_ln_puts(x, line, "\"");
    }
    else if (
        nd->t->t_type == CHAR_VALUE_TOKEN
    ) {
        _rst_ln_puts(x, line, "'");
        _rst_ln_puts_escaped(x, line, nd->t->body->body);
        _rst_ln_puts(x, line, "'");
    }
    
    switch (nd->t->t_type) {
        case CALL_ADDR_TOKEN: {
            if (!nd->c) break;

            _rst_ln_puts(x, line, "<");
            for (ast_node_t* type = nd->c; type; type = type->siblings.n) {
                _rst_ln_puts(x, line, RST_restore_type(type->t));
                if (type->siblings.n) _rst_ln_puts(x, line, ", ");
            }
            _rst_ln_puts(x, line, ">");
            break;
        }
        case CALLING_TOKEN: {
            if (!nd->c) break;

            int simple_callee = (
                TKN_is_variable(nd->c->t) ||
                nd->c->t->t_type == CUSTOM_VARIABLE_TOKEN ||
                nd->c->t->t_type == GENERIC_VARIABLE_TOKEN ||
                nd->c->t->t_type == ADDR_CALL_TOKEN ||
                nd->c->t->t_type == FUNC_NAME_TOKEN ||
                nd->c->t->t_type == CALL_ADDR_TOKEN
            );
            if (!simple_callee) _rst_ln_puts(x, line, "(");
            _restore_code_lines(x, nd->c, u, indent);
            if (!simple_callee) _rst_ln_puts(x, line, ")");

            _rst_ln_puts(x, line, "(");
            ast_node_t* args = nd->c->siblings.n ? nd->c->siblings.n->c : NULL;
            for (ast_node_t* arg = args; arg; arg = arg->siblings.n) {
                _restore_code_lines(x, arg, u, indent);
                if (arg->siblings.n) _rst_ln_puts(x, line, ", ");
            }
            _rst_ln_puts(x, line, ")");
            break;
        }
        case SIZEOF_TOKEN: {
            _rst_ln_puts(x, line, SIZEOF_COMMAND "(");
            _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, ")");
            break;
        }
        case LAMBDA_FUNCTION_TOKEN: {
            ast_node_t* args = nd->c;
            ast_node_t* p = args ? args->c : NULL;

            _rst_ln_puts(x, line, "(");
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) {
                    _rst_ln_puts(x, line, ", ");
                }
            }

            _rst_ln_puts(x, line, ") " LAMBDA_COMMAND " ");
            _restore_inline_scope_lines(x, p, u, indent);
            break;
        }
        case MEMBER_ACCESS_TOKEN: {
            _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, ".");
            _restore_code_lines(x, nd->c ? nd->c->siblings.n : NULL, u, indent);
            break;
        }
        case CONTAINER_TOKEN: {
            ast_node_t* name = nd->c;
            ast_node_t* body = name ? name->siblings.n : NULL;
            _rst_ln_printf(x, line, "%s %s\n", CONTAINER_COMMAND, name && name->t ? name->t->body->body : "");

            _restore_body_lines(x, body, u, indent);

            complex = 1;
            break;
        }
        case INDEXATION_TOKEN: {
            _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, "[");
            _restore_code_lines(x, nd->c->siblings.n, u, indent);
            _rst_ln_puts(x, line, "]");
            break;
        }
        case START_TOKEN: {
            _rst_ln_puts(x, line, START_COMMAND " (");
            ast_node_t* p = nd->c;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ")\n");

            _restore_body_lines(x, p, u, indent);

            complex = 1;
            break;
        }
        case FUNC_PROT_TOKEN: {
            _rst_ln_printf(
                x, line, "%s%s %s(",
                nd->t->flags.glob ? GLOB_COMMAND " " : "",
                FUNCTION_COMMAND,
                nd->c->t->body->body
            );

            ast_node_t* p = nd->c->siblings.n ? nd->c->siblings.n->c : NULL;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ")");
            if (nd->c->c) _rst_ln_printf(x, line, " -> %s", RST_restore_type(nd->c->c->t));
            break;
        }
        case FUNC_TOKEN: {
            _rst_ln_printf(
                x, line, "%s%s %s(",
                nd->t->flags.glob ? GLOB_COMMAND " " : "",
                FUNCTION_COMMAND,
                nd->c->t->body->body
            );

            ast_node_t* p = nd->c->siblings.n ? nd->c->siblings.n->c : NULL;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ") ");
            if (nd->c->c) _rst_ln_printf(x, line, "-> %s ", RST_restore_type(nd->c->c->t));
            _rst_ln_puts(x, line, "\n");

            _restore_body_lines(x, p, u, indent);

            complex = 1;
            break;
        }
        case ASM_TOKEN: {
            _rst_ln_puts(x, line, ASM_COMMAND "(");
            ast_node_t *args = nd->c, *body = args ? args->siblings.n : NULL;
            for (ast_node_t* arg = args ? args->c : NULL; arg; arg = arg->siblings.n) {
                _restore_code_lines(x, arg, u, indent);
                if (arg->siblings.n) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ")\n");

            int body_line = _rst_line(body);
            _rst_ln_indent(x, body_line, indent);
            _rst_ln_puts(x, body_line, "{\n");
            for (ast_node_t* stmt = body ? body->c : NULL; stmt; stmt = stmt->siblings.n) {
                int stmt_line = _rst_line(stmt);
                _rst_ln_indent(x, stmt_line, indent + 1);
                _restore_code_lines(x, stmt, u, indent + 1);
                _rst_ln_puts(x, stmt_line, stmt->siblings.n ? ",\n" : "\n");
            }
            _rst_ln_indent(x, body_line, indent);
            _rst_ln_puts(x, body_line, "}\n");
            complex = 1;
            break;
        }
        case SYSCALL_TOKEN: {
            _rst_ln_puts(x, line, SYSCALL_COMMAND "(");
            for (ast_node_t* p = nd->c; p; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ")");
            break;
        }
        case EXTERN_TOKEN:     _simple_restore_lines(x, nd->c, u, indent, EXTERN_COMMAND " ");      break;
        case REF_TYPE_TOKEN:   _simple_restore_lines(x, nd->c, u, indent, REF_COMMAND " ");         break;
        case DREF_TYPE_TOKEN:  _simple_restore_lines(x, nd->c, u, indent, DREF_COMMAND " ");        break;
        case NOT_TOKEN:        _simple_restore_lines(x, nd->c, u, indent, NOT_COMMAND " ");         break;
        case NEGATIVE_TOKEN:   _simple_restore_lines(x, nd->c, u, indent, NEGATIVE_COMMAND " ");    break;
        case LOOP_TOKEN: {
            _rst_ln_puts(x, line, LOOP_COMMAND "\n");
            _restore_body_lines(x, nd->c, u, indent);
            complex = 1;
            break;
        }
        case EXIT_TOKEN:       _simple_restore_lines(x, nd->c, u, indent, EXIT_COMMAND " ");        break;
        case RETURN_TOKEN:     _simple_restore_lines(x, nd->c, u, indent, RETURN_COMMAND " ");      break;
        case BREAK_TOKEN:      _simple_restore_lines(x, NULL, u, indent, BREAK_COMMAND " ");        break;
        case BREAKPOINT_TOKEN: _simple_restore_lines(x, nd->c, u, indent, BREAKPOINT_COMMAND " ");  break;
        case CONVERT_TOKEN: {
            if (nd->c && nd->c->siblings.n) _restore_code_lines(x, nd->c->siblings.n, u, indent);
            _rst_ln_printf(x, line, " %s %s", CONVERT_COMMAND, RST_restore_type(nd->c->t));
            break;
        }
        case CASE_TOKEN:
        case DEFAULT_TOKEN:
        case SWITCH_TOKEN:
        case WHILE_TOKEN: {
            if (nd->t->t_type == DEFAULT_TOKEN) {
                _rst_ln_puts(x, line, DEFAULT_COMMAND "\n");
            }
            else {
                switch (nd->t->t_type) {
                    case WHILE_TOKEN:  _rst_ln_puts(x, line, WHILE_COMAND " ");   break;
                    case SWITCH_TOKEN: _rst_ln_puts(x, line, SWITCH_COMMAND " "); break;
                    default:           _rst_ln_puts(x, line, CASE_COMMAND " ");   break;
                }

                if (nd->c) _restore_code_lines(x, nd->c, u, indent);
                _rst_ln_puts(x, line, ";\n");
            }

            ast_node_t* body = nd->c ? nd->c->siblings.n : NULL;
            if (nd->t->t_type == DEFAULT_TOKEN) body = nd->c;
            _restore_body_lines(x, body, u, indent);

            complex = 1;
            break;
        }
        case IF_TOKEN: {
            _rst_ln_puts(x, line, IF_COMMAND " ");
            if (nd->c) _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, ";\n");

            ast_node_t* tbranch = (nd->c ? nd->c->siblings.n : NULL);
            ast_node_t* fbranch = (tbranch ? tbranch->siblings.n : NULL);

            _restore_body_lines(x, tbranch, u, indent);

            if (fbranch) {
                int fb_line = _rst_line(fbranch);
                _rst_ln_indent(x, fb_line, indent);
                _rst_ln_puts(x, fb_line, "else ");
                int r = _restore_code_lines(x, fbranch, u, indent);
                if (r < 0) _rst_ln_puts(x, fb_line, ";\n");
            }

            complex = 1;
            break;
        }
        case SCOPE_TOKEN: {
            _rst_ln_puts(x, line, "{\n");
            for (ast_node_t* c = nd->c; c; c = c->siblings.n) {
                int cl = _rst_line(c);
                _rst_ln_indent(x, cl, indent + 1);
                if (_restore_code_lines(x, c, u, indent + 1) < 0) _rst_ln_puts(x, cl, ";\n");
            }

            _rst_ln_indent(x, line, indent);
            _rst_ln_puts(x, line, "}\n");
            complex = 1;
            break;
        }
        default: break;
    }

    if (u && set_has(u, nd)) _rst_hl_end(x);
    return complex;
}

static int _rst_flush_markers(rst_ln_ctx_t* x) {
    if (x->hl_depth > 0) {
        _rst_span_push(x, x->hl_start, (x->col > 0 ? x->col : 1));
        x->hl_depth = 0;
    }

    if (x->sp_n > 0) {
        if (!x->at_line_start) fputc('\n', x->fd);
        _rst_print_markers_for_line(x);
        x->at_line_start = 1;
    }

    return 1;
}

int RST_restore_code(FILE* fd, ast_node_t* nd, set_t* u, int indent) {
    int mx = 0;
    _rst_max_line(nd, &mx);

    rst_ln_ctx_t x = {
        .fd            = fd,
        .width         = _rst_digits(mx > 0 ? mx : 1),
        .at_line_start = 1,
        .spans         = NULL,
        .sp_cap        = 0,
        .sp_n          = 0
    };
    
    if (_restore_code_lines(&x, nd, u, indent) < 0) {
        _rst_ln_puts(&x, 0, ";");
        _rst_ln_puts(&x, 0, "\n");
    }

    _rst_flush_markers(&x);
    mm_free(x.spans);
    return 1;
}
