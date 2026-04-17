#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_variable_declaration(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the variable declaration type! <type> <name>!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL, .reg = FIELD_NO_CHANGE };
    ANNOT_read_annotations(&ctx->annots, &annots);
    if (annots.is_argpop) list_add(&base->annots, ANNOT_create_annotation(POPARG_ANNOTATION, NULL, 0));

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) AST_add_node(base, name);
    else {
        PARSE_ERROR("Can't create a base for the variable's name! <type> <name>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.v_id = VRTB_add_info(name->t->body, base->t->t_type, name->sinfo.s_id, &name->t->flags, &smt->v);
    if (
        !base->t->flags.ptr &&
        base->t->t_type == STR_TYPE_TOKEN
    ) ARTB_add_info(name->sinfo.v_id, 0, 0, I8_TYPE_TOKEN, &base->t->flags, &smt->a);
    var_lookup(name, ctx, smt);

    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.reg, annots.align, &smt->v);
    if (!TKN_in_stack(name->t)) {
        if (!annots.section) annots.section = create_string(name->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    if (consume_token(it, ASSIGN_TOKEN)) {
        forward_token(it, 1);
        ast_node_t* value_node = cpl_parse_expression(it, ctx, smt, 1);
        if (!value_node) {
            PARSE_ERROR("Error during parsing of the declaration statement!");
            AST_unload(base);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (
            !base->t->flags.ptr &&            /* Array can be only on a stack         */
            base->t->t_type == STR_TYPE_TOKEN /* String is a special case of an array */
        ) {
            ARTB_update_info(
                name->sinfo.v_id, value_node->t->body->len(value_node->t->body) + 1, FIELD_NO_CHANGE, 
                I8_TYPE_TOKEN, &base->t->flags, &smt->a
            );
            STTB_update_info(value_node->sinfo.v_id, NULL, STR_ARRAY_VALUE, &smt->s);
        }

        if (
            base->t->flags.glob
        ) VRTB_update_definition(name->sinfo.v_id, value_node->t->body->to_llong(value_node->t->body), NO_SYMBOL_ID, &smt->v, 0);
        AST_add_node(base, value_node);
    }

    ANNOT_destroy_summary(&annots);
    return base;
}
