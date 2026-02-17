#include <sem/ast/ast_walker.h>

/*
Create a semantic handler for a walker.
Params:
    - `v` - AST visitor.
    - `l` - Handler level.

Returns the semantic handler.
*/
static ast_sem_handler_t* _create_sem_handler(ast_visitor_t* v, attention_level_t l) {
    ast_sem_handler_t* h = (ast_sem_handler_t*)mm_malloc(sizeof(ast_sem_handler_t));
    if (!h) return NULL;
    h->w = v;
    h->l = l;
    return h;
}

/*
Unload the semantic handler.
Params:
    - `h` - Semantic handler.

Returns 1 if succeeds.
*/
static int _unload_sem_handler(ast_sem_handler_t* h) {
    ASTVIS_unload_visitor(h->w);
    return mm_free(h);
}

int ASTWLK_register_visitor(unsigned int trg, int (*perform)(AST_VISITOR_ARGS), ast_walker_t* ctx, attention_level_t l) {
    ast_visitor_t* v = ASTVIS_create_visitor(trg, perform);
    if (!v) return 0;
    ast_sem_handler_t* w = _create_sem_handler(v, l);
    if (!w) {
        _unload_sem_handler(w);
        return 0;
    }

    return list_add(&ctx->visitors, w);
}

int ASTWLK_init_ctx(ast_walker_t* ctx, sym_table_t* smt) {
    str_memset(ctx, 0, sizeof(ast_walker_t));
    ctx->smt = smt;
    return list_init(&ctx->visitors);
}

/*
Get a node type based on the provided token type.
Params:
    - `tkn` - Token type.

Returns a node type.
*/
static ast_node_type_t _get_ast_node_type(token_type_t tkn) {
    switch (tkn) {
        case BREAK_TOKEN:  return BREAK_NODE;
        case DEFAULT_TOKEN:
        case CASE_TOKEN:   return CASE_NODE;
        case SWITCH_TOKEN: return SWITCH_NODE;
        case RETURN_TOKEN:
        case EXIT_TOKEN:   return TERM_NODE;
        case IF_TOKEN:     return IF_NODE;
        case LOOP_TOKEN:   return LOOP_NODE;
        case WHILE_TOKEN:  return WHILE_NODE;
        case START_TOKEN:  return START_NODE;
        case ADDR_CALL_TOKEN:
        case CALL_TOKEN:   return CALL_NODE;
        case FUNC_TOKEN:   return FUNCTION_NODE;
        
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: return DEF_ARRAY_NODE;

        case CHAR_VALUE_TOKEN:
        case STRING_VALUE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN: return VARIABLE_NODE;

        case I0_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN: return DECLARATION_NODE;

        case ASSIGN_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case MULASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case BITORASSIGN_TOKEN:
        case MODULOASSIGN_TOKEN:
        case BITANDASSIGN_TOKEN:
        case BITXORASSIGN_TOKEN: return ASSIGN_NODE;

        case OR_TOKEN:
        case AND_TOKEN:
        case PLUS_TOKEN:
        case BITOR_TOKEN:
        case LOWER_TOKEN:
        case MINUS_TOKEN:
        case LARGER_TOKEN:
        case DIVIDE_TOKEN:
        case MODULO_TOKEN:
        case BITAND_TOKEN:
        case BITXOR_TOKEN:
        case LOWEREQ_TOKEN:
        case COMPARE_TOKEN:
        case MULTIPLY_TOKEN:
        case NCOMPARE_TOKEN:
        case LARGEREQ_TOKEN:
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: return EXPRESSION_NODE;
        default: break;
    }

    return UNKNOWN_NODE;
}

/*
Update the walker flag.
Params:
    - `nd` - Current node under consideration.
    - `f` - Flags to update.
    - `s` - Flags value.

Returns 1 if succeeds.
*/
static int _update_flags(ast_node_t* nd, walker_flags_t* f, int s) {
    if (nd->t) switch (_get_ast_node_type(nd->t->t_type)) {
        case IF_NODE:       f->in_if = MAX(f->in_if + s, 0);         break;
        case LOOP_NODE:     f->in_loop = MAX(f->in_loop + s, 0);     break;
        case WHILE_NODE:    f->in_while = MAX(f->in_while + s, 0);   break;
        case FUNCTION_NODE: f->in_func = MAX(f->in_func + s, 0);     break;
        case START_NODE:    f->in_start = MAX(f->in_start + s, 0);   break;
        case SWITCH_NODE:   f->in_switch = MAX(f->in_switch + s, 0); break;
        case CASE_NODE:     f->in_case = MAX(f->in_case + s, 0);     break;
        default: break;
    }

    return 1;
}

/*
Perform a walk thru the AST. This is a DFS approach, that allows us
to use a analytic symtables for the complex static analysis.
Params:
    - `nd` - AST node.
    - `ctx` - Walker context.

Returns 1 if succeeds. Otherwise returns 0 - Semantic block of a compilation.
*/
static int _ast_walk(ast_node_t* nd, ast_walker_t* ctx) {
    if (!nd) return 0;
    
    /* Parent flags setup */
    _update_flags(nd, &ctx->flags, 1);
    if (_ast_walk(nd->c, ctx) == -1) return 0;

    /* Parent flags update */
    _update_flags(nd, &ctx->flags, -1);
    if (_ast_walk(nd->siblings.n, ctx) == -1) return 0;

    foreach (ast_sem_handler_t* v, &ctx->visitors) {
        if (
            nd->t && 
            _get_ast_node_type(nd->t->t_type) & v->w->trg
        ) {
            int res = v->w->perform(nd, &ctx->flags, ctx->smt);
            if (
                !res && 
                v->l == ATTENTION_BLOCK_LEVEL
            ) return -1;
        }
    }

    return 1;
}

int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx) {
    return _ast_walk(actx->r, ctx);
}

int ASTWLK_unload_ctx(ast_walker_t* ctx) {
    list_free_force_op(&ctx->visitors, (int (*)(void *))_unload_sem_handler);
    return mm_free(ctx);
}
