#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_contdef(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL, .reg = FIELD_NO_CHANGE };
    ANNOT_read_annotations(&ctx->annots, &annots);

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for a container!");
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) {
        AST_add_node(base, name);
        stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
        name->sinfo.t_id = TPTB_add_info(name->t->body, name->sinfo.s_id, TYPE_CUSTOM, annots.align, &smt->t);
        name->t->t_type  = CUSTOM_TYPE_TOKEN;
    }
    else {
        PARSE_ERROR("Can't create a name for a container!");
        ANNOT_destroy_summary(&annots);
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
                type = TPTB_add_info_from_token(decl->sinfo.s_id, decl->t, &smt->t);
            }
            else if (type == name->sinfo.t_id) {
                type = TPTB_add_copy(type, decl->t, &smt->t);
            }

            decl->sinfo.t_id = type;
            TPTB_add_as_child(
                name->sinfo.t_id, type, decl->c->t->body, 
                decl->t->flags.ptr ? CONF_get_full_bytness() : FIELD_NO_CHANGE, &smt->t
            );
        }
    }
    else {
        PARSE_ERROR("Can't parse the container's body!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ANNOT_destroy_summary(&annots);
    return base;
}