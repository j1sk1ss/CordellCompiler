#include <sem/semantic.h>

static int _get_max_bitness(ast_node_t* r) {
    if (!r || !r->token) return 1;
    if (TKN_isoperand(r->token)) {
        if (!r->child) return -1;
        char lbitness = _get_max_bitness(r->child);
        if (!r->child->sibling) return -2;
        char rbitness = _get_max_bitness(r->child->sibling);
        return MAX(lbitness, rbitness);
    }

    if (!TKN_isnumeric(r->token)) return TKN_variable_bitness(r->token, 1); 
    else {
        unsigned long val = r->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(r->token->value) : r->token->value[0];
        if (val <= UCHAR_MAX) return 8;
        if (val <= USHRT_MAX) return 16;
        if (val <= UINT_MAX)  return 32;
        return 64;
    }
}

static int _find_ret(ast_node_t* n, token_t* rtype, int* match) {
    if (!n) return 0;
    for (ast_node_t* t = n; t; t = t->sibling) {
        if (TKN_isblock(t->token)) {
            _find_ret(t->child, rtype, match);
            continue;
        }

        if (t->token->t_type == RETURN_TOKEN) {
            ast_node_t* rval = t->child;
            int expected_bitness = TKN_variable_bitness(rtype, 1);
            int provided_bitness = _get_max_bitness(rval);

            if (provided_bitness != expected_bitness) {
                print_warn(
                    "Unmatched return type in line=%i. Should return bitness=%i, but provide bitness=%i",
                    rval->token->lnum, expected_bitness, provided_bitness
                );

                *match = 0;
                return 1;
            }

            if (TKN_isptr(rval->token) != TKN_isptr(rtype)) {
                print_warn(
                    "Unmatched pointer status at line=%i. Should be %s, but %s!",
                    rval->token->lnum, TKN_isptr(rtype) ? "ptr" : "stack", TKN_isptr(rval->token) ? "ptr" : "stack"
                );

                *match = 0;
                return 1;
            }
        }
    }

    return 1;
}

static int _check_function(ast_node_t* f) {
    ast_node_t* name = f->child;
    ast_node_t* rtype = name->child;
    if (!rtype) return 1;

    int is_correct = 1;
    _find_ret(name->sibling, rtype->token, &is_correct);
    return is_correct;
}

static int _find_func(ast_node_t* node) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (t->token && t->token->t_type == FUNC_TOKEN) {
            _check_function(t);
        }

        if (TKN_isblock(t->token)) {
            _find_func(t->child);
            continue;
        }
    }

    return 1;
}

int SMT_check_rettype(ast_ctx_t* sctx) {
    if (!sctx->r) return 0;
    _find_func(sctx->r);
    return 1;
}