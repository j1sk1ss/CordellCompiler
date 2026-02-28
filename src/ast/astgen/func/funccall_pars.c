#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_call_arguments(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    int* args = (int*)carry;
    ast_node_t* node = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        if (arg) AST_add_node(node, arg);
        else { 
            PARSE_ERROR("Error during the call argument parsing! <arg>!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL; 
        }

        if (args) {
            (*args)++;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    } 
    return node;
}

ast_node_t* cpl_parse_funccall(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the function call!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    /* If there is no brackets in function call, this means, this is a call address instead.
       We mark the node token as a call address and pass it further. */
    int args_count = 0;
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) node->t->t_type = CALL_ADDR_TOKEN;
    else {
        forward_token(it, 1);
        ast_node_t* args = cpl_parse_call_arguments(it, ctx, smt, (long)&args_count);
        if (args) AST_add_node(node, args);
        else {
            PARSE_ERROR("Function arguments parse error!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        forward_token(it, 1);
    }
    
    /* The default argument is allowed only for non-polymorphic functions.
       That's why we're using the list's head (we don't care about other functions) */
    func_info_t fi;
    for (int s = ctx->scopes.stack.top; s >= 0; s--) { 
        short sid = (short)((long)ctx->scopes.stack.data[s].d);
        if (FNTB_get_info(node->t->body, sid, &fi, &smt->f)) {
            node->sinfo.v_id = fi.id;
            if (node->t->t_type != CALL_ADDR_TOKEN) {
                fn_iterate_args (&fi) {
                    if (
                        args_count-- > 0 ||              /* Ignore already passed arguments             */
                        (!arg->c || !arg->c->siblings.n) /* If this argument doesn't have a declaration */
                    ) continue;
                    AST_add_node(node->c, AST_copy_node(arg->c->siblings.n, 0, 0, 1));
                }
            }
        }
    }

    return node;
}
