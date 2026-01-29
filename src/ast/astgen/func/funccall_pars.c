#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_funccall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the function call!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type != OPEN_BRACKET_TOKEN) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token in the function call!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    int args = 0;
    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        if (arg) AST_add_node(node, arg);
        else { 
            PARSE_ERROR("Error during the call argument parsing! <name>(<arg>)!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL; 
        }

        args++;
    }

    /* The default argument is allowed only for non-polymorphic functions.
       That's why we're using the list's head (we don't care about other functions) */
    func_info_t fi;
    if (!FNTB_get_info(node->t->body, &fi, &smt->f)) {
        PARSE_ERROR("Can't find a function by the provided name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    node->sinfo.v_id = fi.id;
    fn_iterate_args (&fi) {
        if (
            args-- > 0 ||       /* Ignore already passed arguments             */
            !arg->c->siblings.n /* If this argument doesn't have a declaration */
        ) continue;
        AST_add_node(node, AST_copy_node(arg->c->siblings.n, 0, 0, 1));
    }

    forward_token(it, 1);
    return node;
}
