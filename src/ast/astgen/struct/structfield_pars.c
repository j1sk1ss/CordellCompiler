#include <ast/astgen/astgen.h>

symbol_id_t _extract_base_type(ast_node_t* node, sym_table_t* smt) {
    switch (node->t->t_type) {
        case UNKNOWN_STRING_TOKEN: {
            break;
        }
        case CONVERT_TOKEN: return _extract_base_type(node->c, smt);
        case DOTTING_TOKEN: return _extract_base_type(node->c->siblings.n, smt);
        default: {
            break;
        }
    }

    return -1;
}

ast_node_t* cpl_parse_structfield_access(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* field  = NULL;
    ast_node_t* source = (ast_node_t*)carry;
    if (!source) {
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);

    type_info_t ti;
    if (TPTB_get_info_id(_extract_base_type(source, smt), &ti, &smt->t)) {
        foreach (type_entry_info_t* tinf, &ti.entries) {
            variable_info_t vi;
            if (VRTB_get_info_id(tinf->vid, &vi, &smt->v) && vi.name->equals(vi.name, CURRENT_TOKEN->body)) {
                field = AST_create_node(CURRENT_TOKEN);
                if (vi.ctype >= 0) field->t->t_type = STRUCT_FIELD_STRUCT;
                else               field->t->t_type = STRUCT_FIELD_PRIMITIVE;
            }
        }
    }

    if (consume_token(it, DOT_TOKEN)) {
        ast_node_t* subfield = cpl_parse_structfield_access(it, ctx, smt, (long)field);
        if (subfield) AST_add_node(field, subfield);
        else {
            PARSE_ERROR("Structure field parsing error!");
            AST_unload(field);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }

    return field;
}
