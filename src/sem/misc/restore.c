#include <sem/misc/restore.h>

const char* RST_restore_type(token_t* t) {
    if (!t) return "";
    switch (t->t_type) {
        case VAR_ARGUMENTS_TOKEN:   return VAR_ARGUMENTS_COMMAND;
        case ARRAY_TYPE_TOKEN:
        case ARR_VARIABLE_TOKEN:    return ARR_VARIABLE;
        case STR_TYPE_TOKEN:
        case STR_VARIABLE_TOKEN:    return !t->flags.ptr ? STR_VARIABLE : "ptr " STR_VARIABLE;
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

/* 
Get the largest .line value from the node and its childs.
Params:
    - `nd` - AST node.
    - `mx` - Current largest line.

Returns 1 if succeeds.
*/
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

static int _restore_code_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent);

static inline void _simple_restore_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent, const char* entry) {
    _rst_ln_puts(x, _rst_line(nd), entry);
    _restore_code_lines(x, nd, u, indent);
}

static int _restore_code_lines(rst_ln_ctx_t* x, ast_node_t* nd, set_t* u, int indent) {
    if (!nd) return 0;

    int line = _rst_line(nd);
    if (u && set_has(u, nd)) _rst_hl_begin(x);

    int complex = -1;
    if (TKN_isdecl(nd->t) || nd->t->t_type == VAR_ARGUMENTS_TOKEN) {
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
    else if (TKN_isoperand(nd->t)) {
        if (nd->c) _restore_code_lines(x, nd->c, u, indent);
        _rst_ln_printf(x, line, " %s ", nd->t->body->body);
        if (nd->c->siblings.n) _restore_code_lines(x, nd->c->siblings.n, u, indent);
    }
    else if (
        TKN_isnumeric(nd->t)  ||
        TKN_isvariable(nd->t)
    ) _rst_ln_puts(x, line, nd->t->body->body);
    else if (
        nd->t->t_type == STRING_VALUE_TOKEN
    ) {
        _rst_ln_puts(x, line, "\"");
        _rst_ln_puts(x, line, nd->t->body->body);
        _rst_ln_puts(x, line, "\"");
    }
    
    switch (nd->t->t_type) {
        case CALLING_TOKEN: {
            if (!TKN_isvariable(nd->c->t)) _rst_ln_puts(x, line, "(");
            _restore_code_lines(x, nd->c, u, indent);
            if (!TKN_isvariable(nd->c->t)) _rst_ln_puts(x, line, ")");

            _rst_ln_puts(x, line, "(");
            for (ast_node_t* arg = nd->c->siblings.n->c; arg; arg = arg->siblings.n) {
                _restore_code_lines(x, nd->c->siblings.n->c, u, indent);
                if (arg->siblings.n) _rst_ln_puts(x, line, ", ");
            }
            _rst_ln_puts(x, line, ")");
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

            int p_line = _rst_line(p);
            _rst_ln_indent(x, p_line, indent);
            int r = _restore_code_lines(x, p, u, indent);
            if (r < 0) _rst_ln_puts(x, p_line, ";\n");

            complex = 1;
            break;
        }

        case FUNC_PROT_TOKEN: {
            _rst_ln_printf(x, line, "%s %s(", FUNCTION_COMMAND, nd->c->t->body->body);

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
            _rst_ln_printf(x, line, "%s %s(", FUNCTION_COMMAND, nd->c->t->body->body);

            ast_node_t* p = nd->c->siblings.n ? nd->c->siblings.n->c : NULL;
            for (; p && p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n && p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ") ");
            if (nd->c->c) _rst_ln_printf(x, line, "-> %s ", RST_restore_type(nd->c->c->t));
            _rst_ln_puts(x, line, "\n");

            int p_line = _rst_line(p);
            _rst_ln_indent(x, p_line, indent);
            int r = _restore_code_lines(x, p, u, indent);
            if (r < 0) _rst_ln_puts(x, p_line, ";\n");

            complex = 1;
            break;
        }

        case CALL_TOKEN: {
            _rst_ln_printf(x, line, "%s(", nd->t->body->body);
            for (ast_node_t* p = nd->c->c; p; p = p->siblings.n) {
                _restore_code_lines(x, p, u, indent);
                if (p->siblings.n) _rst_ln_puts(x, line, ", ");
            }

            _rst_ln_puts(x, line, ")");
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
        case NEGATIVE_TOKEN:   _simple_restore_lines(x, nd->c, u, indent, NEGATIVE_COMMAND " ");    break;
        case LOOP_TOKEN:       _simple_restore_lines(x, nd->c, u, indent, LOOP_COMMAND);            break;
        case EXIT_TOKEN:       _simple_restore_lines(x, nd->c, u, indent, EXIT_COMMAND " ");        break;
        case RETURN_TOKEN:     _simple_restore_lines(x, nd->c, u, indent, RETURN_COMMAND " ");      break;
        case BREAK_TOKEN:      _simple_restore_lines(x, NULL, u, indent, BREAK_COMMAND " ");        break;
        case BREAKPOINT_TOKEN: _simple_restore_lines(x, nd->c, u, indent, BREAKPOINT_COMMAND " ");  break;

        case CONVERT_TOKEN: {
            if (nd->c && nd->c->siblings.n) _restore_code_lines(x, nd->c->siblings.n, u, indent);
            _rst_ln_printf(x, line, "%s %s", CONVERT_COMMAND, RST_restore_type(nd->c->t));
            break;
        }

        case WHILE_TOKEN: {
            _rst_ln_puts(x, line, WHILE_COMAND " ");
            if (nd->c) _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, ";\n");

            ast_node_t* body = nd->c ? nd->c->siblings.n : NULL;
            int body_line = _rst_line(body);
            _rst_ln_indent(x, body_line, indent);
            _restore_code_lines(x, body, u, indent);

            complex = 1;
            break;
        }

        case IF_TOKEN: {
            _rst_ln_puts(x, line, IF_COMMAND " ");
            if (nd->c) _restore_code_lines(x, nd->c, u, indent);
            _rst_ln_puts(x, line, ";\n");

            ast_node_t* tbranch = (nd->c ? nd->c->siblings.n : NULL);
            ast_node_t* fbranch = (tbranch ? tbranch->siblings.n : NULL);

            int tb_line = _rst_line(tbranch);
            _rst_ln_indent(x, tb_line, indent);
            _restore_code_lines(x, tbranch, u, indent);

            if (fbranch) {
                int fb_line = _rst_line(fbranch);
                _rst_ln_indent(x, fb_line, indent);
                _rst_ln_puts(x, fb_line, "else ");
                _restore_code_lines(x, fbranch, u, indent);
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
        .fd = fd,
        .width = _rst_digits(mx > 0 ? mx : 1),
        .at_line_start = 1,

        .spans  = NULL,
        .sp_cap = 0,
        .sp_n   = 0
    };
    
    if (_restore_code_lines(&x, nd, u, indent) < 0) {
        fprintf(fd, ";");
        if (!set_size(u)) fprintf(fd, "\n");
    }

    _rst_flush_markers(&x);
    mm_free(x.spans);
    return 1;
}
