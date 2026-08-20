#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_contdef, {
    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL, .salign = -1, .reg = -1 };
    ANNOT_read_annotations(&ctx->annots, &annots);

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for a container!");

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT_DO(!name, "Can't create a name for a container!", { AST_unload(base); ANNOT_destroy_summary(&annots); });

    AST_add_node(base, name);
    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.t_id = TPTB_add_info(name->t->body, name->sinfo.s_id, TYPE_CUSTOM, annots.is_like_c ? -1 : annots.align, !annots.is_union, &smt->t);
    name->t->t_type  = CUSTOM_TYPE_TOKEN;

    forward_token(it, 1);
    ctx->t_id = name->sinfo.t_id;
    ast_node_t* decls = cpl_parse_scope(it, ctx, smt, 1);
    ctx->t_id = NO_SYMBOL_ID;

    PARSER_ASSERT_DO(!decls, "Can't parse the container's body!", { AST_unload(base); ANNOT_destroy_summary(&annots); });
    
    TPTB_set_child_scope_id(name->sinfo.t_id, decls->sinfo.s_id, &smt->t);
    AST_add_node(base, decls);
    ANNOT_destroy_summary(&annots);
    return base;
})
