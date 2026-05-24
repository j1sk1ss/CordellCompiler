#include <hir/hirgens/hirgens.h>

static int _str_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name  = node->c;
    ast_node_t* size  = name->siblings.n;
    ast_node_t* type  = size->siblings.n;
    ast_node_t* value = type->siblings.n;
    HIR_BLOCK2(ctx, HIR_STRDECL, HIR_SUBJ_ASTVAR(name), HIR_SUBJ_STRING(value));

    variable_info_t vi;
    if (VRTB_get_info_id(name->sinfo.v_id, &vi, &smt->v) && vi.vfs.glob) {
        char* head = value->t->body->body;
        while (head && *head) ARTB_add_elems(vi.v_id, *(head++), &smt->a);
        ARTB_add_elems(vi.v_id, 0, &smt->a);
    }

    return 1;
}

static int _arr_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name  = node->c;
    ast_node_t* size  = name->siblings.n;
    ast_node_t* type  = size->siblings.n;
    ast_node_t* elems = type->siblings.n;

    if (elems->t->t_type == STRING_VALUE_TOKEN) {
        return _str_declaration(node, ctx, smt);
    }

    array_info_t ai;
    variable_info_t vi;
    if (
        VRTB_get_info_id(name->sinfo.v_id, &vi, &smt->v) && 
        ARTB_get_info(vi.v_id, &ai, &smt->a)
    ) {
        hir_subject_t* init_elems = HIR_SUBJ_LIST();
        for (ast_node_t* e = elems; e; e = e->siblings.n) {
            hir_subject_t* el = HIR_generate_elem(e, ctx, smt);
            if (!el) continue;
            if (vi.vfs.glob) {
                if (!HIR_is_defined_type(el->t)) HIRGEN_ERROR(ctx, "Global initializer element must be a constant numeric value!");
                else ARTB_add_elems(vi.v_id, el->storage.num.value->to_llong(el->storage.num.value), &smt->a);
                HIR_unload_subject(el);
            }
            else {
                hir_subject_t* element = el;
                if (!HIR_is_defined_type(element->t)) {
                    HIR_BLOCK1(ctx, HIR_VRUSE, element);
                    element = HIR_copy_subject(element);
                }

                list_add(&init_elems->storage.list.h, element);
            }
        }

        HIR_BLOCK3(ctx, HIR_ARRDECL, HIR_SUBJ_ASTVAR(name), HIR_generate_elem(size, ctx, smt), init_elems);
    }

    return 1;
}

static int _cnt_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    type_info_t ti;
    if (!TPTB_get_info_id(node->sinfo.t_id, &ti, &smt->t)) return 0;
    HIR_BLOCK3(ctx, HIR_ARRDECL, HIR_SUBJ_ASTVAR(node->c), HIR_SUBJ_CONST(ti.memory.size), HIR_SUBJ_LIST());
    return 1;
}

static inline int _starr_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    switch (node->t->t_type) {
        case ARRAY_TYPE_TOKEN:  return _arr_declaration(node, ctx, smt);
        case CUSTOM_TYPE_TOKEN: return _cnt_declaration(node, ctx, smt);
        default: return 1;
    }
}

int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    ast_node_t* name = node->c;
    if (!TKN_is_one_slot(name->t)) {
        return _starr_declaration(node, ctx, smt);
    }

    /* Don't declare a variable if it is global.
       All essential info already in the symtable. */
    if (!TKN_in_stack(name->t)) {
        return 1;
    }
    
    HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(name));
    HAS_ANNOTATION(POPARG_ANNOTATION, node, {
        if (!ctx->carry.ptr3) return 0;
        hir_subject_t* decl = HIR_SUBJ_ASTVAR(name);
        HIR_BLOCK2(ctx, HIR_GDREF, decl, HIR_copy_subject((hir_subject_t*)ctx->carry.ptr3));
        hir_subject_t* res = HIR_SUBJ_TMPVAR(
            ((hir_subject_t*)ctx->carry.ptr3)->t, 
            VRTB_add_info(NULL, HIR_get_tmptkn_type(((hir_subject_t*)ctx->carry.ptr3)->t), NO_SYMBOL_ID, NULL, &smt->v)
        );
        res->ptr = ((hir_subject_t*)ctx->carry.ptr3)->ptr;
        HIR_BLOCK3(
            ctx, HIR_iADD, res, HIR_copy_subject((hir_subject_t*)ctx->carry.ptr3), 
            HIR_SUBJ_CONST(CONF_get_full_bytness())
        );
        HIR_BLOCK2(ctx, HIR_STORE, HIR_copy_subject((hir_subject_t*)ctx->carry.ptr3), res);
        return 1;
    });

    if (!name->siblings.n) return 1;
    return HIR_generate_assignment_block(node, ctx, smt);
}
