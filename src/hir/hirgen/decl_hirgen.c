#include <hir/hirgens/hirgens.h>

static int _strdeclaration(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;
    HIR_BLOCK2(ctx, HIR_STRDECL, HIR_SUBJ_ASTVAR(name_node), HIR_SUBJ_STRING(val_node));
    return 1;
}

static int _arrdeclaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_type_node = size_node->sibling;
    ast_node_t* elems_node   = el_type_node->sibling;

    variable_info_t vi;
    if (VRTB_get_info_id(name_node->sinfo.v_id, &vi, &smt->v)) {
        hir_subject_t* elems = HIR_SUBJ_LIST();
        for (ast_node_t* e = elems_node; e; e = e->sibling) {
            hir_subject_t* el = HIR_generate_elem(e, ctx, smt);
            if (vi.glob && el->t == HIR_NUMBER) ARTB_add_elems(vi.v_id, str_atoi(el->storage.num.value), &smt->a);
            else if (!vi.glob) {
                list_add(&elems->storage.list.h, el);
                HIR_BLOCK1(ctx, HIR_VRUSE, el);
            }
        }

        HIR_BLOCK3(ctx, HIR_ARRDECL, HIR_SUBJ_ASTVAR(name_node), HIR_generate_elem(size_node, ctx, smt), elems);
    }

    return 1;
}

static int _starr_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, ctx, smt);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, ctx);
    return 1;
}

int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node = node->child;
    if (!TKN_one_slot(name_node->token)) {
        return _starr_declaration(node, ctx, smt);
    }

    HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(name_node));
    if (!name_node->sibling) return 1;
    return HIR_generate_assignment_block(node, ctx, smt);
}
