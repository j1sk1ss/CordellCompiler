#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_variable_declaration(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the variable's declaration type! <type> <name>!");
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
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (carry != NO_SYMBOL_ID) {
        base->sinfo.t_id = carry;
        base->t->t_type  = EXTRACT_TYPE_TYPE(carry, smt);
    }

    /* Register a variable in the symtable and update its type
       from the carry, if we're working with a dynamic type. */
    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.v_id = VRTB_add_info(name->t->body, base->t->t_type, name->sinfo.s_id, &base->t->flags, &smt->v);
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, carry, &smt->v);

    /* If this is a custom type variable, register it as an array as well. */
    type_info_t ti;
    if (
        base->t->t_type == CUSTOM_TYPE_TOKEN && 
        TPTB_get_info_id(carry, &ti, &smt->t)
    ) ARTB_add_info(name->sinfo.v_id, ti.memory.size, 0, U8_TYPE_TOKEN, &base->t->flags, &smt->a);

    /* Update variable's memory flags according to the provided annotations
       and move it to a corresponding section. */
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
            PARSE_ERROR("Error during parsing of a declaration statement!");
            AST_unload(base);
            ANNOT_destroy_summary(&annots);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if ( /* If it's a global variable, it acts differently.
                It doesn't generate any initialization code and must have a pre-compiled value */
            base->t->flags.glob
        ) VRTB_update_definition(name->sinfo.v_id, value_node->t->body->to_llong(value_node->t->body), NO_SYMBOL_ID, &smt->v, 0);
        AST_add_node(base, value_node);
    }

    /* Register the variable as a basic type of the parent type,
       if this is a declaraion in a type. */
    if (base->sinfo.t_id != NO_SYMBOL_ID) base->sinfo.t_id = base->sinfo.t_id; // TODO: Figure out how to create similar types etc, maybe copy in containers?
    else                                  base->sinfo.t_id = type_lookup(base->t, ctx, smt); // TODO: refactor types system
    if (base->sinfo.t_id == NO_SYMBOL_ID) base->sinfo.t_id = TPTB_add_info_from_token(base->sinfo.s_id, base->t, name->sinfo.v_id, &smt->t);
    else                                  base->sinfo.t_id = TPTB_add_copy(base->sinfo.t_id, name->sinfo.v_id, base->t->flags.ptr, &smt->t);
    if (ctx->t_id != NO_SYMBOL_ID) {
       TPTB_add_as_child(ctx->t_id, base->sinfo.t_id, name->t->body, base->t->flags.ptr ? CONF_get_full_bytness() : FIELD_NO_CHANGE, &smt->t);
    }

    ANNOT_destroy_summary(&annots);
    return base;
}
