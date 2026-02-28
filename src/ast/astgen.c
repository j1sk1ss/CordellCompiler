#include <ast/astgen.h>

int AST_init_ctx(ast_ctx_t* ctx) {
    str_memset(ctx, 0, sizeof(ast_ctx_t));
    stack_init(&ctx->scopes.stack);
    stack_init(&ctx->annots);
    return 1;
}

int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(tkn, &it);

    ctx->r = cpl_parse_scope(&it, ctx, smt, 1);
    if (!ctx->r) return 0;
    
    int entries = 0;
    map_foreach(func_info_t* fi, &smt->f.functb) {
        if (fi->flags.local) continue;
        if (fi->flags.entry) {
            string_t* entry_name = create_string(CONF_get_entry_name()); 
            FNTB_rename_func(fi->id, entry_name, &smt->f);
            destroy_string(entry_name);
            entries++;
        }
    }

    if (!entries)         print_warn("The 'start' function isn't found!");
    else if (entries > 1) print_error("There is more than 1 entry point in code!");
    return entries <= 1;
}

int AST_unload_ctx(ast_ctx_t* ctx) {
    AST_unload(ctx->r);
    stack_free(&ctx->scopes.stack);
    stack_free_force_op(&ctx->annots, (int (*)(void*))ANNOT_destroy_annotation);
    return 1;
}
