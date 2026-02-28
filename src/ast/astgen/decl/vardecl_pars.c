/* Declaration statement parser.
   - <type> <name> = decl; */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_variable_declaration(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a node for the variable declaration type! <type> <name>!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (!name) {
        PARSE_ERROR("Can't create a node for the variable's name! <type> <name>!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, name);

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.v_id = VRTB_add_info(name->t->body, node->t->t_type, name->sinfo.s_id, &name->t->flags, &smt->v);
    if (
        node->t->t_type == STR_TYPE_TOKEN
    ) ARTB_add_info(name->sinfo.v_id, 0, 0, I8_TYPE_TOKEN, &node->t->flags, &smt->a);

    annotations_summary_t annots = { .align = CONF_get_full_bytness(), .section = NULL };
    ANNOT_read_annotations(&ctx->annots, &annots);
    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.align, &smt->v);
    if (
        name->t->flags.glob || 
        name->t->flags.ro
    ) {
        if (!annots.section) annots.section = create_string(name->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    if (consume_token(it, ASSIGN_TOKEN)) {
        forward_token(it, 1);
        ast_node_t* value_node = cpl_parse_expression(it, ctx, smt, 1);
        if (!value_node) {
            PARSE_ERROR("Error during parsing of the declaration statement!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (node->t->t_type == STR_TYPE_TOKEN) { /* Special case for a string variable.
                                                    The reason why we care - a string isn't a regular array.
                                                    Also, we can't separate the string from a variable given the
                                                    ability of string arguments, etc. */
            ARTB_update_info(
                name->sinfo.v_id, value_node->t->body->len(value_node->t->body) + 1, 
                0, I8_TYPE_TOKEN, &node->t->flags, &smt->a
            );
            
            STTB_update_info(value_node->sinfo.v_id, NULL, STR_ARRAY_VALUE, &smt->s);
        }

        AST_add_node(node, value_node);
    }

    ANNOT_destroy_summary(&annots);
    var_lookup(name, ctx, smt);
    return node;
}
