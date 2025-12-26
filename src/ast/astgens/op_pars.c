#include <ast/astgens/astgens.h>

/*
Parse left part of a stmt.
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.

Returns an AST node.
*/
static ast_node_t* _parse_primary(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse expression that looks like: <stmt> <op> <stmt>. 
Note: <stmt> here can be either a simple <(a..> or a complex sub-stmt.
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.
    - `min_priority` - Minimal priority. 
                       Note: Defenition of a minimal priority of a token
                             that will stop parsing for the current level.

Returns an AST node.
*/
static ast_node_t* _parse_binary_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int mp) {
    ast_node_t* left = _parse_primary(it, ctx, smt);
    if (!left) return NULL;
    
    while (CURRENT_TOKEN) {
        int p = TKN_token_priority(CURRENT_TOKEN);
        if (
            p < mp || /* Skip token with a lower priority */
            p == -1   /* Skip unknown priority            */
        ) break;

        int next_mp = p + 1;
        if (TKN_update_operator(CURRENT_TOKEN)) {
            next_mp = p;
        }

        ast_node_t* op_node = AST_create_node(CURRENT_TOKEN);
        if (!op_node) {
            print_error("AST_create_node error!");
            AST_unload(left);
            return NULL;
        }

        forward_token(it, 1);
        ast_node_t* right = _parse_binary_expression(it, ctx, smt, next_mp);
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

/*
Parse array expression that looks like: <name>(opt: [<stmt>])
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.

Returns an AST node.
*/
static ast_node_t* _parse_array_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create the base for the array expression!");
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
            print_error("Index expression parse error!");
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
    SAVE_TOKEN_POINT;

    if (CURRENT_TOKEN->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        ast_node_t* node = _parse_binary_expression(it, ctx, smt, 0);
        if (!node || !CURRENT_TOKEN || CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
            print_error("Error during the binary expression parsing!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
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
