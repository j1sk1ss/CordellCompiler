#include <builder.h>

static int _print_parse_tree(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("\t");
    if (node->token) printf(
        "[%s] (t=%d, size=%i, is_ptr=%i, off=%i, ro=%i glob=%i)\n", 
        (char*)node->token->value, node->token->t_type, node->variable_size, node->token->ptr, 
        node->variable_offset, node->token->ro, node->token->glob
    );
    else printf("{ scope }\n");
    
    ast_node_t* child = node->first_child;
    while (child) {
        _print_parse_tree(child, depth + 1);
        child = child->next_sibling;
    }
    
    return 1;
}

static params_t _params = { };
static object_t _files[MAX_FILES];
static int _current_file = 0;

/*
-1 - Can't open file.
-2 - Tokenize error.
-3 - Markup error.
-4 - Semantic check failed.
1 - Success.
*/
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

    obj->syntax->arrs = ARM_create_ctx();
    obj->syntax->vars = VRM_create_ctx();
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
static int _add_object(char* path) {
    _files[_current_file].path = path;
    print_log("Raw AST generation for [%s]...", path);

    int res_code = _generate_raw_ast(&_files[_current_file++]);
    print_log("AST-gen result [%s (%i)]", RESULT(res_code));
    return 1;
}

static int _compile_object(object_t* obj) {
    int str_opt_res = OPT_strpack(obj->syntax);
    print_log("String optimization of [%s]... [%s (%i)]", obj->path, RESULT(str_opt_res));

    int assign_opt_res = 0;
    int is_fold_vars = 0;
    do {
        assign_opt_res = assign_optimization(obj->syntax);
        is_fold_vars = OPT_muldiv(obj->syntax);
    } while (is_fold_vars);
    print_log("Assign and muldiv optimization... [Code: %i/%i]", assign_opt_res, is_fold_vars);
    
    int stmt_opt_res = stmt_optimization(obj->syntax);
    print_log("Statement optimization... [%s (%i)]", RESULT(stmt_opt_res));

    int varuse_opt_res = OPT_varuse(obj->syntax);
    print_log("Var usage optimization... [%s (%i)]", RESULT(varuse_opt_res));

    int offset_recalc_res = OPT_offrecalc(obj->syntax);
    print_log("Offset recalculation... [%s (%i)]", RESULT(offset_recalc_res));

    char save_path[128] = { 0 };
    sprintf(save_path, "%s.asm", obj->path);

    FILE* output = fopen(save_path, "w");
    if (_params.syntax) _print_parse_tree(obj->syntax->r, 0);
    
    gen_ctx_t* gctx = GEN_create_ctx();
    gctx->synt = obj->syntax;

    GEN_generate(gctx, output);
    fclose(output);

    char compile_command[128] = { 0 };
    sprintf(compile_command, "%s -f%s %s -g -o %s.o", _params.asm_compiler, _params.arch, save_path, save_path);

    print_debug("COMPILING: system(%s)", compile_command);
    system(compile_command);

    AST_unload(obj->syntax->r);
    TKN_unload(obj->toks);
    ARM_unload(obj->syntax->arrs);
    ARM_destroy_ctx(obj->syntax->arrs);
    VRM_unload(obj->syntax->vars);
    VRM_destroy_ctx(obj->syntax->vars);
    GEN_destroy_ctx(gctx);

    print_log("Optimization of [%s] complete", obj->path);
    return 1;
}

int builder_add_file(char* input) {
    return _add_object(input);
}

int builder_compile() {
    if (!_current_file) return 0;
    for (int i = 0; i < _current_file; i++) funcopt_add_ast(_files[i].syntax);
    func_optimization();

    /*
    Production of .asm files with temporary saving in files directory.
    */
    for (int i = _current_file - 1; i >= 0; i--) {
        int res = _compile_object(&_files[i]);
        if (!res) return res;
    }

    /*
    Linking output files.
    */
    char link_command[256] = { 0 };
    sprintf(link_command, "%s -m %s %s ", _params.linker, _params.linker_arch, _params.linker_flags);

    for (int i = _current_file - 1; i >= 0; i--) {
        char object_path[128] = { 0 };
        sprintf(object_path, " %s.asm.o", _files[i].path);
        str_strcat(link_command, object_path);
    }

    str_strcat(link_command, " -o ");
    str_strcat(link_command, _params.save_path);

    print_debug("LINKING: system(%s)", link_command);
    system(link_command);

    /*
    Cleanup .asm and .o files.
    */
    for (int i = _current_file - 1; i >= 0; i--) {
        char delete_command[128] = { 0 };
        if (!_params.save_asm) sprintf(delete_command, "rm %s.asm %s.asm.o", _files[i].path, _files[i].path);
        else sprintf(delete_command, "rm %s.asm.o", _files[i].path);

        print_debug("CLEANUP: system(%s)", delete_command);
        system(delete_command);
    }

    return 1;
}

int set_params(params_t* params) {
    str_memcpy(&_params, params, sizeof(params_t));
    return 1;
}
