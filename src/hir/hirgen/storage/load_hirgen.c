#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* res = NULL;
    switch (node->t->t_type) {
        case UNKNOWN_NUMERIC_TOKEN: {
            unsigned long long number = node->t->body->to_ullong(node->t->body, 10);
            if (number < CHAR_MAX)          res = HIR_SUBJ_NUMBER(HIR_I8NUMBER, node->t->body);
            else if (number < UCHAR_MAX)    res = HIR_SUBJ_NUMBER(HIR_U8NUMBER, node->t->body);
            else if (number < SHRT_MAX)     res = HIR_SUBJ_NUMBER(HIR_I16NUMBER, node->t->body);
            else if (number < USHRT_MAX)    res = HIR_SUBJ_NUMBER(HIR_U16NUMBER, node->t->body);
            else if (number < INT_MAX)      res = HIR_SUBJ_NUMBER(HIR_I32NUMBER, node->t->body);
            else if (number < UINT_MAX)     res = HIR_SUBJ_NUMBER(HIR_U32NUMBER, node->t->body);
            else if (number < LONG_MAX)     res = HIR_SUBJ_NUMBER(HIR_I64NUMBER, node->t->body);
            else                            res = HIR_SUBJ_NUMBER(HIR_U64NUMBER, node->t->body);
            break;
        }
        case STRING_VALUE_TOKEN:          res = HIR_SUBJ_STRING(node);           break;
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: res = HIR_SUBJ_FNUMBER(node->t->body); break;
        case CALL_ADDR_TOKEN: {
            res = HIR_SUBJ_TMPVAR(HIR_TMPVARI0, VRTB_add_info(NULL, TMP_I0_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v));
            res->ptr = 1;
            HIR_BLOCK2(ctx, HIR_REF, res, HIR_SUBJ_FUNCNAME(node));
            break;
        }
        default: res = HIR_SUBJ_ASTVAR(node); break;
    }

    return res;
}
