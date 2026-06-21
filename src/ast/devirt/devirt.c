#include <ast/devirt.h>

int AST_DVRT_init_ctx(devirt_ctx_t* ctx) {
    map_init(&ctx->templates, MAP_NO_CMP);
    queue_init(&ctx->to_impl);
    set_init(&ctx->impl, SET_NO_CMP);
    return 1;
}

int AST_DVRT_register_template(symbol_id_t f_id, ast_node_t* root, devirt_ctx_t* ctx) {
    ast_node_t* prev;
    if (map_get(&ctx->templates, f_id, (void**)&prev) && prev) return 1;
    return map_put(&ctx->templates, f_id, root);
}

static inline template_t* _create_template(symbol_id_t base_id, ast_node_t* root) {
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

static int _recursive_walk(
    ast_node_t* node, symbol_id_t v_id, symbol_id_t nv_id, token_type_t t, 
    void (*process)(ast_node_t*, symbol_id_t, symbol_id_t, token_type_t)
) {
    if (!node) return 0;
    _recursive_walk(node->siblings.n, v_id, nv_id, t, process);
    _recursive_walk(node->c, v_id, nv_id, t, process);
    if (!node->t) return 0;
    process(node, v_id, nv_id, t);
    return 1;
}

/* Change all encounters of a variable with a copy ID.
Params:
    - `node` - Root node.
    - `v_id` - Old variable Id.
    - `nv_id` - New variable Id.
    - `t` - New token type for a generic variables.

Returns 1 if succeeds. */
static void _update_variable_id(ast_node_t* node, symbol_id_t v_id, symbol_id_t nv_id, token_type_t t) {
    if (TKN_is_variable(node->t) && node->sinfo.v_id == v_id) {
        node->sinfo.v_id = nv_id;
        if (node->t->t_type == GENERIC_VARIABLE_TOKEN) {
            node->t->t_type = t;
        }
    }
}

/* Find any occurance of a generic type and replace it with implementation map.
Params:
    - `node` - Root node.
    - `root` - Root node copy.
    - `types` - Types map for a function.
    - `smt` - Symtable.

Returns 1 if succeeds. */
static int _find_type_usage_and_replace(ast_node_t* node, ast_node_t* root, map_t* types, sym_table_t* smt) {
    if (!node) return 0;
    _find_type_usage_and_replace(node->siblings.n, root, types, smt);
    _find_type_usage_and_replace(node->c, root, types, smt);
    if (!node->t) return 0;
    if (TKN_is_builtin_type(node->t) || node->t->t_type == GENERIC_TYPE_TOKEN) {
        long t = GENERIC_TYPE_TOKEN;
        if (
            node->t->t_type == GENERIC_TYPE_TOKEN && 
            map_get(types, node->sinfo.t_id, (void**)&t)
        ) node->t->t_type = t; /* replace just a type node */
        if (node->c) {         /* update the declared variable and its id */
            variable_info_t vi;
            if (!VRTB_get_info_id(node->c->sinfo.v_id, &vi, &smt->v)) return 0;
            if (node->c->t->t_type == GENERIC_VARIABLE_TOKEN) node->c->t->t_type = TKN_get_var_from_type(t);
            node->c->sinfo.v_id = VRTB_add_copy(&vi, &smt->v);
            _recursive_walk(root, vi.v_id, node->c->sinfo.v_id, TKN_get_var_from_type(t), _update_variable_id);
        }
    }

    return 1;
}

/* Find all function names and replace its Id with a new one.
Params:
    - `node` - Root node.
    - `v_id` - Old function Id.
    - `nv_id` - New function Id.

Returns 1 if succeeds. */
static void _update_function_id(ast_node_t* node, symbol_id_t v_id, symbol_id_t nv_id, __attribute__((unused)) token_type_t t) {
    if (
        (node->t->t_type == FUNC_NAME_TOKEN || node->t->t_type == LAMBDA_FUNCTION_TOKEN) && 
        node->sinfo.v_id == v_id
    ) node->sinfo.v_id = nv_id;
}

/* Find any local function declaration (lambda as well) and replace it with a copy.
Params:
    - `node` - Root node.
    - `root` - Root node copy.
    - `smt` - Symtable.
    - `ctx` - Devirt context.

Returns 1 if succeeds. */
static int _find_function_declaration_and_replace(ast_node_t* node, ast_node_t* root, sym_table_t* smt, devirt_ctx_t* ctx) {
    if (!node) return 0;
    _find_function_declaration_and_replace(node->siblings.n, root, smt, ctx);
    _find_function_declaration_and_replace(node->c, root, smt, ctx);
    if (!node->t) return 0;
    ast_node_t *name = NULL, *args = NULL, *rtype = NULL;
    switch (node->t->t_type) {
        case FUNC_TOKEN: {
            name  = node->c;
            rtype = name->c;
            args  = name->siblings.n;
            break;
        }
        case LAMBDA_FUNCTION_TOKEN: {
            name = node;
            args = name->c;
            break;
        }
        default: break;
    }

    if (name) {
        func_info_t fi;
        if (FNTB_get_info_id(name->sinfo.v_id, &fi, &smt->f)) {
            name->sinfo.v_id = FNTB_add_copy(&fi, &smt->f);
            FNTB_update_func(name->sinfo.v_id, NULL, FNTB_SET_GENERIC(!fi.flags.generic), args, rtype, &smt->f);
            _recursive_walk(root, fi.id, name->sinfo.v_id, CUSTOM_TYPE_TOKEN, _update_function_id);
            if (fi.flags.generic) {
                AST_DVRT_register_template(name->sinfo.v_id, node, ctx);
            }
        }
    }

    return 1;
}

/* Take a copy of a template and replace its types with the implementation types.
Params:
    - `root` - Template root node.
    - `f_id` - Implementation function Id.
    - `smt` - Symtable.
    - `ctx` - Devir context.

Returns 1 if succeeds. */
ast_node_t* _implement_template(ast_node_t* root, symbol_id_t f_id, sym_table_t* smt, devirt_ctx_t* ctx) {
    if (f_id == NO_SYMBOL_ID) return root;
    func_info_t fi;
    if (!FNTB_get_info_id(f_id, &fi, &smt->f)) return root;
    ast_node_t* copy = AST_copy_node(root, 0, 0, 1, NULL);
    
    _find_type_usage_and_replace(copy, copy, &fi.template.generic, smt);
    _find_function_declaration_and_replace(copy->c, copy, smt, ctx);

    AST_DVRT_resolve_calls(copy, smt, ctx);
    copy->c->sinfo.v_id = f_id;
    return copy;
}

int AST_DVRT_pop_implementation(sym_table_t* smt, devirt_ctx_t* ctx, ast_node_t** out) {
    template_t* template;
    if (!queue_pop(&ctx->to_impl, (void**)&template)) return 0;
    ast_node_t* template_ast = template->root;
    if (!template_ast) {
        print_warn("Pattern has a NULL root which means, there is no pattern implementation!");
        *out = NULL;
    }
    else {
        ast_node_t* prepared = _implement_template(template_ast, template->f_id, smt, ctx);
        *out = prepared;
    }

    _unload_template(template);
    return 1;
}

int AST_DVRT_unload_ctx(devirt_ctx_t* ctx) {
    set_free(&ctx->impl);
    map_free(&ctx->templates);
    queue_free_force_op(&ctx->to_impl, (int (*)(void *))_unload_template);
    return 1;
}