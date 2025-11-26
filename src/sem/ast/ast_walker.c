#include <sem/ast/ast_walker.h>

int ASTWLK_register_visitor(token_type_t trg, int (*perform)(ast_node_t*), ast_walker_t* ctx) {
    ast_visitor_t* v = ASTVIS_create_visitor(trg, perform);
    if (!v) return 0;
    return list_add(&ctx->visitors, v);
}

int ASTWLK_init_ctx(ast_walker_t* ctx, sym_table_t* smt) {
    ctx->smt = smt;
    return list_init(&ctx->visitors);
}

static ast_node_type_t _get_ast_node_type(token_type_t tkn) {
    switch (tkn) {
        case START_TOKEN: return START_NODE;
        case CALL_TOKEN:  return CALL_NODE;
        case FUNC_TOKEN:  return FUNCTION_NODE;
        
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: return DEF_ARRAY_NODE;

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
        case DIVASSIGN_TOKEN:  return ASSIGN_NODE;

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

static int _ast_walk(ast_node_t* nd, ast_walker_t* ctx) {
    if (!nd) return 0;
    _ast_walk(nd->child, ctx);
    _ast_walk(nd->sibling, ctx);
    
    list_iter_t it;
    ast_visitor_t* v;
    list_iter_hinit(&ctx->visitors, &it);
    while ((v = (ast_visitor_t*)list_iter_next(&it))) {
        if (
            nd->token && 
            _get_ast_node_type(nd->token->t_type) & v->trg
        ) v->perform(nd, ctx->smt);
    }

    return 1;
}

int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx) {
    return _ast_walk(actx->r, ctx);
}

int ASTWLK_unload_ctx(ast_walker_t* ctx) {
    list_free_force(&ctx->visitors);
    return mm_free(ctx);
}
