#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_contdef(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for a container!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) {
        AST_add_node(base, name);
        name->sinfo.v_id = TPTB_add_info(name->t->body, name->sinfo.s_id, TYPE_CUSTOM, &smt->t);
    }
    else {
        PARSE_ERROR("Can't create a name for a container!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* decls = cpl_parse_scope(it, ctx, smt, 0);
    if (decls) {
        AST_add_node(base, decls);
        for (ast_node_t* decl = decls->c; decl; decl = decl->siblings.n) {
            symbol_id_t type = type_lookup(decl->t, ctx, smt);
            if (TKN_is_builtin_type(decl->t)) {
            } // TODO:
            else if (type != NO_SYMBOL_ID) {
            }
        }
    }
    else {
        AST_unload(base);
        PARSE_ERROR("Can't parse the container's body!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return base;
}