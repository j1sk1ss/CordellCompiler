#include <semantic.h>

static int _check_exp_bitness(ast_node_t* r) {
    if (!r || !r->token) return 1;
    if (VRS_isoperand(r->token) || VRS_isdecl(r->token)) {
        if (!r->child) return -1;
        char lbitness = _check_exp_bitness(r->child);
        if (!r->child->sibling) return -2;
        char rbitness = _check_exp_bitness(r->child->sibling);
        if (lbitness < rbitness) {
            print_warn(
                "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?",
                r->child->token->lnum, lbitness, r->child->token->value, rbitness, r->child->sibling->token->value
            );
        }

        return MAX(lbitness, rbitness);
    }

    if (VRS_isnumeric(r->token)) {
        unsigned long val = r->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(r->token->value) : r->token->value[0];
        if (val <= UCHAR_MAX) return 8;
        if (val <= USHRT_MAX) return 16;
        if (val <= UINT_MAX)  return 32;
        return 64;
    }
    else {
        return VRS_variable_bitness(r->token, 1);
    }
}

int SMT_check_bitness(ast_node_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (ast_node_t* t = node; t; t = t->sibling) {
        SMT_check_bitness(t->child);
        if (!t->token) continue;
        _check_exp_bitness(t);
    }
    
    return result;
}
