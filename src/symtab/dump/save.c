#include <symtab/dump.h>

static char* _dump_appendf(char* dst, char* end, const char* fmt, ...);

static char* _format_type(symbol_id_t id, typetab_ctx_t* ctx) {
    static char signature[256];
    type_info_t ti;
    if (TPTB_get_info_id(id, &ti, ctx)) {
        if (ti.name) return ti.name->body;
        if (ti.t == TYPE_SIGNATURE) {
            char* dst = signature;
            char* end = signature + sizeof(signature);
            dst = _dump_appendf(dst, end, "fn(");

            int first = 1;
            foreach (symbol_id_t arg_id, &ti.body.signature.arg_types) {
                if (!first) dst = _dump_appendf(dst, end, ",");
                dst = _dump_appendf(dst, end, "%li", arg_id);
                first = 0;
            }

            dst = _dump_appendf(dst, end, ")%li", ti.body.signature.ret_type);
            for (int i = 0; i < ti.ptr; i++) {
                dst = _dump_appendf(dst, end, "*");
            }
            *dst = 0;
            return signature;
        }
    }

    return "NULL";
}

static inline const char* _format_flag(const char* base, int flag) {
    return flag ? base : "";
}

static int _format_varinfo(variable_info_t* vi, sym_table_t* smt, FILE* output) {
    fprintf(
        output,
        "var id=%li type=%s name=%s align=%i par=%li scope=%li ptr=%i%s%s%s%s%s %s\n",
        vi->v_id, _format_type(vi->t_id, &smt->t), vi->name->body, vi->vmi.align,
        vi->p_id, vi->s_id, vi->vfs.ptr,
        _format_flag(", ro", vi->vfs.ro),   _format_flag(", glob", vi->vfs.glob), 
        _format_flag(", ext", vi->vfs.ext), _format_flag(", not_null", vi->csa.not_null),
        _format_flag(", volatile", vi->vmi.vlatile),
        vi->vmi.reg != SMT_NULL ? DUMP_registers_to_string(vi->vmi.reg) : ""
    );
    return 1;
}

int DUMP_format_vartb(sym_table_t* smt, FILE* output) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        _format_varinfo(vi, smt, output);
    }

    return 1;
}

static char* _dump_appendf(char* dst, char* end, const char* fmt, ...) {
    if (dst >= end) return dst;

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(dst, end - dst, fmt, args);
    va_end(args);

    if (written < 0) return dst;
    if (written >= end - dst) return end - 1;
    return dst + written;
}

static inline const char* _format_str(string_t* str) {
    return str && str->body ? str->body : "NULL";
}

static char* _format_token(char* dst, char* end, token_t* tkn) {
    if (!tkn) return _dump_appendf(dst, end, "NULL");
    const char* body = tkn->body && tkn->body->body ? tkn->body->body : DUMP_format_token_type(tkn->t_type);
    if (!body || !body[0]) body = "?";
    dst = _dump_appendf(dst, end, "%s", body);
    for (int i = 0; i < tkn->flags.ptr; i++) {
        dst = _dump_appendf(dst, end, "*");
    }

    return dst;
}

static int _format_func_args(ast_node_t* args, char* output, size_t output_size) {
    char* dst = output;
    char* end = output + output_size;
    int first = 1;

    dst = _dump_appendf(dst, end, "(");
    for (ast_node_t* arg = args ? args->c : NULL; arg && arg->t && arg->t->t_type != SCOPE_TOKEN; arg = arg->siblings.n) {
        if (!first) dst = _dump_appendf(dst, end, ", ");

        if (arg->t->t_type == VAR_ARGUMENTS_TOKEN) {
            dst = _dump_appendf(dst, end, "...");
        }
        else {
            dst = _format_token(dst, end, arg->t);
            if (arg->c && arg->c->t) {
                dst = _dump_appendf(dst, end, " ");
                dst = _format_token(dst, end, arg->c->t);
            }
        }

        first = 0;
    }
    _dump_appendf(dst, end, ")");
    return 1;
}

static int _format_func_type(ast_node_t* type, char* output, size_t output_size) {
    char* dst = output;
    char* end = output + output_size;
    _format_token(dst, end, type ? type->t : NULL);
    return 1;
}

static int _format_id_list(list_t* ids, char* output, size_t output_size) {
    char* dst = output;
    char* end = output + output_size;
    int first = 1;

    dst = _dump_appendf(dst, end, "{");
    foreach (symbol_id_t id, ids) {
        if (!first) dst = _dump_appendf(dst, end, ", ");
        dst = _dump_appendf(dst, end, "%li", id);
        first = 0;
    }
    _dump_appendf(dst, end, "}");
    return 1;
}

static int _format_funcinfo(func_info_t* fi, FILE* output) {
    char args[128]         = { 0 };
    char rtype[64]         = { 0 };
    char locals[64]        = { 0 };
    char generic_types[64] = { 0 };
    char resolutions[64]   = { 0 };

    _format_func_args(fi->args, args, sizeof(args));
    _format_func_type(fi->rtype, rtype, sizeof(rtype));
    _format_id_list(&fi->local, locals, sizeof(locals));
    _format_id_list(&fi->template.registered_types, generic_types, sizeof(generic_types));
    _format_id_list(&fi->template.resolutions, resolutions, sizeof(resolutions));

    fprintf(
        output,
        "fn id=%li name=%s virt=%s scope=%li args=%s ret=%s locals=%s generic_types=%s resolutions=%s "
        "%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
        fi->id, _format_str(fi->name), _format_str(fi->virt), fi->s_id,
        args, rtype, locals, generic_types, resolutions,
        _format_flag(", ext", fi->flags.external),     _format_flag(", glob", fi->flags.global), _format_flag(", entry", fi->flags.entry),
        _format_flag(", used", fi->flags.used),        _format_flag(", local", fi->flags.local), _format_flag(", vargs", fi->flags.vargs),
        _format_flag(", gen", fi->flags.generic),      _format_flag(", abi", fi->flags.abi),     _format_flag(", weak", fi->flags.weak),
        _format_flag(", self", fi->flags.self),        _format_flag(", naked", fi->flags.naked), _format_flag(", inline", fi->flags.inln),
        _format_flag("only_body", fi->flags.onlybody), _format_flag(", vname", fi->flags.vname)
    );
    return 1;
}

int DUMP_format_fntb(sym_table_t* smt, FILE* output) {
    map_foreach (func_info_t* fi, &smt->f.functb) {
        _format_funcinfo(fi, output);
    }

    return 1;
}

static int _format_secinfo(section_info_t* si, FILE* output) {
    const char* name = si->name ? si->name->body : "NULL";
    char vars[512]  = { 0 };
    char funcs[512] = { 0 };

    _format_id_list(&si->sorted.vars, vars, sizeof(vars));
    _format_id_list(&si->sorted.func, funcs, sizeof(funcs));

    fprintf(
        output,
        "sec name=%s align=%i vars=%s funcs=%s\n",
        name, si->align, vars, funcs
    );
    
    return 1;
}

int DUMP_format_sectb(sym_table_t* smt, FILE* output) {
    foreach (section_info_t* si, &smt->c.sorted.sectb) {
        _format_secinfo(si, output);
    }

    return 1;
}
