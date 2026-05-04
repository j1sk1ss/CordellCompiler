#include <ast/devirt.h>

static int _find_and_register_resolved_call(ast_node_t* node, sym_table_t* smt) {
    if (!node) return 0;
    _find_and_register_resolved_call(node->siblings.n, smt);
    _find_and_register_resolved_call(node->c, smt);
    if (!node->t) return 0;
    if (
        node->t->t_type == CALLING_TOKEN      &&
        node->c->t->t_type == FUNC_NAME_TOKEN &&
        node->c->c
    ) {
        list_t types;
        list_init(&types);
        ast_node_t* type_node = node->c->c;
        while (type_node) {
            if (type_node->t->t_type == GENERIC_TYPE_TOKEN) {
                list_free(&types);
                return 0;
            }

            list_add(&types, (void*)type_node->t->t_type);
            type_node = type_node->siblings.n;
        }

        string_t* section = SCTB_get_section_name(node->c->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
        if (!section) section = create_string(CONF_get_code_section());
        else section = section->copy(section);

        node->c->sinfo.v_id = FNTB_create_resolved_copy(node->c->sinfo.v_id, &types, &smt->f);
        node->c->sinfo.s_id = NO_SYMBOL_ID;
        
        SCTB_add_to_section(section, node->c->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
        destroy_string(section);
        list_free(&types);
    }
 
    return 1;
}

int AST_resolve_calls(ast_node_t* root, sym_table_t* smt) {
    return _find_and_register_resolved_call(root, smt);
}
