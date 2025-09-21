#include <ast/parsers/parser.h>

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt);
static ast_node_t* _parse_binary_expression(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt, int min_priority) {
    ast_node_t* left = _parse_primary(curr, ctx, smt);
    if (!left) return NULL;
    
    while (*curr) {
        int priority = VRS_token_priority(*curr);
        if (priority < min_priority || priority == -1) break;

        int next_min_priority = priority + 1;
        if (VRS_update_operator(*curr)) {
            next_min_priority = priority;
        }

        ast_node_t* op_node = AST_create_node(*curr);
        if (!op_node) {
            AST_unload(left);
            return NULL;
        }

        forward_token(curr, 1);
        ast_node_t* right = _parse_binary_expression(curr, ctx, smt, next_min_priority);
        if (!right) {
            print_error("AST error during expression parsing! line=%i", (*curr)->lnum);
            AST_unload(left);
            return NULL;
        }

        AST_add_node(op_node, left);
        AST_add_node(op_node, right);
        left = op_node;
    }
    
    return left;
}

static ast_node_t* _parse_array_expression(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    if (node->token->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->token->value, &smt->s);
    }

    var_lookup(node, ctx, smt);
    
    forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_INDEX_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* offset_exp = cpl_parse_expression(curr, ctx, smt);
        if (!offset_exp) {
            print_error("AST error during index parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, offset_exp);
        forward_token(curr, 1);
    }

    if (VRS_isclose(*curr)) {
        return node;
    }

    ast_node_t* opnode = AST_create_node(*curr);
    if (!opnode) {
        AST_unload(node);
        return NULL;
    }

    forward_token(curr, 1);
    ast_node_t* right = cpl_parse_expression(curr, ctx, smt);
    if (!right) {
        print_error("AST error during right expression parsing! line=%i", (*curr)->lnum);
        AST_unload(node);
        AST_unload(opnode);
        return NULL;
    }

    AST_add_node(opnode, node);
    AST_add_node(opnode, right);
    return opnode;
}

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* node = _parse_binary_expression(curr, ctx, smt, 0);
        if (!node || !*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            print_error("AST error during expression parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }

        forward_token(curr, 1);
        return node;
    }
    
    if (VRS_isptr(*curr))                      return _parse_array_expression(curr, ctx, smt);
    else if ((*curr)->t_type == CALL_TOKEN)    return cpl_parse_funccall(curr, ctx, smt);
    else if ((*curr)->t_type == SYSCALL_TOKEN) return cpl_parse_syscall(curr, ctx, smt);

    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    if (node->token->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->token->value, &smt->s);
    }

    var_lookup(node, ctx, smt);
    forward_token(curr, 1);
    return node;
}

ast_node_t* cpl_parse_expression(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    return _parse_binary_expression(curr, ctx, smt, 0);
}
