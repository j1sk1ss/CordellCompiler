#include <ast/astgen.h>

int AST_init_ctx(ast_ctx_t* ctx, const char* fentry) {
    str_memset(ctx, 0, sizeof(ast_ctx_t));
    ctx->fentry = fentry ? fentry : "_main";
    stack_init(&ctx->scopes.stack);
    return 1;
}

int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(tkn, &it);

    ctx->r = cpl_parse_scope(&it, ctx, smt, 0);
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

    if (last && !has_entry && FNTB_update_info(last->id, -1, 1, -1, NULL, NULL, &smt->f)) {
        print_warn("The 'start' function isn't found! Default entry set to the '%s'!", last->name->body);
    }

    return 1;
}

int AST_unload_ctx(ast_ctx_t* ctx) {
    AST_unload(ctx->r);
    stack_free(&ctx->scopes.stack);
    return 1;
}
