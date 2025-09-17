#include <cpl_parser.h>

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
static ast_node_t* _parse_binary_expression(token_t** curr, syntax_ctx_t* ctx, int min_priority, parser_t* p) {
    ast_node_t* left = _parse_primary(curr, ctx, p);
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
        ast_node_t* right = _parse_binary_expression(curr, ctx, next_min_priority, p);
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

static ast_node_t* _parse_array_expression(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    STX_var_lookup(node, ctx);
    
    forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_INDEX_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* offset_exp = p->expr(curr, ctx, p);
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
    ast_node_t* right = cpl_parse_expression(curr, ctx, p);
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

static ast_node_t* _parse_primary(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* node = _parse_binary_expression(curr, ctx, 0, p);
        if (!node || !*curr || (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            print_error("AST error during expression parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }

        forward_token(curr, 1);
        return node;
    }
    
    if (VRS_isptr(*curr))                      return _parse_array_expression(curr, ctx, p);
    else if ((*curr)->t_type == CALL_TOKEN)    return p->funccall(curr, ctx, p);
    else if ((*curr)->t_type == SYSCALL_TOKEN) return p->syscall(curr, ctx, p);

    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    STX_var_lookup(node, ctx);
    forward_token(curr, 1);
    return node;
}

ast_node_t* cpl_parse_expression(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    return _parse_binary_expression(curr, ctx, 0, p);
}
