#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_extern(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the extern statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    ast_node_t* arg = NULL;
    if (TKN_is_decl(CURRENT_TOKEN)) {
        arg = cpl_parse_variable_declaration(it, ctx, smt, carry);
        arg->t->flags.ext = 1;
    }
    else if (CURRENT_TOKEN->t_type == FUNC_TOKEN) {
        arg = cpl_parse_function(it, ctx, smt, carry);
        if (!FNTB_update_info(arg->c->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, 1, NULL, NULL, &smt->f)) {
            PARSE_ERROR("Function update error!");
            AST_unload(arg);
            AST_unload(base);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }
    else {
        PARSE_ERROR("Extern incorrect token error! extern <[type]/function>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (arg) AST_add_node(base, arg);
    else {
        PARSE_ERROR("Extern declaration error! extern <[type]/function>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return base;
}