#include <ast/devirt.h>

int AST_DVRT_init_ctx(devirt_ctx_t* ctx) {
    map_init(&ctx->templates, MAP_NO_CMP);
    queue_init(&ctx->to_impl);
    set_init(&ctx->impl, SET_NO_CMP);
    return 1;
}

int AST_DVRT_register_template(symbol_id_t f_id, ast_node_t* root, devirt_ctx_t* ctx) {
    return map_put(&ctx->templates, f_id, root);
}

static template_t* _create_template(symbol_id_t base_id, ast_node_t* root) {
    template_t* tmpl = (template_t*)mm_malloc(sizeof(template_t));
    if (!tmpl) return NULL;
    tmpl->f_id = base_id;
    tmpl->root = AST_copy_node(root, 0, 0, 1, NULL);
    return tmpl;
}

static int _unload_template(template_t* tmpl) {
    AST_unload(tmpl->root);
    return mm_free(tmpl);
}

int AST_DVRT_register_implementation(symbol_id_t f_id, symbol_id_t src_id, devirt_ctx_t* ctx) {
    if (set_has(&ctx->impl, (void*)f_id)) return 1;
    else set_add(&ctx->impl, (void*)f_id);
    ast_node_t* root;
    if (!map_get(&ctx->templates, src_id, (void**)&root)) return 0;
    template_t* tmpl = _create_template(f_id, root);
    if (!tmpl) return 0;
    return queue_push(&ctx->to_impl, tmpl);
}

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

// TODO: docs
ast_node_t* _implement_template(ast_node_t* root, symbol_id_t f_id, sym_table_t* smt, devirt_ctx_t* ctx) {
    if (f_id == NO_SYMBOL_ID) return root;
    func_info_t fi;
    if (!FNTB_get_info_id(f_id, &fi, &smt->f)) return root;
    ast_node_t* copy = AST_copy_node(root, 0, 0, 1, NULL);
    
    _find_type_usage(copy, copy, &fi.template.generic, smt);
    _find_function_declaration(copy->c, copy, smt);

    AST_DVRT_resolve_calls(copy, smt, ctx);
    copy->c->sinfo.v_id = f_id;
    return copy;
}

ast_node_t* AST_DVRT_pop_implementation(sym_table_t* smt, devirt_ctx_t* ctx) {
    template_t* template;
    if (!queue_pop(&ctx->to_impl, (void**)&template)) return NULL;
    ast_node_t* template_ast = template->root;
    ast_node_t* prepared = _implement_template(template_ast, template->f_id, smt, ctx);
    _unload_template(template);
    return prepared;
}

int AST_DVRT_unload_ctx(devirt_ctx_t* ctx) {
    set_free(&ctx->impl);
    map_free(&ctx->templates);
    queue_free_force_op(&ctx->to_impl, (int (*)(void *))_unload_template);
    return 1;
}
