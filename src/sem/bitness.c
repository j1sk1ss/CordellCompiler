#include <sem/semantic.h>

static int _check_exp_bitness(ast_node_t* r, sym_table_t* smt) {
    if (!r || !r->token) return 1;
    if (VRS_isoperand(r->token) || VRS_isdecl(r->token)) {
        if (!r->child) return -1;
        char lbitness = _check_exp_bitness(r->child, smt);
        if (!r->child->sibling) return -2;
        char rbitness = _check_exp_bitness(r->child->sibling, smt);
        if (lbitness < rbitness) {
            print_warn(
                "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?",
                r->child->token->lnum, lbitness, r->child->token->value, rbitness, r->child->sibling->token->value
            );
        }

        return MAX(lbitness, rbitness);
    }

    if (VRS_isnumeric(r->token)) {
        long val = r->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(r->token->value) : r->token->value[0];
        if (val <= UCHAR_MAX) return 8;
        if (val <= USHRT_MAX) return 16;
        if (val <= UINT_MAX)  return 32;
        return 64;
    }
    else if (r->token->t_type == CALL_TOKEN) {
        func_info_t finfo;
        if (FNTB_get_info(r->token->value, &finfo, &smt->f) && finfo.rtype) return VRS_variable_bitness(finfo.rtype->token, 1);
        else return 8;
    }
    else {
        return VRS_variable_bitness(r->token, 1);
    }
}

int _check_bitness(ast_node_t* r, sym_table_t* smt) {
    if (!r) return 1;
    for (ast_node_t* t = r; t; t = t->sibling) {
        SMT_check_bitness(t->child, smt);
        if (!t->token) continue;
        _check_exp_bitness(t, smt);
    }

    return 1;
}

int SMT_check_bitness(syntax_ctx_t* sctx, sym_table_t* smt) {
    if (!sctx->r) return 0;
    return _check_bitness(sctx->r, smt);
}
