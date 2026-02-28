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
    
    int has_entry = 0;
    func_info_t* last = NULL;
    map_foreach(func_info_t* fi, &smt->f.functb) {
        if (fi->flags.local) continue;
        last = fi;
        if (fi->flags.entry) {
            has_entry = 1;
            break;
        }
    }

    string_t* entry_name = create_string(CONF_get_entry_name());
    if (
        last && 
        !has_entry && 
        FNTB_update_info(last->id, FIELD_NO_CHANGE, 1, FIELD_NO_CHANGE, NULL, NULL, &smt->f) &&
        FNTB_rename_func(last->id, entry_name, &smt->f)
    ) print_warn("The 'start' function isn't found! Default entry set to the '%s'!", last->name->body);
    destroy_string(entry_name);
    return 1;
}

int AST_unload_ctx(ast_ctx_t* ctx) {
    AST_unload(ctx->r);
    stack_free(&ctx->scopes.stack);
    stack_free_force_op(&ctx->annots, (int (*)(void*))ANNOT_destroy_annotation);
    return 1;
}
