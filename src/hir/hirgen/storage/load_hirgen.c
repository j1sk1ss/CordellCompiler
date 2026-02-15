#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node) {
    hir_subject_t* res = NULL;
    switch (node->t->t_type) {
        case STRING_VALUE_TOKEN:          res = HIR_SUBJ_STRING(node);           break;
        case UNKNOWN_NUMERIC_TOKEN:       res = HIR_SUBJ_NUMBER(node->t->body);  break;
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: res = HIR_SUBJ_FNUMBER(node->t->body); break;
        default:                          res = HIR_SUBJ_ASTVAR(node);           break;
    }

    return res;
}
