#include <cpl_parser.h>

static ast_node_t* _navigation_handler(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    if (!curr || !*curr) return 0;
    switch ((*curr)->t_type) {
        case START_TOKEN:           return p->start(curr, ctx, p);
        case ASM_TOKEN:             return p->asmer(curr, ctx, p);
        case OPEN_BLOCK_TOKEN:
        case CLOSE_BLOCK_TOKEN:     return p->scope(curr, ctx, p);
        case STR_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case I16_TYPE_TOKEN:      
        case U32_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case U16_TYPE_TOKEN:        return p->vardecl(curr, ctx, p);
        case SWITCH_TOKEN:          return p->switchstmt(curr, ctx, p);
        case IF_TOKEN:              
        case WHILE_TOKEN:           return p->condop(curr, ctx, p);
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
        case UNKNOWN_NUMERIC_TOKEN: return p->expr(curr, ctx, p);
        case SYSCALL_TOKEN:         return p->syscall(curr, ctx, p);
        case EXTERN_TOKEN:          return p->extrn(curr, ctx, p);
        case IMPORT_SELECT_TOKEN:   return p->import(curr, ctx, p);
        case ARRAY_TYPE_TOKEN:      return p->arraydecl(curr, ctx, p);
        case CALL_TOKEN:            return p->funccall(curr, ctx, p);
        case FUNC_TOKEN:            return p->function(curr, ctx, p);
        case EXIT_TOKEN:
        case RETURN_TOKEN:          return p->rexit(curr, ctx, p);
        default:                    return NULL;
    }
}

ast_node_t* cpl_parse_block(token_t** curr, syntax_ctx_t* ctx, token_type_t ex, parser_t* p) {
    ast_node_t* node = AST_create_node(NULL);
    if (!node) return NULL;
    
    while (*curr && (*curr)->t_type != ex) {
        ast_node_t* block = _navigation_handler(curr, ctx, p);
        if (block) AST_add_node(node, block);
        else {
            if (!forward_token(curr, 1)) break;
        }
    }
    
    return node;
}
