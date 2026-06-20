#include <ast/astgen.h>

int AST_init_ctx(ast_ctx_t* ctx) {
    str_memset(ctx, 0, sizeof(ast_ctx_t));
    stack_init(&ctx->scopes.stack);
    stack_init(&ctx->annots);
    ctx->t_id        = NO_SYMBOL_ID;
    ctx->carry.pfunc = NO_SYMBOL_ID;
    return 1;
}

static int _append_root_children(ast_node_t* dst, ast_node_t* src) {
    if (!dst || !src) return 0;
    while (src->c) {
        ast_node_t* child = src->c;
        src->c = child->siblings.n;
        child->siblings.n = child->siblings.t = NULL;
        AST_add_node(dst, child);
    }

    AST_unload(src);
    return 1;
}

int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(tkn, &it);

    if (!ctx->r) {
        stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
        ctx->r = cpl_parse_block(&it, ctx, smt, EOF_TOKEN);
        return ctx->r != NULL;
    }

    ast_node_t* root = cpl_parse_block(&it, ctx, smt, EOF_TOKEN);
    if (!root) return 0;
    return _append_root_children(ctx->r, root);
}

int AST_finalize_parse(ast_ctx_t* ctx, sym_table_t* smt) {
    if (!ctx || !ctx->r) return 0;
    if (ctx->scopes.stack.top >= 0) stack_pop(&ctx->scopes.stack, NULL);

    int entries = 0;
    map_foreach(func_info_t* fi, &smt->f.functb) {
        if (fi->flags.local) continue;
        if (fi->flags.entry) entries++;
    }

    if (!entries)         print_warn("The 'start' function isn't found!");
    else if (entries > 1) print_error("There is more than 1 entry point in the code!");

    queue_t methods;
    queue_init(&methods);
    AST_DVRT_move_container_functions(ctx->r, &methods);
    
    ast_node_t* method;
    while (queue_pop(&methods, (void**)&method)) {
        AST_insert_node(ctx->r, method);
    }

    queue_free(&methods);

    devirt_ctx_t dctx;
    AST_DVRT_init_ctx(&dctx);

    AST_DVRT_find_templates(ctx->r, smt, &dctx);
    int generated = 0;
    do {
        generated = 0;
        AST_DVRT_resolve_calls(ctx->r, smt, &dctx);

        ast_node_t* devirt;
        while (AST_DVRT_pop_implementation(smt, &dctx, (ast_node_t**)&devirt)) {
            if (devirt) {
                AST_add_node(ctx->r, devirt);
                generated = 1;
            }
        }
    } while (generated);

    AST_DVRT_unload_ctx(&dctx);
    return 1;
}

int AST_unload_ctx(ast_ctx_t* ctx) {
    AST_unload(ctx->r);
    stack_free(&ctx->scopes.stack);
    stack_free_force_op(&ctx->annots, (int (*)(void*))ANNOT_destroy_annotation);
    return 1;
}
