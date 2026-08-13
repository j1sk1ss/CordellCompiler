#include <symtab/dump.h>

static char* _format_type(symbol_id_t id, typetab_ctx_t* ctx) {
    type_info_t ti;
    if (TPTB_get_info_id(id, &ti, ctx)) {
        return ti.name->body;
    }

    return "NULL";
}

static int _format_varinfo(variable_info_t* vi, sym_table_t* smt, char* output) {
    sprintf(
        output,
        // 5id       20t    20n     5algn  5par    5scp   5ptr    ro    glb    ext
        "| %-5li | %-20s | %-20s | %-5i | %-5li | %-5li | %-5i | %-3s | %-3s | %-3s |",
        vi->v_id, _format_type(vi->t_id, &smt->t), vi->name->body, vi->vmi.align,
        vi->p_id, vi->s_id, vi->vfs.ptr, 
        vi->vfs.ro ? "+" : " ", vi->vfs.glob ? "+" : " ", vi->vfs.ext ? "+" : " "
    );
    return 1;
}

int DUMP_format_vartb(sym_table_t* smt, FILE* output) {
    fprintf(
        output,
        "| %-5s | %-20s | %-20s | %-5s | %-5s | %-5s | %-5s | %-3s | %-3s | %-3s |\n",
        "id", "type", "name", "align", "par", "scope", "ptr", "ro", "glb", "ext"
    );
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        char line[512] = { 0 };
        _format_varinfo(vi, smt, line);
        fprintf(output, "%s\n", line);
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
        "| %-5li | %-20s | %-20s | %-5li | %-36s | %-16s | %-18s | %-18s | %-18s |"
        " %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s |\n",
        fi->id, _format_str(fi->name), _format_str(fi->virt), fi->s_id,
        args, rtype, locals, generic_types, resolutions,
        fi->flags.external ? "+" : " ", fi->flags.global   ? "+" : " ", fi->flags.entry    ? "+" : " ",
        fi->flags.used     ? "+" : " ", fi->flags.local    ? "+" : " ", fi->flags.vargs    ? "+" : " ",
        fi->flags.generic  ? "+" : " ", fi->flags.abi      ? "+" : " ", fi->flags.weak     ? "+" : " ",
        fi->flags.self     ? "+" : " ", fi->flags.naked    ? "+" : " ", fi->flags.inln     ? "+" : " ",
        fi->flags.onlybody ? "+" : " ", fi->flags.vname    ? "+" : " "
    );
    return 1;
}

int DUMP_format_fntb(sym_table_t* smt, FILE* output) {
    fprintf(
        output,
        "| %-5s | %-20s | %-20s | %-5s | %-36s | %-16s | %-18s | %-18s | %-18s |"
        " %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s | %-3s |\n",
        "id", "name", "virt", "scope", "args", "ret", "locals", "generic types", "resolutions",
        "ext", "glb", "ent", "use", "loc", "var", "gen", "abi", "wek", "slf", "nkd", "inl", "bod", "vnm"
    );

    map_foreach (func_info_t* fi, &smt->f.functb) {
        _format_funcinfo(fi, output);
    }

    return 1;
}

#define SECTION_ALIGN_WIDTH 5
#define SECTION_VARS_WIDTH  64
#define SECTION_FUNCS_WIDTH 64

static int _format_secinfo(section_info_t* si, FILE* output, int name_width) {
    const char* name = si->name ? si->name->body : "NULL";
    char vars[512]  = { 0 };
    char funcs[512] = { 0 };

    _format_id_list(&si->sorted.vars, vars, sizeof(vars));
    _format_id_list(&si->sorted.func, funcs, sizeof(funcs));

    fprintf(
        output,
        "| %-*s | %-*i | %-*s | %-*s |\n",
        name_width, name, SECTION_ALIGN_WIDTH, si->align,
        SECTION_VARS_WIDTH, vars, SECTION_FUNCS_WIDTH, funcs
    );
    return 1;
}

int DUMP_format_sectb(sym_table_t* smt, FILE* output) {
    int name_width = 20;
    foreach (section_info_t* si, &smt->c.sorted.sectb) {
        if (!si->name || !si->name->body) continue;
        name_width = MAX(name_width, (int)strlen(si->name->body));
    }

    fprintf(
        output,
        "| %-*s | %-*s | %-*s | %-*s |\n",
        name_width, "section", SECTION_ALIGN_WIDTH, "align", SECTION_VARS_WIDTH, "vars", SECTION_FUNCS_WIDTH, "funcs"
    );

    foreach (section_info_t* si, &smt->c.sorted.sectb) {
        _format_secinfo(si, output, name_width);
    }

    return 1;
}
