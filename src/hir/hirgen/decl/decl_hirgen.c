#include <hir/hirgens/hirgens.h>

static int _str_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name  = node->c;
    ast_node_t* value = name->siblings.n;
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

    array_info_t ai;
    variable_info_t vi;
    if (
        VRTB_get_info_id(name->sinfo.v_id, &vi, &smt->v) && 
        ARTB_get_info(vi.v_id, &ai, &smt->a) // TODO: Cast element to the array's element type?
    ) {
        hir_subject_t* init_elems = HIR_SUBJ_LIST();
        for (ast_node_t* e = elems; e; e = e->siblings.n) {
            hir_subject_t* el = HIR_generate_elem(e, ctx, smt);
            if (vi.vfs.glob) {
                ARTB_add_elems(vi.v_id, el->storage.num.value->to_llong(el->storage.num.value), &smt->a);
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

static int _starr_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (node->t->t_type == ARRAY_TYPE_TOKEN)    return _arr_declaration(node, ctx, smt);
    else if (node->t->t_type == STR_TYPE_TOKEN) return _str_declaration(node, ctx, smt);
    return 1;
}

int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    ast_node_t* name = node->c;
    if (!TKN_is_one_slot(name->t)) {
        return _starr_declaration(node, ctx, smt);
    }

    HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(name));
    HAS_ANNOTATION(POPARG_ANNOTATION, node, {
        HIR_BLOCK2(ctx, ctx->carry.val2, HIR_SUBJ_ASTVAR(name), HIR_SUBJ_CONST(ctx->carry.val1++));
        return 1;
    });

    if (!name->siblings.n) return 1;
    return HIR_generate_assignment_block(node, ctx, smt);
}
