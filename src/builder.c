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

static params_t _params;
static object_t _files[MAX_FILES];
static int _current_file = 0;

static int _generate_raw_ast(object_t* obj) {
    int fd = open(obj->path, O_RDONLY);
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

    obj->syntax->arrs = ART_create_ctx();
    obj->syntax->vars = VRT_create_ctx();
    STX_create(tokens, obj->syntax);
    if (!SMT_check(obj->syntax->r)) {
        AST_unload(obj->syntax->r);
        TKN_unload(tokens);
        close(fd);
        return -4;
    }

    obj->toks = tokens;
    close(fd);
    return 1;
}

#define RESULT(code) code > 0 ? "OK" : "ERROR", code 
static int _compile_object(object_t* obj) {
    int optres = OPT_strpack(obj->syntax);
    print_log("String optimization of [%s]... [%s (%i)]", obj->path, RESULT(optres));

    int is_fold_vars = 0;
    do {
        optres       = OPT_varinline(obj->syntax);
        is_fold_vars = OPT_constfold(obj->syntax);
    } while (is_fold_vars);
    print_log("Assign and muldiv optimization... [Code: %i/%i]", RESULT(optres), RESULT(is_fold_vars));
    
    optres = OPT_condunroll(obj->syntax);
    print_log("Branches optimization... [%s (%i)]", RESULT(optres));

    optres = OPT_deadscope(obj->syntax);
    print_log("Dead scope optimization... [%s (%i)]", RESULT(optres));

    optres = OPT_offrecalc(obj->syntax);
    print_log("Offset recalculation and stack optimization... [%s (%i)]", RESULT(optres));

    if (_params.syntax) {
        _print_ast(obj->syntax->r, 0);
    }
    
    char save_path[128] = { 0 };
    sprintf(save_path, "%s.asm", obj->path);
    FILE* output = fopen(save_path, "w");
    gen_ctx_t* gctx = GEN_create_ctx();
    gctx->synt = obj->syntax;
    GEN_generate(gctx, output);
    fclose(output);

    char compile_command[128] = { 0 };
    snprintf(compile_command, 128, "%s -f%s %s -g -o %s.o", _params.asm_compiler, _params.arch, save_path, save_path);
    print_debug("COMPILING: system(%s)", compile_command);
    system(compile_command);

    AST_unload(obj->syntax->r);
    TKN_unload(obj->toks);
    ART_unload(obj->syntax->arrs);
    ART_destroy_ctx(obj->syntax->arrs);
    VRT_unload(obj->syntax->vars);
    VRT_destroy_ctx(obj->syntax->vars);
    GEN_destroy_ctx(gctx);

    print_log("Optimization of [%s] complete", obj->path);
    return 1;
}

int BLD_add_target(char* input, syntax_ctx_t* ctx) {
    _files[_current_file].path = input;
    str_memcpy(_files[_current_file].syntax, ctx, sizeof(syntax_ctx_t));
    print_log("Raw AST generation for [%s]...", input);
    int res_code = _generate_raw_ast(&_files[_current_file++]);
    print_log("AST-gen result [%s (%i)]", RESULT(res_code));
    return 1;
}

int BLD_build() {
    if (!_current_file) return 0;
    deadfunc_ctx_t dfctx = { .size = 0 };
    for (int i = 0; i < _current_file; i++) {
        OPT_deadfunc_add(_files[i].syntax, &dfctx);
    }

    OPT_deadfunc_clear(&dfctx);

    /* Production of .asm files with temporary saving in files directory */
    for (int i = _current_file - 1; i >= 0; i--) {
        int res = _compile_object(&_files[i]);
        if (!res) return res;
    }

    /* Linking output files */
    char link_command[256] = { 0 };
    snprintf(link_command, 256, "%s -m %s %s ", _params.linker, _params.linker_arch, _params.linker_flags);
    for (int i = _current_file - 1; i >= 0; i--) {
        char object_path[128] = { 0 };
        snprintf(object_path, 128, " %s.asm.o", _files[i].path);
        str_strcat(link_command, object_path);
    }

    str_strcat(link_command, " -o ");
    str_strcat(link_command, _params.save_path);
    print_debug("LINKING: system(%s)", link_command);
    system(link_command);

    /* Cleanup .asm and .o files */
    for (int i = _current_file - 1; i >= 0; i--) {
        char delete_command[128] = { 0 };
        if (!_params.save_asm) snprintf(delete_command, 128, "rm %s.asm %s.asm.o", _files[i].path, _files[i].path);
        else snprintf(delete_command, 128, "rm %s.asm.o", _files[i].path);
        print_debug("CLEANUP: system(%s)", delete_command);
        system(delete_command);
    }

    return 1;
}

int BLD_set_params(params_t* params) {
    str_memcpy(&_params, params, sizeof(params_t));
    return 1;
}
