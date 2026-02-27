/* Declaration statement parser.
   - arr <name>[<size>, <primitive_type>] = { decl }; */
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
            ast_node_t* arr_type = _parse_array_type(it, ctx, smt, carry);
            if (arr_type && arr_size) {
                PARSE_ERROR("Can't create the size and the type for the array!");
                AST_add_node(type, arr_size);
                AST_add_node(type, arr_type);
                RESTORE_TOKEN_POINT;
                return NULL;
            }
            else {
                AST_unload(arr_size);
                AST_unload(arr_type);
            }

            break;
        }
        default: {
            type = AST_create_node(CURRENT_TOKEN);
            if (!type) {
                PARSE_ERROR("Can't create a base for the array type!");
                RESTORE_TOKEN_POINT;
                return NULL;
            }
            break;
        }
    }

    return type;
}

ast_node_t* cpl_parse_array_declaration(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the array declaration!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) AST_add_node(base, name);
    else {
        PARSE_ERROR("Can't create a base for the array name!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_INDEX_TOKEN)) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected OPEN_INDEX_TOKEN!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* length = cpl_parse_expression(it, ctx, smt, 0);
    if (length) AST_add_node(base, length);
    else {
        PARSE_ERROR("Can't create a base for the size!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    if (CURRENT_TOKEN->t_type != COMMA_TOKEN) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected COMMA_TOKEN!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* type = _parse_array_type(it, ctx, smt, carry);
    if (type) AST_add_node(base, type);
    else {
        PARSE_ERROR("Can't create a base for the array type!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, CLOSE_INDEX_TOKEN)) {
        PARSE_ERROR("Error during array parsing! arr <name>[<size>, <type>]! Expected CLOSE_INDEX_TOKEN!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    long long const_length = -1;
    if (length->t->t_type != UNKNOWN_NUMERIC_TOKEN) name->t->flags.heap = 1;
    else const_length = length->t->body->to_llong(length->t->body);
    if (consume_token(it, ASSIGN_TOKEN) && consume_token(it, OPEN_BLOCK_TOKEN)) {
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
                PARSE_ERROR("Error during parsing of the array static element!");
                AST_unload(base);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            const_length = MAX(const_length, act_size++);
        }

        forward_token(it, 1);
    }

    /* Add variable information. Note here:
       Array, basically, is a pointer. That's why we increment the .ptr flag to 1. */
    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.v_id = VRTB_add_info(name->t->body, ARRAY_TYPE_TOKEN, name->sinfo.s_id, &name->t->flags, &smt->v);
    ARTB_add_info(
        name->sinfo.v_id, const_length, name->t->flags.heap, 
        type->t->t_type, &type->t->flags, &smt->a
    );
    
    /* Add RO or global array declaration to a
       section that belongs to. */
    if (
        name->t->flags.glob || 
        name->t->flags.ro
    ) {
        string_t* section = create_string(name->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(section, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
        destroy_string(section);
    }

    var_lookup(name, ctx, smt);
    return base;
}