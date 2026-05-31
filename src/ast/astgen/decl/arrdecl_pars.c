#include <ast/astgen/astgen.h>

/*
Parse a type of an array. Handle recursive array types.
Params:
    - <Parser args> - Arguments.

Returns an AST node of a type.
*/
static ast_node_t* _parse_array_type(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* type = NULL;
    forward_token(it, 1);
    switch (CURRENT_TOKEN->t_type) {
        case ARRAY_TYPE_TOKEN: {
            type = AST_create_node(CURRENT_TOKEN);
            forward_token(it, 2);
            ast_node_t* arr_size = cpl_parse_expression(it, ctx, smt, carry);
            if (CURRENT_TOKEN->t_type != COMMA_TOKEN) {
                PARSE_ERROR("Can't parse nested array type! Expected the 'COMMA_TOKEN'!");
                AST_unload(type);
                AST_unload(arr_size);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            ast_node_t* arr_type = _parse_array_type(it, ctx, smt, carry);
            if (
                arr_type && arr_size && 
                consume_token(it, CLOSE_INDEX_TOKEN)
            ) {
                AST_add_node(type, arr_size);
                AST_add_node(type, arr_type);
            }
            else {
                PARSE_ERROR("Can't create the size and the type for an array!");
                AST_unload(type);
                AST_unload(arr_size);
                AST_unload(arr_type);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            break;
        }
        default: {
            type = AST_create_node(CURRENT_TOKEN);
            if (!type) {
                PARSE_ERROR("Can't create a base for the array's type!");
                RESTORE_TOKEN_POINT;
                return NULL;
            }
            break;
        }
    }

    return type;
}

static symbol_id_t _resolve_array_type(ast_node_t* type, ast_ctx_t* ctx, sym_table_t* smt);

static long _get_array_field_size(long length, ast_node_t* type, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!type || length < 0) return FIELD_NO_CHANGE;
    if (type->sinfo.t_id == NO_SYMBOL_ID) type->sinfo.t_id = _resolve_array_type(type, ctx, smt);

    long element_size = TPTB_get_memory_size_id(type->sinfo.t_id, &smt->t);
    if (element_size == FIELD_NO_CHANGE) {
        type->sinfo.t_id = TPTB_add_info_from_token(type->sinfo.s_id, type->t, NO_SYMBOL_ID, &smt->t);
        element_size = TPTB_get_memory_size_id(type->sinfo.t_id, &smt->t);
    }

    return element_size == FIELD_NO_CHANGE ? FIELD_NO_CHANGE : length * element_size;
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
    if (length && length->t->t_type == UNKNOWN_NUMERIC_TOKEN) {
        const_length = length->t->body->to_llong(length->t->body);
    }

    _resolve_array_type(elem_type, ctx, smt);
    long size = _get_array_field_size(const_length, elem_type, ctx, smt);
    type->sinfo.t_id = TPTB_add_info_from_token(type->sinfo.s_id, type->t, NO_SYMBOL_ID, &smt->t);
    TPTB_set_memory_size_id(type->sinfo.t_id, size, &smt->t);
    TPTB_link_child(type->sinfo.t_id, elem_type->sinfo.t_id, &smt->t);
    return type->sinfo.t_id;
}

ast_node_t* cpl_parse_array_declaration(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the array's declaration!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL };
    ANNOT_read_annotations(&ctx->annots, &annots);

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) AST_add_node(base, name);
    else {
        PARSE_ERROR("Can't create a base for the array's name!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_INDEX_TOKEN)) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected the 'OPEN_INDEX_TOKEN'!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* length = cpl_parse_expression(it, ctx, smt, 0);
    if (length) AST_add_node(base, length);
    else {
        PARSE_ERROR("Can't create a base for the array's size!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    if (CURRENT_TOKEN->t_type != COMMA_TOKEN) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected the 'COMMA_TOKEN'!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* type = _parse_array_type(it, ctx, smt, carry);
    if (type) AST_add_node(base, type);
    else {
        PARSE_ERROR("Can't create a base for the array's type!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, CLOSE_INDEX_TOKEN)) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected the 'CLOSE_INDEX_TOKEN'!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    long long const_length = -1;
    if (length->t->t_type != UNKNOWN_NUMERIC_TOKEN) base->t->flags.vla = 1;
    else const_length = length->t->body->to_llong(length->t->body);
    if (consume_token(it, ASSIGN_TOKEN)) {
        forward_token(it, 1);
        switch (CURRENT_TOKEN->t_type) {
            case OPEN_BLOCK_TOKEN: {
                long long act_size = 0;
                forward_token(it, 1);
                while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN) {
                    if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                        forward_token(it, 1);
                        continue;
                    }

                    ast_node_t* elem = cpl_parse_expression(it, ctx, smt, 1);
                    if (elem) AST_add_node(base, elem);
                    else { 
                        PARSE_ERROR("Error during parsing of the array's initial element!");
                        AST_unload(base);
                        ANNOT_destroy_summary(&annots);
                        RESTORE_TOKEN_POINT;
                        return NULL;
                    }

                    const_length = MAX(const_length, act_size++);
                }

                break;
            }
            case STRING_VALUE_TOKEN: {
                ast_node_t* elem = AST_create_node(CURRENT_TOKEN);
                if (elem) AST_add_node(base, elem);
                else { 
                    PARSE_ERROR("Error during parsing of the array's initial element!");
                    AST_unload(base);
                    ANNOT_destroy_summary(&annots);
                    RESTORE_TOKEN_POINT;
                    return NULL;
                }
                
                const_length = MAX(const_length, CURRENT_TOKEN->body->len(CURRENT_TOKEN->body) + 1);
                break;
            }
            default: break;
        }

        forward_token(it, 1);
    }

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    _resolve_array_type(type, ctx, smt);
    name->sinfo.v_id = VRTB_add_info(name->t->body, ARRAY_TYPE_TOKEN, name->sinfo.s_id, &base->t->flags, &smt->v);
    ARTB_add_info(name->sinfo.v_id, const_length, base->t->flags.vla, type->t->t_type, &type->t->flags, &smt->a);
    base->sinfo.t_id = TPTB_add_info_from_token(name->sinfo.s_id, base->t, name->sinfo.v_id, &smt->t);
    TPTB_set_memory_size_id(base->sinfo.t_id, _get_array_field_size(const_length, type, ctx, smt), &smt->t);
    TPTB_link_child(base->sinfo.t_id, type->sinfo.t_id, &smt->t);
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, base->sinfo.t_id, &smt->v);
    
    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.align, &smt->v);
    if (base->t->flags.glob || base->t->flags.ro) {
        if (!annots.section) annots.section = create_string(base->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    if (ctx->t_id != NO_SYMBOL_ID) {
        TPTB_add_as_child(ctx->t_id, base->sinfo.t_id, name->t->body, _get_array_field_size(const_length, type, ctx, smt), &smt->t);
    }

    ANNOT_destroy_summary(&annots);
    var_lookup(name, ctx, smt);
    return base;
}
