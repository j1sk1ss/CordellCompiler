#include <builder.h>
#include <ast/dump.h>

int BUILD_emit_ast(ast_ctx_t* ctx, const options_t* options) {
    if (!ctx || !options) return 0;
    if (!options->config.emit_ast) return 1;

    const char* output_path = BUILD_output_path_or_default(options->locations.ast_output, "output.ast");
    FILE* output = fopen(output_path, "w");
    if (!output) {
        fprintf(stderr, "Can't open AST output file %s: %s\n", output_path, strerror(errno));
        return 0;
    }

    DUMP_format_astctx(ctx, output);
    fclose(output);
    return 1;
}

int BUILD_emit_hir(hir_ctx_t* hctx, cfg_ctx_t* cctx, sym_table_t* smt, const options_t* options) {
    if (!hctx || !cctx || !smt || !options) return 0;

    if (options->config.emit_ir) {
        const char* output_path = BUILD_output_path_or_default(options->locations.ir_output, "output.ir");
        FILE* output = fopen(output_path, "w");
        if (!output) {
            fprintf(stderr, "Can't open HIR output file %s: %s\n", output_path, strerror(errno));
            return 0;
        }

        DUMP_format_hirctx(hctx, smt, 0, 0, output);
        fclose(output);
    }

    if (options->config.emit_hir_cfg) {
        const char* output_path = "output.dot";
        FILE* output = fopen(output_path, "w");
        if (!output) {
            fprintf(stderr, "Can't open HIR CFG output file %s: %s\n", output_path, strerror(errno));
            return 0;
        }

        DUMP_format_hir_cfg(cctx, smt, options->locations.hir_cfg_name, output);
        fclose(output);
    }

    return 1;
}

int BUILD_emit_lir(lir_ctx_t* lctx, cfg_ctx_t* cctx, sym_table_t* smt, const options_t* options) {
    if (!lctx || !cctx || !smt || !options) return 0;

    if (options->config.emit_lir) {
        const char* output_path = BUILD_output_path_or_default(options->locations.lir_output, "output.lir");
        FILE* output = fopen(output_path, "w");
        if (!output) {
            fprintf(stderr, "Can't open LIR output file %s: %s\n", output_path, strerror(errno));
            return 0;
        }

        DUMP_format_lirctx(lctx, smt, 0, 0, output);
        fclose(output);
    }

    if (options->config.emit_lir_cfg) {
        const char* output_path = "output.dot";
        FILE* output = fopen(output_path, "w");
        if (!output) {
            fprintf(stderr, "Can't open LIR CFG output file %s: %s\n", output_path, strerror(errno));
            return 0;
        }

        DUMP_format_lir_cfg(cctx, smt, options->locations.lir_cfg_name, output);
        fclose(output);
    }

    return 1;
}

static const char* _symtab_output_path(const char* type) {
    if (!strcmp(type, "var")) return "output.var.symtab";
    if (!strcmp(type, "fn"))  return "output.fn.symtab";
    if (!strcmp(type, "sec")) return "output.sec.symtab";
    return NULL;
}

static int _emit_symtab(sym_table_t* smt, const char* type) {
    const char* output_path = _symtab_output_path(type);
    if (!output_path) return 0;

    FILE* output = fopen(output_path, "w");
    if (!output) {
        fprintf(stderr, "Can't open symtab output file %s: %s\n", output_path, strerror(errno));
        return 0;
    }

    int ok = 0;
    if (!strcmp(type, "var"))      ok = DUMP_format_vartb(smt, output);
    else if (!strcmp(type, "fn"))  ok = DUMP_format_fntb(smt, output);
    else if (!strcmp(type, "sec")) ok = DUMP_format_sectb(smt, output);

    fclose(output);
    return ok;
}

int BUILD_emit_requested_symtabs(sym_table_t* smt, list_t* types) {
    foreach (char* type, types) {
        if (!_emit_symtab(smt, type)) return 0;
    }

    return 1;
}
