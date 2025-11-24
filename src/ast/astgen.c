#include <ast/astgen.h>

int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(tkn, &it);
    ctx->r = cpl_parse_block(&it, ctx, smt, CLOSE_BLOCK_TOKEN);

    int has_entry = 0;
    map_iter_t mit;
    func_info_t* fi;
    map_iter_init(&smt->f.functb, &mit);
    while (map_iter_next(&mit, (void**)&fi)) {
        if (fi->entry) {
            has_entry = 1;
            break;
        }
    }

    if (!has_entry && FNTB_update_info(fi->id, fi->used, 1, fi->args, fi->rtype, &smt->f)) {
        print_warn("start function not found! Default start set to %s", fi->name);
    }

    return ctx->r != NULL;
}
