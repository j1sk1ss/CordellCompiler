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

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL, .salign = -1, .reg = -1 };
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
    name->sinfo.v_id = VRTB_add_info(name->t->body, base->t->t_type, name->sinfo.s_id, base->t->flags, &smt->v);
    if (CONF_is_symtab_error()) {
        PARSE_ERROR("Can't register a variable with the name '%s'", name->t->body->body);
        CONF_set_parser_error();
    }
    
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, carry, &smt->v);

    /* If this is a custom type variable, register it as an array as well. */
    type_info_t ti;
    if (
        base->t->t_type == CUSTOM_TYPE_TOKEN && 
        TPTB_get_info_id(carry, &ti, &smt->t)
    ) ARTB_add_info(name->sinfo.v_id, ti.memory.size, 0, U8_TYPE_TOKEN, base->t->flags, &smt->a);

    /* Update variable's memory flags according to the provided annotations
       and move it to a corresponding section. */
    var_lookup(name, ctx, smt);
    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.reg, annots.align, &smt->v);
    if (!TKN_in_stack(name->t)) {
        if (!annots.section) annots.section = create_string(name->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, annots.salign, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    ast_node_t* init_values = cpl_parse_declaration_value(it, ctx, smt, 0);
    if (init_values) AST_add_node(base, init_values);
    
    /* Register the variable as a basic type of the parent type,
       if this is a declaraion in a type. */
    symbol_id_t declared_type = base->sinfo.t_id;
    if (declared_type == NO_SYMBOL_ID) declared_type = type_lookup(base->t, ctx, smt);
    if (declared_type == NO_SYMBOL_ID) {
        declared_type = TPTB_add_info_from_token(base->sinfo.s_id, base->t, NO_SYMBOL_ID, &smt->t);
    }

    base->sinfo.t_id = TPTB_add_copy(declared_type, name->sinfo.v_id, base->t->flags.ptr, &smt->t);
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, base->sinfo.t_id, &smt->v);
    TPTB_add_as_child(ctx->t_id, base->sinfo.t_id, name->t->body, base->t->flags.ptr ? CONF_get_full_bytness() : FIELD_NO_CHANGE, &smt->t);

    ANNOT_destroy_summary(&annots);
    return base;
}
