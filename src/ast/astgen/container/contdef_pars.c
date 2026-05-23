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
        stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
        name->sinfo.t_id = TPTB_add_info(name->t->body, name->sinfo.s_id, TYPE_CUSTOM, &smt->t);
        name->t->t_type  = CUSTOM_TYPE_TOKEN;
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
            symbol_id_t type = NO_SYMBOL_ID;
            if (decl->sinfo.t_id != NO_SYMBOL_ID) type = decl->sinfo.t_id;
            else type = type_lookup(decl->t, ctx, smt);

            if (type == NO_SYMBOL_ID) {
                type = TPTB_add_info_from_token_type(decl->sinfo.s_id, decl->t->t_type, &smt->t);
            }

            decl->sinfo.t_id = type;
            TPTB_add_as_child(name->sinfo.t_id, type, decl->c->t->body, &smt->t);
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