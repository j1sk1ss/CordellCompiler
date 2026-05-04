#include <ast/devirt.h>

// TODO: docs
static int _update_variable_id(ast_node_t* node, symbol_id_t v_id, symbol_id_t nv_id, token_type_t t) {
    if (!node) return 0;
    _update_variable_id(node->siblings.n, v_id, nv_id, t);
    _update_variable_id(node->c, v_id, nv_id, t);
    if (!node->t) return 0;
    if (TKN_is_variable(node->t) && node->sinfo.v_id == v_id) {
        node->sinfo.v_id = nv_id;
        if (node->t->t_type == GENERIC_VARIABLE_TOKEN) node->t->t_type = t;
    }

    return 1;
}

// TODO: docs
static int _find_type_usage(ast_node_t* node, ast_node_t* root, map_t* types, sym_table_t* smt) {
    if (!node) return 0;
    _find_type_usage(node->siblings.n, root, types, smt);
    _find_type_usage(node->c, root, types, smt);
    if (!node->t) return 0;
    if (TKN_is_builtin_type(node->t) || node->t->t_type == GENERIC_TYPE_TOKEN) {
        long t;
        if (
            node->t->t_type == GENERIC_TYPE_TOKEN && 
            map_get(types, node->sinfo.v_id, (void**)&t)
        ) node->t->t_type = t;
        if (node->c) {
            variable_info_t vi;
            if (!VRTB_get_info_id(node->c->sinfo.v_id, &vi, &smt->v)) return 0;
            if (node->c->t->t_type == GENERIC_VARIABLE_TOKEN) node->c->t->t_type = TKN_get_var_from_type(t);
            node->c->sinfo.v_id = VRTB_add_copy(&vi, &smt->v);
            _update_variable_id(root, vi.v_id, node->c->sinfo.v_id, TKN_get_var_from_type(t));
        }
    }

    return 1;
}

// TODO: docs
static int _update_function_id(ast_node_t* node, symbol_id_t v_id, symbol_id_t nv_id) {
    if (!node) return 0;
    _update_function_id(node->siblings.n, v_id, nv_id);
    _update_function_id(node->c, v_id, nv_id);
    if (!node->t) return 0;
    if (node->t->t_type == FUNC_NAME_TOKEN && node->sinfo.v_id == v_id) {
        node->sinfo.v_id = nv_id;
    }

    return 1;
}

// TODO: docs
static int _find_function_declaration(ast_node_t* node, ast_node_t* root, sym_table_t* smt) {
    if (!node) return 0;
    _find_function_declaration(node->siblings.n, root, smt);
    _find_function_declaration(node->c, root, smt);
    if (!node->t) return 0;
    if (
        node->t->t_type == FUNC_TOKEN && 
        node->c && node->c->t->t_type == FUNC_NAME_TOKEN
    ) {
        func_info_t fi;
        if (FNTB_get_info_id(node->c->sinfo.v_id, &fi, &smt->f)) {
            node->c->sinfo.v_id = FNTB_add_copy(&fi, &smt->f);
            _update_function_id(root, fi.id, node->c->sinfo.v_id);
        }
    }

    return 1;
}

ast_node_t* AST_implement_template(ast_node_t* root, symbol_id_t f_id, sym_table_t* smt) {
    if (f_id == NO_SYMBOL_ID) return root;
    
    func_info_t fi;
    if (!FNTB_get_info_id(f_id, &fi, &smt->f)) return root;
    ast_node_t* copy = AST_copy_node(root, 0, 0, 1, NULL);
    
    _find_type_usage(copy, copy, &fi.template.generic, smt);
    _find_function_declaration(copy->c, copy, smt);

    AST_resolve_calls(copy, smt);
    copy->c->sinfo.v_id = f_id;
    return copy;
}

int AST_destroy_template_implementation(ast_node_t* node) {
    return AST_unload(node);
}
