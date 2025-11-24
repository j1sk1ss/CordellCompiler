#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(list_iter_current(it));
    if (!node) return NULL;
    
    forward_token(it, 1);
    if (list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        while (list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
            if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
            if (arg) AST_add_node(node, arg);
        }
    }

    ast_node_t* body = AST_create_node(NULL);
    if (!body) {
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 1);
    if (list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(it, 1);
        while (list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BLOCK_TOKEN) {
            if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            token_t* asmline = (token_t*)list_iter_current(it);
            int sid = STTB_add_info(asmline->value, STR_RAW_ASM, &smt->s);
            ast_node_t* arg = AST_create_node(asmline);
            arg->sinfo.v_id = sid;
            if (arg) AST_add_node(body, arg);
            forward_token(it, 1);
        }
    }

    forward_token(it, 1);
    AST_add_node(node, body);
    return node;
}
