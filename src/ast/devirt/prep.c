#include <ast/devirt.h>

/* Search for templates in an AST tree and register them.
Params:
    - `node` - Root node.
    - `smt` - Symtable.
    - `ctx` - Devirt context.

Returns 1 if succeeds. */
static int _find_and_register_template(ast_node_t* node, sym_table_t* smt, devirt_ctx_t* ctx) {
    if (!node) return 0;
    _find_and_register_template(node->siblings.n, smt, ctx);
    _find_and_register_template(node->c, smt, ctx);
    if (!node->t || !node->c) return 0;

    func_info_t fi;
    if (
        !FNTB_get_info_id(node->c->sinfo.v_id, &fi, &smt->f) ||
        !fi.flags.generic
    ) return 0;

    switch (node->t->t_type) {
        case FUNC_TOKEN:
        case FUNC_PROT_TOKEN: AST_DVRT_register_template(fi.id, node->t->t_type == FUNC_TOKEN ? node : NULL, ctx);
        default: break;
    }

    return 1;
}

int AST_DVRT_find_templates(ast_node_t* root, sym_table_t* smt, devirt_ctx_t* ctx) {
    return _find_and_register_template(root, smt, ctx);
}

/* Find a call of a generic function and create an implementation.
Params:
    - `node` - Root node.
    - `smt` - Symtable.
    - `ctx` - Devirt context.

Returns 1 if succeeds. */
static int _find_and_register_resolved_call(ast_node_t* node, sym_table_t* smt, devirt_ctx_t* ctx) {
    if (!node) return 0;
    _find_and_register_resolved_call(node->siblings.n, smt, ctx);
    _find_and_register_resolved_call(node->c, smt, ctx);
    if (!node->t) return 0;

    ast_node_t* name = NULL;
    if (
        node->t->t_type == CALLING_TOKEN &&
        node->c->t->t_type == FUNC_NAME_TOKEN
    ) name = node->c;
    else if (
        node->t->t_type == CALL_ADDR_TOKEN
    ) name = node;

    if (name && name->c) {
        func_info_t fi;
        if (
            !FNTB_get_info_id(name->sinfo.v_id, &fi, &smt->f) ||
            !fi.flags.generic
        ) return 1;

        list_t types;
        list_init(&types);
        ast_node_t* type_node = name->c;
        while (type_node) {
            if (type_node->t->t_type == GENERIC_TYPE_TOKEN) {
                list_free(&types);
                return 0;
            }

            list_add(&types, (void*)type_node->t->t_type);
            type_node = type_node->siblings.n;
        }
        
        string_t* section = SCTB_get_section_name(name->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
        if (!section) section = create_string(CONF_get_code_section());
        else          section = section->copy(section);
        
        symbol_id_t base = name->sinfo.v_id;
        name->sinfo.v_id = FNTB_create_resolved_copy(base, &types, &smt->f);
        name->sinfo.s_id = NO_SYMBOL_ID;
        AST_DVRT_register_implementation(name->sinfo.v_id, base, ctx);
        
        SCTB_add_to_section(section, FIELD_NO_CHANGE, name->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
        destroy_string(section);
        list_free(&types);
    }
 
    return 1;
}

int AST_DVRT_resolve_calls(ast_node_t* root, sym_table_t* smt, devirt_ctx_t* ctx) {
    return _find_and_register_resolved_call(root, smt, ctx);
}

static int _find_and_cut_container_function(ast_node_t* node, string_t* container, queue_t* funcs) {
    if (!node) return 0;
    int found_func = 0;
    if (
        node->t && container &&
        (node->t->t_type == FUNC_PROT_TOKEN || node->t->t_type == FUNC_TOKEN)
    ) found_func = 1;
    
    _find_and_cut_container_function(node->siblings.n, container, funcs);
    if (found_func) {
        if (node->p) AST_remove_node(node->p, node);
        queue_push(funcs, node);
        return 1;
    }

    if (
        node->t && 
        node->t->t_type == CONTAINER_TOKEN
    ) container = node->t->body;
    _find_and_cut_container_function(node->c, container, funcs);
    return 1;
}

int AST_DVRT_move_container_functions(ast_node_t* root, queue_t* out) {
    return _find_and_cut_container_function(root, NULL, out);
}
