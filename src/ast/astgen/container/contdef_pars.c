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
    ctx->t_id = name->sinfo.t_id;
    ast_node_t* decls = cpl_parse_scope(it, ctx, smt, 0);
    ctx->t_id = NO_SYMBOL_ID;
    if (decls) AST_add_node(base, decls);
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