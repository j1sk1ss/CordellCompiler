#include <builder.h>

static int _print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("    ");
    if (node->token && node->token->t_type != SCOPE_TOKEN) {
        printf(
            "[%s] (t=%d, size=%i,%soff=%i, s_id=%i%s%s)\n", 
            node->token->value, node->token->t_type, node->info.size, 
            node->token->vinfo.ptr ? " ptr, " : " ", 
            node->info.offset, node->info.s_id,
            node->token->vinfo.ro ? ", ro" : "", node->token->vinfo.glob ? ", glob" : ""
        );
    }
    else if (node->token && node->token->t_type == SCOPE_TOKEN) {
        printf("{ scope, id=%i }\n", node->info.s_id);
    }
    else {
        printf("[ block ]\n");
    }
    
    ast_node_t* child = node->child;
    while (child) {
        _print_ast(child, depth + 1);
        child = child->sibling;
    }
    
    return 1;
}

static int _generate_raw_ast(builder_ctx_t* ctx) {
    int fd = open(ctx->files[ctx->fcount].path, O_RDONLY);
    if (fd < 0) return -1;

    token_t* tokens = TKN_tokenize(fd);
    if (!tokens) {
        close(fd);
        return -2;
    }

    int markup_res = MRKP_mnemonics(tokens);
    markup_res = MRKP_variables(tokens);
    if (!markup_res) {
        TKN_unload(tokens);
        close(fd);
        return -3;
    }

    ctx->files[ctx->fcount].syntax = STX_create_ctx();
    ctx->files[ctx->fcount].syntax->arrs = ART_create_ctx();
    ctx->files[ctx->fcount].syntax->vars = VRT_create_ctx();
    if (!STX_create(tokens, ctx->files[ctx->fcount].syntax, &ctx->p)) {
        ART_destroy_ctx(ctx->files[ctx->fcount].syntax->arrs);
        VRT_destroy_ctx(ctx->files[ctx->fcount].syntax->vars);
        STX_destroy_ctx(ctx->files[ctx->fcount].syntax);
        TKN_unload(tokens);
        close(fd);
        return -4;
    }

    if (!SMT_check(ctx->files[ctx->fcount].syntax->r)) {
        AST_unload(ctx->files[ctx->fcount].syntax->r);
        ART_destroy_ctx(ctx->files[ctx->fcount].syntax->arrs);
        VRT_destroy_ctx(ctx->files[ctx->fcount].syntax->vars);
        STX_destroy_ctx(ctx->files[ctx->fcount].syntax);
        TKN_unload(tokens);
        close(fd);
        return -5;
    }

    ctx->files[ctx->fcount].toks = tokens;
    ctx->fcount++;

    close(fd);
    return 1;
}

#define RESULT(code) code > 0 ? "OK" : "ERROR", code 
static int _compile_object(builder_ctx_t* ctx, char index) {
    int optres = OPT_strpack(ctx->files[index].syntax);
    print_log("String optimization of [%s]... [%s (%i)]", ctx->files[index].path, RESULT(optres));

    int is_fold_vars = 0;
    do {
        optres       = OPT_varinline(ctx->files[index].syntax);
        is_fold_vars = OPT_constfold(ctx->files[index].syntax);
    } while (is_fold_vars);
    print_log("Assign and muldiv optimization... [Code: %i/%i]", RESULT(optres), RESULT(is_fold_vars));
    
    optres = OPT_condunroll(ctx->files[index].syntax);
    print_log("Branches optimization... [%s (%i)]", RESULT(optres));

    optres = OPT_deadscope(ctx->files[index].syntax);
    print_log("Dead scope optimization... [%s (%i)]", RESULT(optres));

    optres = OPT_offrecalc(ctx->files[index].syntax);
    print_log("Offset recalculation and stack optimization... [%s (%i)]", RESULT(optres));

    if (ctx->prms.syntax) {
        _print_ast(ctx->files[index].syntax->r, 0);
    }
    
    char save_path[128] = { 0 };
    sprintf(save_path, "%s.asm", ctx->files[index].path);
    FILE* output = fopen(save_path, "w");
    gen_ctx_t* gctx = GEN_create_ctx();
    gctx->synt = ctx->files[index].syntax;
    GEN_generate(gctx, &ctx->g, output);
    GEN_destroy_ctx(gctx);
    fclose(output);

    char compile_command[128] = { 0 };
    snprintf(compile_command, 128, "%s -f%s %s -g -o %s.o", ctx->prms.asm_compiler, ctx->prms.arch, save_path, save_path);
    print_debug("COMPILING: system(%s)", compile_command);
    system(compile_command);

    AST_unload(ctx->files[index].syntax->r);
    TKN_unload(ctx->files[index].toks);
    ART_unload(ctx->files[index].syntax->arrs);
    ART_destroy_ctx(ctx->files[index].syntax->arrs);
    VRT_unload(ctx->files[index].syntax->vars);
    VRT_destroy_ctx(ctx->files[index].syntax->vars);
    STX_destroy_ctx(ctx->files[index].syntax);

    print_log("Optimization of [%s] complete", ctx->files[index].path);
    return 1;
}

int BLD_add_target(char* input, builder_ctx_t* ctx) {
    ctx->files[ctx->fcount].path = input;
    print_log("Raw AST generation for [%s]...", input);
    int res = _generate_raw_ast(ctx);
    print_log("AST-gen result [%s (%i)]", RESULT(res));
    return 1;
}

int BLD_build(builder_ctx_t* ctx) {
    if (!ctx->fcount) return 0;
    deadfunc_ctx_t dfctx = { .size = 0 };
    for (int i = 0; i < ctx->fcount; i++) {
        OPT_deadfunc_add(ctx->files[i].syntax, &dfctx);
    }

    OPT_deadfunc_clear(&dfctx);

    /* Production of .asm files with temporary saving in files directory */
    for (int i = 0; i < ctx->fcount; i++) {
        int res = _compile_object(ctx, i);
        if (!res) return res;
    }

    /* Linking output files */
    char link_command[256] = { 0 };
    snprintf(link_command, 256, "%s -m %s %s ", ctx->prms.linker, ctx->prms.linker_arch, ctx->prms.linker_flags);
    for (int i = ctx->fcount - 1; i >= 0; i--) {
        char object_path[128] = { 0 };
        snprintf(object_path, 128, " %s.asm.o", ctx->files[i].path);
        str_strcat(link_command, object_path);
    }

    str_strcat(link_command, " -o ");
    str_strcat(link_command, ctx->prms.save_path);
    print_debug("LINKING: system(%s)", link_command);
    system(link_command);

    /* Cleanup .asm and .o files */
    for (int i = ctx->fcount - 1; i >= 0; i--) {
        char delete_command[128] = { 0 };
        if (!ctx->prms.save_asm) snprintf(delete_command, 128, "rm %s.asm %s.asm.o", ctx->files[i].path, ctx->files[i].path);
        else snprintf(delete_command, 128, "rm %s.asm.o", ctx->files[i].path);
        print_debug("CLEANUP: system(%s)", delete_command);
        system(delete_command);
    }

    return 1;
}
