#include <cpl_parser.h>

static ast_node_t* _dummy_parser(token_t** curr, syntax_ctx_t* ctx, parser_t* p) { return NULL; }
static ast_node_t* (*_get_parser(syntax_ctx_t* ctx, token_type_t t_type, parser_t* p))(token_t**, syntax_ctx_t*, parser_t*) {
    switch (t_type) {
        case START_TOKEN:           return p->start;
        case ASM_TOKEN:             return p->asmer;
        case OPEN_BLOCK_TOKEN:
        case CLOSE_BLOCK_TOKEN:     return p->scope;
        case STR_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case I16_TYPE_TOKEN:      
        case U32_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case U16_TYPE_TOKEN:        return p->vardecl;
        case SWITCH_TOKEN:          return p->switchstmt;
        case IF_TOKEN:              
        case WHILE_TOKEN:           return p->condop;
        case I32_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case UNKNOWN_STRING_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN: return p->expr;
        case SYSCALL_TOKEN:         return p->syscall;
        case EXTERN_TOKEN:          return p->extrn;
        case IMPORT_SELECT_TOKEN:   return p->import;
        case ARRAY_TYPE_TOKEN:      return p->arraydecl;
        case CALL_TOKEN:            return p->funccall;
        case FUNC_TOKEN:            return p->function;
        case EXIT_TOKEN:
        case RETURN_TOKEN:          return p->rexit;
        default:                    return _dummy_parser;
    }
}

ast_node_t* cpl_parse_block(token_t** curr, syntax_ctx_t* ctx, token_type_t ex, parser_t* p) {
    ast_node_t* node = AST_create_node(NULL);
    if (!node) return NULL;
    
    while (*curr && (*curr)->t_type != ex) {
        ast_node_t* block = _get_parser(ctx, (*curr)->t_type, p)(curr, ctx, p);
        if (block) AST_add_node(node, block);
        else {
            if (!forward_token(curr, 1)) break;
        }
    }
    
    return node;
}
