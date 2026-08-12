#include <symtab/dump.h>
#include <string.h>

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

#define SECTION_ALIGN_WIDTH 5
#define SECTION_VARS_WIDTH  64

static inline void _format_secinfo_close_line(FILE* output, int len) {
    for (int i = 0; i < MAX(0, SECTION_VARS_WIDTH - len); i++) fputc(' ', output);
    fprintf(output, " |\n");
}

static int _format_secinfo(section_info_t* si, sym_table_t* smt, FILE* output, int name_width) {
    (void)smt;

    const char* name = si->name ? si->name->body : "NULL";
    int vars_count = list_size(&si->sorted.vars);
    int vars_len = 0;

    fprintf(output, "| %-*s | %-*i | ", name_width, name, SECTION_ALIGN_WIDTH, si->align);
    vars_len += fprintf(output, "{");

    if (!vars_count) {
        vars_len += fprintf(output, "}");
        _format_secinfo_close_line(output, vars_len);
        return 1;
    }

    int index = 0;
    foreach (void* raw_id, &si->sorted.vars) {
        symbol_id_t id = (symbol_id_t)raw_id;
        char item[64] = { 0 };
        const char* tail = ++index == vars_count ? "}" : ", ";
        int item_len = snprintf(item, sizeof(item), "%li%s", id, tail);

        if (vars_len > 1 && vars_len + item_len > SECTION_VARS_WIDTH) {
            _format_secinfo_close_line(output, vars_len);
            fprintf(output, "| %-*s | %-*s | ", name_width, "", SECTION_ALIGN_WIDTH, "");
            vars_len = 1;
            fputc(' ', output);
        }

        fprintf(output, "%s", item);
        vars_len += item_len;
    }

    _format_secinfo_close_line(output, vars_len);
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
        "| %-*s | %-*s | %-*s |\n",
        name_width, "section", SECTION_ALIGN_WIDTH, "align", SECTION_VARS_WIDTH, "vars"
    );

    foreach (section_info_t* si, &smt->c.sorted.sectb) {
        _format_secinfo(si, smt, output, name_width);
    }

    return 1;
}
