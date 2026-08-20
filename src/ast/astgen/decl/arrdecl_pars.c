#include <ast/astgen/astgen.h>

/* Parse a type of an array. Handle recursive array types.
Params:
    - <Parser args> - Arguments.

Returns an AST node of a type. */
static DEFINE_PARSER(_parse_array_type, {
    ast_node_t* type = NULL;
    forward_token(it, 1);
    switch (CURRENT_TOKEN->t_type) {
        case ARRAY_TYPE_TOKEN: {
            type = AST_create_node(CURRENT_TOKEN);
            forward_token(it, 2);
            ast_node_t* arr_size = cpl_parse_expression(it, ctx, smt, carry);
            PARSER_ASSERT_DO(
                !type || !arr_size || CURRENT_TOKEN->t_type != COMMA_TOKEN, "Can't parse nested array type! Expected the ',' token!", 
                { AST_unload(type); AST_unload(arr_size); }
            );

            ast_node_t* arr_type = _parse_array_type(it, ctx, smt, carry);
            PARSER_ASSERT_DO(
                !consume_token(it, CLOSE_INDEX_TOKEN), "Can't create the size and the type for an array!", 
                { AST_unload(type); AST_unload(arr_size); AST_unload(arr_type); }
            );

            AST_add_node(type, arr_size);
            AST_add_node(type, arr_type);
            break;
        }
        default: {
            type = AST_create_node(CURRENT_TOKEN);
            PARSER_ASSERT(!type, NULL, "Can't create a base for the array's type!");
            break;
        }
    }

    return type;
})

static symbol_id_t _resolve_array_type(ast_node_t* type, ast_ctx_t* ctx, sym_table_t* smt);

static long _get_array_field_size(long length, ast_node_t* type, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!type || length < 0) return SMT_NULL;
    if (type->sinfo.t_id == NO_SYMBOL_ID) type->sinfo.t_id = _resolve_array_type(type, ctx, smt);

    long element_size = TPTB_get_memory_size_id(type->sinfo.t_id, &smt->t);
    if (element_size == SMT_NULL) {
        type->sinfo.t_id = TPTB_add_info_from_token(type->sinfo.s_id, type->t, NO_SYMBOL_ID, &smt->t);
        element_size = TPTB_get_memory_size_id(type->sinfo.t_id, &smt->t);
    }

    return element_size == SMT_NULL ? SMT_NULL : length * element_size;
}

static symbol_id_t _resolve_array_type(ast_node_t* type, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!type) return NO_SYMBOL_ID;
    if (type->sinfo.t_id != NO_SYMBOL_ID) return type->sinfo.t_id;

    /* Array of non-array types, we lazy create a type and checks whether it is a new
       one */
    if (type->t->t_type != ARRAY_TYPE_TOKEN) {
        type->sinfo.t_id = type_lookup(type->t, ctx, smt);
        if (type->sinfo.t_id == NO_SYMBOL_ID) type->sinfo.t_id = TPTB_add_info_from_token(type->sinfo.s_id, type->t, NO_SYMBOL_ID, &smt->t);
        return type->sinfo.t_id;
    }

    ast_node_t* length     = type->c;
    ast_node_t* elem_type  = length ? length->siblings.n : NULL;
    long long const_length = -1;
    if (
        length && 
        length->t->t_type == UNKNOWN_NUMERIC_TOKEN
    ) const_length = length->t->body->to_llong(length->t->body);
    
    _resolve_array_type(elem_type, ctx, smt);
    type->sinfo.t_id = TPTB_add_info_from_token(type->sinfo.s_id, type->t, NO_SYMBOL_ID, &smt->t);
    TPTB_set_memory_size_id(type->sinfo.t_id, _get_array_field_size(const_length, elem_type, ctx, smt), &smt->t);
    TPTB_link_child(type->sinfo.t_id, elem_type->sinfo.t_id, &smt->t);
    return type->sinfo.t_id;
}

DEFINE_PARSER(cpl_parse_array_declaration, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for the array's declaration!");

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL, .salign = -1 };
    ANNOT_read_annotations(&ctx->annots, &annots);

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT_DO(
        !name, "Can't create a base for the array's name!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );
    AST_add_node(base, name);

    PARSER_ASSERT_DO(
        !consume_token(it, OPEN_INDEX_TOKEN), "Error during array parsing! arr <name>[<size>, <type>]! Expected the '[' token!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    forward_token(it, 1);
    ast_node_t* length = cpl_parse_expression(it, ctx, smt, 0);
    PARSER_ASSERT_DO(
        !length, "Can't create a base for the array's size!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    AST_add_node(base, length);
    
    PARSER_ASSERT_DO(
        CURRENT_TOKEN->t_type != COMMA_TOKEN, "Error during array parsing! arr <name>[<size>, <type>]! Expected the ',' token!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    ast_node_t* type = _parse_array_type(it, ctx, smt, carry);
    PARSER_ASSERT_DO(
        !type, "Can't create a base for the array's type!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    AST_add_node(base, type);

    PARSER_ASSERT_DO(
        !consume_token(it, CLOSE_INDEX_TOKEN), "Error during array parsing! arr <name>[<size>, <type>]! Expected the ']' token!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    long long const_length = -1;
    if (length->t->t_type != UNKNOWN_NUMERIC_TOKEN) base->t->flags.vla = 1;
    else const_length = length->t->body->to_llong(length->t->body);
    ast_node_t* init_values = cpl_parse_declaration_value(it, ctx, smt, &const_length);
    if (init_values) AST_add_node(base, init_values);

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    _resolve_array_type(type, ctx, smt);
    
    long array_size  = _get_array_field_size(const_length, type, ctx, smt);
    name->sinfo.v_id = VRTB_add_info(name->t->body, ARRAY_TYPE_TOKEN, name->sinfo.s_id, base->t->flags, &smt->v); /* register as a variable                   */ 
    if (CONF_is_symtab_error()) {
        PARSE_ERROR("Can't register an array with the name '%s'", name->t->body->body);
        CONF_set_parser_error();
    }

    base->sinfo.t_id = TPTB_add_info_from_token(name->sinfo.s_id, base->t, name->sinfo.v_id, &smt->t);            /* register as a type                       */
    ARTB_add_info(name->sinfo.v_id, const_length, base->t->flags.vla, type->t->t_type, type->t->flags, &smt->a);  /* register as an array                     */
    TPTB_set_memory_size_id(base->sinfo.t_id, array_size, &smt->t);                                               /* select the type of the array             */
    TPTB_link_child(base->sinfo.t_id, type->sinfo.t_id, &smt->t);                                                 /*                                          */
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, base->sinfo.t_id, &smt->v);                               /* link type to the variable's id           */
    TPTB_add_as_child(ctx->t_id, base->sinfo.t_id, name->t->body, array_size, &smt->t);                           /* ling array to a container (if it exists) */

    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.align, &smt->v);
    if (!TKN_in_stack(base->t)) {
        if (!annots.section) annots.section = create_string(base->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, annots.salign, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    ANNOT_destroy_summary(&annots);
    var_lookup(name, ctx, smt);
    return base;
})
