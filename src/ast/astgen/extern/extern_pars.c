#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_extern(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the extern statement!");
    
    forward_token(it, 1);
    ast_node_t* arg = NULL;
    symbol_id_t type = type_lookup(CURRENT_TOKEN, ctx, smt);
    if (TKN_is_builtin_type(CURRENT_TOKEN) || type != NO_SYMBOL_ID) {
        CURRENT_TOKEN->flags.ext  = 1;
        CURRENT_TOKEN->flags.glob = 1;
        arg = cpl_parse_variable_declaration(it, ctx, smt, type);
        arg->t->flags.ext = 1;
    }
    else if (CURRENT_TOKEN->t_type == FUNC_TOKEN) {
        arg = cpl_parse_function(it, ctx, smt, carry);
        PARSER_DO_OR_THROW_DO(
            !FNTB_update_func(arg->c->sinfo.v_id, FNTB_ONLY_FLAGS(FNTB_SET_EXTERNAL(FNTB_EXPLICIT_EXTERN)), &smt->f), "Function update error!",
            { AST_unload(arg); AST_unload(base); }
        );
    }
    else {
        PARSE_ERROR("Extern unknown token error! extern <[type]/function>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    PARSER_DO_OR_THROW(!arg, base, "Extern declaration error! extern <[type]/function>!");
    AST_add_node(base, arg);
    return base;
}