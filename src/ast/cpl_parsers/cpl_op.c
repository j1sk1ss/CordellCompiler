#include <cpl_parser.h>

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx);
static ast_node_t* _parse_binary_expression(token_t** curr, syntax_ctx_t* ctx, int min_priority) {
    ast_node_t* left = _parse_primary(curr, ctx);
    if (!left) return NULL;

    while (*curr) {
        int priority = VRS_token_priority(*curr);
        if (priority < min_priority || priority == -1) break;

        token_t* op_token = *curr;
        forward_token(curr, 1);

        int next_min_priority = priority + 1;
        if ((*curr)->t_type == ASSIGN_TOKEN) {
            next_min_priority = priority;
        }

        ast_node_t* right = _parse_binary_expression(curr, ctx, next_min_priority);
        if (!right) {
            AST_unload(left);
            return NULL;
        }

        ast_node_t* op_node = AST_create_node(op_token);
        if (!op_node) {
            AST_unload(left);
            AST_unload(right);
            return NULL;
        }

        AST_add_node(op_node, left);
        AST_add_node(op_node, right);
        left = op_node;
    }

    return left;
}

static ast_node_t* _parse_array_expression(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    STX_var_lookup(node, ctx);
    
    forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_INDEX_TOKEN) { /* Indexing? */
        token_t* offset_token = (*curr)->next;
        ast_node_t* offset_exp = ctx->expr(&offset_token, ctx);
        if (!offset_exp) {
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, offset_exp);
        forward_token(curr, 1);
    }

    if (VRS_isclose(*curr)) { /* End of expression? */
        return node;
    }

    ast_node_t* opnode = AST_create_node(*curr);
    if (!opnode) {
        AST_unload(node);
        return NULL;
    }

    forward_token(curr, 1);
    ast_node_t* right = cpl_parse_expression(curr, ctx);
    if (!right) {
        AST_unload(node);
        AST_unload(opnode);
        return NULL;
    }

    AST_add_node(opnode, node);
    AST_add_node(opnode, right);
    return opnode;
}

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx) {
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* node = _parse_binary_expression(curr, ctx, 0);
        if (!node || !*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            AST_unload(node);
            return NULL;
        }

        forward_token(curr, 1);
        return node;
    }
    
    if (VRS_isptr(*curr))                      return _parse_array_expression(curr, ctx);
    else if ((*curr)->t_type == CALL_TOKEN)    return ctx->funccall(curr, ctx);
    else if ((*curr)->t_type == SYSCALL_TOKEN) return ctx->syscall(curr, ctx);

    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    STX_var_lookup(node, ctx);
    forward_token(curr, 1);
    return node;
}

ast_node_t* cpl_parse_expression(token_t** curr, syntax_ctx_t* ctx) {
    return _parse_binary_expression(curr, ctx, 0);
}
