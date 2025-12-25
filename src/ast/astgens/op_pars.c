#include <ast/astgens/astgens.h>

static ast_node_t* _parse_primary(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);
static ast_node_t* _parse_binary_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int min_priority) {
    ast_node_t* left = _parse_primary(it, ctx, smt);
    if (!left) return NULL;
    
    while (CURRENT_TOKEN) {
        int priority = TKN_token_priority(CURRENT_TOKEN);
        if (priority < min_priority || priority == -1) break;

        int next_min_priority = priority + 1;
        if (TKN_update_operator(CURRENT_TOKEN)) {
            next_min_priority = priority;
        }

        ast_node_t* op_node = AST_create_node(CURRENT_TOKEN);
        if (!op_node) {
            print_error("AST_create_node error!");
            AST_unload(left);
            return NULL;
        }

        forward_token(it, 1);
        ast_node_t* right = _parse_binary_expression(it, ctx, smt, next_min_priority);
        if (!right) {
            print_error("AST error during expression parsing! line=%i", CURRENT_TOKEN->lnum);
            AST_unload(left);
            return NULL;
        }

        AST_add_node(op_node, left);
        AST_add_node(op_node, right);
        left = op_node;
    }
    
    return left;
}

static ast_node_t* _parse_array_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }

    if (node->t->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->t->body, STR_ARRAY_VALUE, &smt->s);
    }

    var_lookup(node, ctx, smt);
    
    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type == OPEN_INDEX_TOKEN) {
        forward_token(it, 1);
        ast_node_t* offset_exp = cpl_parse_expression(it, ctx, smt);
        if (!offset_exp) {
            print_error("AST error during index parsing! line=%i", CURRENT_TOKEN->lnum);
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, offset_exp);
        forward_token(it, 1);
    }

    if (TKN_isclose(CURRENT_TOKEN)) {
        return node;
    }

    ast_node_t* opnode = AST_create_node(CURRENT_TOKEN);
    if (!opnode) {
        print_error("AST_create_node error!");
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* right = cpl_parse_expression(it, ctx, smt);
    if (!right) {
        print_error("AST error during right expression parsing! line=%i", CURRENT_TOKEN->lnum);
        AST_unload(node);
        AST_unload(opnode);
        return NULL;
    }

    AST_add_node(opnode, node);
    AST_add_node(opnode, right);
    return opnode;
}

static ast_node_t* _parse_primary(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    if (CURRENT_TOKEN->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        ast_node_t* node = _parse_binary_expression(it, ctx, smt, 0);
        if (!node || !CURRENT_TOKEN || CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
            print_error("AST error during expression parsing! line=%i", CURRENT_TOKEN->lnum);
            AST_unload(node);
            return NULL;
        }

        forward_token(it, 1);
        return node;
    }
    
    if (TKN_isptr(CURRENT_TOKEN))                    return _parse_array_expression(it, ctx, smt);
    else if (CURRENT_TOKEN->t_type == CALL_TOKEN)    return cpl_parse_funccall(it, ctx, smt);
    else if (CURRENT_TOKEN->t_type == SYSCALL_TOKEN) return cpl_parse_syscall(it, ctx, smt);

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) return NULL;
    if (node->t->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->t->body, STR_INDEPENDENT, &smt->s);
    }

    var_lookup(node, ctx, smt);
    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    return _parse_binary_expression(it, ctx, smt, 0);
}
