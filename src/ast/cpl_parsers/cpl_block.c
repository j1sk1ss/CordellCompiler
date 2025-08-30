#include <cpl_parser.h>

static ast_node_t* _dummy_parser(token_t** curr, syntax_ctx_t* ctx) { return NULL; }
static ast_node_t* (*_get_parser(syntax_ctx_t* ctx, token_type_t t_type))(token_t**, syntax_ctx_t*) {
    switch (t_type) {
        case START_TOKEN:           return ctx->start;
        case OPEN_BLOCK_TOKEN:
        case CLOSE_BLOCK_TOKEN:     return ctx->scope;
        case STR_TYPE_TOKEN:
        case INT_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:
        case LONG_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:      return ctx->vardecl;
        case SWITCH_TOKEN:          return ctx->switchstmt;
        case IF_TOKEN:              
        case WHILE_TOKEN:           return ctx->condop;
        case INT_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case UNKNOWN_STRING_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN: return ctx->expr;
        case SYSCALL_TOKEN:         return ctx->syscall;
        case IMPORT_SELECT_TOKEN:   return ctx->import;
        case ARRAY_TYPE_TOKEN:      return ctx->arraydecl;
        case CALL_TOKEN:            return ctx->funccall;
        case FUNC_TOKEN:            return ctx->function;
        case EXIT_TOKEN:
        case RETURN_TOKEN:          return ctx->rexit;
        default:                    return _dummy_parser;
    }
}

ast_node_t* cpl_parse_block(token_t** curr, syntax_ctx_t* ctx, token_type_t ex) {
    ast_node_t* node = AST_create_node(NULL);
    if (!node) return NULL;
    
    while (*curr && (*curr)->t_type != ex) {
        ast_node_t* block = _get_parser(ctx, (*curr)->t_type)(curr, ctx);
        if (block) AST_add_node(node, block);
        else {
            if (!forward_token(curr, 1)) break;
        }
    }
    
    return node;
}
