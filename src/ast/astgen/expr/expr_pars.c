/* Compex expression parser.
   Parses and invokes low-level parsers */
#include <ast/astgen/astgen.h>

/*
Parse left part of a stmt.
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.
    - `na` - No assign.
             Note: By default (0), this function parses an entire 
                   expression with a assign symbol. That means, that
                   expressions such as `a = b`, `a + b = a + b` will
                   be full parsed.
                   If you want to parse only the left part (before assign),
                   set this flag to 1.

Returns an AST node.
*/
static ast_node_t* _parse_primary(list_iter_t*, ast_ctx_t*, sym_table_t*, int);

/*
Parse expression that looks like: <stmt> <op> <stmt>. 
Note: <stmt> here can be either a simple <(a..> or a complex sub-stmt.
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.
    - `mp` - Minimal priority. 
             Note: Defenition of a minimal priority of a token
                   that will stop parsing for the current level.
    - `na` - No assign.
             Note: By default (0), this function parses an entire 
                   expression with a assign symbol. That means, that
                   expressions such as `a = b`, `a + b = a + b` will
                   be full parsed.
                   If you want to parse only the left part (before assign),
                   set this flag to 1.

Returns an AST node.
*/
static ast_node_t* _parse_binary_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int mp, int na) {
    SAVE_TOKEN_POINT;

    ast_node_t* left = _parse_primary(it, ctx, smt, na);
    if (!left) {
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    while (CURRENT_TOKEN) {
        switch (CURRENT_TOKEN->t_type) {
            /* Convert operator */
            case CONVERT_TOKEN: {
                ast_node_t* conv_type = cpl_parse_conv(it);
                if (conv_type) {
                    ast_node_t* tmp = left;
                    left = conv_type;
                    AST_add_node(left, tmp);
                    forward_token(it, 1);
                }
                else {
                    PARSE_ERROR("Error during a cast parsing!");
                    AST_unload(left);
                    RESTORE_TOKEN_POINT;
                    return NULL;
                }

                goto _stop_expression_parsing;
            }
            /* Default operators such as:
               plus, minus, multiply, etc. */
            default: {
                int p = TKN_token_priority(CURRENT_TOKEN);
                if (
                    p < mp  ||                                    /* Stop at a token with a lower priority   */
                    p == -1 ||                                    /* Stop at an unknown priority             */
                    (na && CURRENT_TOKEN->t_type == ASSIGN_TOKEN) /* Stop at an assign if there is a na == 1 */
                ) goto _stop_expression_parsing;

                int next_mp = p + 1;
                if (TKN_update_operator(CURRENT_TOKEN)) {
                    next_mp = p;
                }

                ast_node_t* op_node = AST_create_node(CURRENT_TOKEN);
                if (!op_node) {
                    PARSE_ERROR("Can't create the expression's base!");
                    AST_unload(left);
                    RESTORE_TOKEN_POINT;
                    return NULL;
                }

                forward_token(it, 1);
                ast_node_t* right = _parse_binary_expression(it, ctx, smt, next_mp, na);
                if (!right) {
                    PARSE_ERROR("Error during the right part parse!");
                    AST_unload(op_node);
                    AST_unload(left);
                    RESTORE_TOKEN_POINT;
                    return NULL;
                }

                AST_add_node(op_node, left);
                AST_add_node(op_node, right);
                left = op_node;
                break;
            }
        }
    }

_stop_expression_parsing: {}
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
static ast_node_t* _parse_array_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int na) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the array-like expression!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type == OPEN_INDEX_TOKEN) {
        forward_token(it, 1);
        ast_node_t* offset_exp = cpl_parse_expression(it, ctx, smt, na);
        if (!offset_exp) {
            PARSE_ERROR("Index expression parse error!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(node, offset_exp);
        forward_token(it, 1);
    }

    var_lookup(node, ctx, smt);
    return node;
}

static ast_node_t* _parse_primary(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int na) {
    SAVE_TOKEN_POINT;
    
    if (TKN_isclose(CURRENT_TOKEN)) {
        PARSE_ERROR("Expected a token, but got a terminator!");
        return NULL;
    }

    /* Check basic cases such as pointer access, 
       call token (basic call), syscall token */
    if (TKN_isptr(CURRENT_TOKEN)) return _parse_array_expression(it, ctx, smt, na); /* str / arr[] / arr / ptr / ptr[] */
    switch (CURRENT_TOKEN->t_type) {
        case OPEN_BRACKET_TOKEN: {
            forward_token(it, 1);
            ast_node_t* node = _parse_binary_expression(it, ctx, smt, 0, na);
            if (
                !node || !CURRENT_TOKEN || 
                CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN
            ) {
                PARSE_ERROR("Error during the binary expression parsing!");
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            forward_token(it, 1);
            return node;
        }
    
        case CALL_TOKEN:      return cpl_parse_funccall(it, ctx, smt); /* call()    */
        case POPARG_TOKEN:    return cpl_parse_poparg(it);             /* poparg    */
        case SYSCALL_TOKEN:   return cpl_parse_syscall(it, ctx, smt);  /* syscall() */
        case NEGATIVE_TOKEN:  return cpl_parse_neg(it, ctx, smt);      /* neg       */
        case REF_TYPE_TOKEN:  return cpl_parse_ref(it, ctx, smt);      /* ref       */
        case DREF_TYPE_TOKEN: return cpl_parse_dref(it, ctx, smt);     /* dref      */
        default: break;
    }

    /* If this isn't a basic case, we are able to say,
       that this is a variable / value */
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the value!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    /* Register a string in a string symbol table 
    */
    if (node->t->t_type == STRING_VALUE_TOKEN) {
        node->sinfo.v_id = STTB_add_info(node->t->body, STR_INDEPENDENT, &smt->s);
    }

    var_lookup(node, ctx, smt);
    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, int na) {
    return _parse_binary_expression(it, ctx, smt, 0, na);
}
