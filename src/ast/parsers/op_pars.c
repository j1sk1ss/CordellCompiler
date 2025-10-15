#include <ast/parsers/parser.h>

static ast_node_t* _parse_primary(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
static ast_node_t* _parse_binary_expression(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt, int min_priority) {
    ast_node_t* left = _parse_primary(it, ctx, smt);
    if (!left) return NULL;
    
    while ((token_t*)list_iter_current(it)) {
        int priority = TKN_token_priority((token_t*)list_iter_current(it));
        if (priority < min_priority || priority == -1) break;

        int next_min_priority = priority + 1;
        if (TKN_update_operator((token_t*)list_iter_current(it))) {
            next_min_priority = priority;
        }

        ast_node_t* op_node = AST_create_node((token_t*)list_iter_current(it));
        if (!op_node) {
            AST_unload(left);
            return NULL;
        }

        forward_token(it, 1);
        ast_node_t* right = _parse_binary_expression(it, ctx, smt, next_min_priority);
        if (!right) {
            print_error("AST error during expression parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
            AST_unload(left);
            return NULL;
        }

        AST_add_node(op_node, left);
        AST_add_node(op_node, right);
        left = op_node;
    }
    
    return left;
}

static ast_node_t* _parse_array_expression(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    if (node->token->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->token->value, STR_ARRAY_VALUE, &smt->s);
    }

    var_lookup(node, ctx, smt);
    
    forward_token(it, 1);
    if (((token_t*)list_iter_current(it))->t_type == OPEN_INDEX_TOKEN) {
        forward_token(it, 1);
        ast_node_t* offset_exp = cpl_parse_expression(it, ctx, smt);
        if (!offset_exp) {
            print_error("AST error during index parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, offset_exp);
        forward_token(it, 1);
    }

    if (TKN_isclose((token_t*)list_iter_current(it))) {
        return node;
    }

    ast_node_t* opnode = AST_create_node((token_t*)list_iter_current(it));
    if (!opnode) {
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* right = cpl_parse_expression(it, ctx, smt);
    if (!right) {
        print_error("AST error during right expression parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
        AST_unload(node);
        AST_unload(opnode);
        return NULL;
    }

    AST_add_node(opnode, node);
    AST_add_node(opnode, right);
    return opnode;
}

static ast_node_t* _parse_primary(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    if (((token_t*)list_iter_current(it))->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        ast_node_t* node = _parse_binary_expression(it, ctx, smt, 0);
        if (!node || !(token_t*)list_iter_current(it) || ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
            print_error("AST error during expression parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
            AST_unload(node);
            return NULL;
        }

        forward_token(it, 1);
        return node;
    }
    
    if (TKN_isptr((token_t*)list_iter_current(it)))                      return _parse_array_expression(it, ctx, smt);
    else if (((token_t*)list_iter_current(it))->t_type == CALL_TOKEN)    return cpl_parse_funccall(it, ctx, smt);
    else if (((token_t*)list_iter_current(it))->t_type == SYSCALL_TOKEN) return cpl_parse_syscall(it, ctx, smt);

    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    if (node->token->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->token->value, STR_INDEPENDENT, &smt->s);
    }

    var_lookup(node, ctx, smt);
    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_expression(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    return _parse_binary_expression(it, ctx, smt, 0);
}
