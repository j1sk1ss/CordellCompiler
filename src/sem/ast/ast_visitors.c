#include <sem/ast/ast_visitors.h>

static const char* _fmt_tkn_op(token_type_t t) {
    switch (t) {
        case I0_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN:    return "declaration";

        case ASSIGN_TOKEN:        return "=";
        case ADDASSIGN_TOKEN:     return "+=";
        case SUBASSIGN_TOKEN:     return "-=";
        case MULASSIGN_TOKEN:     return "*=";
        case DIVASSIGN_TOKEN:     return "/=";

        case OR_TOKEN:            return "||";
        case AND_TOKEN:           return "&&";
        case PLUS_TOKEN:          return "+";
        case BITOR_TOKEN:         return "|";
        case LOWER_TOKEN:         return "<";
        case MINUS_TOKEN:         return "-";
        case LARGER_TOKEN:        return ">";
        case DIVIDE_TOKEN:        return "/";
        case MODULO_TOKEN:        return "%%";
        case BITAND_TOKEN:        return "&";
        case BITXOR_TOKEN:        return "^";
        case LOWEREQ_TOKEN:       return "<=";
        case COMPARE_TOKEN:       return "==";
        case MULTIPLY_TOKEN:      return "*";
        case NCOMPARE_TOKEN:      return "!=";
        case LARGEREQ_TOKEN:      return ">=";
        case BITMOVE_LEFT_TOKEN:  return "<<";
        case BITMOVE_RIGHT_TOKEN: return ">>";
        default: break;
    }

    return "";
}

static const char* _fmt_tkn_type(token_t* t) {
    if (!t) return "";
    switch (t->t_type) {
        case I0_TYPE_TOKEN:  return !t->flags.ptr ? "i0"  : "ptr i0";
        case I8_VARIABLE_TOKEN:
        case I8_TYPE_TOKEN:  return !t->flags.ptr ? "i8"  : "ptr i8";
        case U8_VARIABLE_TOKEN:
        case U8_TYPE_TOKEN:  return !t->flags.ptr ? "u8"  : "ptr u8";
        case I16_VARIABLE_TOKEN:
        case I16_TYPE_TOKEN: return !t->flags.ptr ? "i16" : "ptr i16";
        case U16_VARIABLE_TOKEN:
        case U16_TYPE_TOKEN: return !t->flags.ptr ? "u16" : "ptr u16";
        case I32_VARIABLE_TOKEN:
        case I32_TYPE_TOKEN: return !t->flags.ptr ? "i32" : "ptr i32";
        case U32_VARIABLE_TOKEN:
        case U32_TYPE_TOKEN: return !t->flags.ptr ? "u32" : "ptr u32";
        case F32_VARIABLE_TOKEN:
        case F32_TYPE_TOKEN: return !t->flags.ptr ? "f32" : "ptr f32";
        case I64_VARIABLE_TOKEN:
        case I64_TYPE_TOKEN: return !t->flags.ptr ? "i64" : "ptr i64";
        case U64_VARIABLE_TOKEN:
        case U64_TYPE_TOKEN: return !t->flags.ptr ? "u64" : "ptr u64";
        case F64_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN: return !t->flags.ptr ? "f64" : "ptr f64";
        default: return "";
    }
}

int ASTWLKR_ro_assign(ast_node_t* nd, sym_table_t* smt) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) return 1;

    if (larg->token->flags.ro) {
        print_error("Read-only variable=%s assign! [line=%i]", larg->token->value, larg->token->lnum);
        return 0;
    }

    return 1;
}

int ASTWLKR_rtype_assign(ast_node_t* nd, sym_table_t* smt) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg || rarg->token->t_type != CALL_TOKEN) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(rarg->sinfo.v_id, &fi, &smt->f)) return 1;

    if (TKN_variable_bitness(fi.rtype->token, 1) != TKN_variable_bitness(rarg->token, 1)) {
        print_warn(
            "Function=%s return type=%s not match to %s type=%s! [line=%i]", fi.name,
            _fmt_tkn_type(fi.rtype->token), _fmt_tkn_op(nd->token->t_type), _fmt_tkn_type(larg->token),
            larg->token->lnum
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_not_init(ast_node_t* nd, sym_table_t* smt) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) {
        print_warn("Variable=%s without initialization! [line=%i]", larg->token->value, larg->token->lnum);
        return 0;
    }

    return 1;
}

static int _search_rexit_ast(ast_node_t* nd, int* found) {
    if (!nd) return 0;

    if (!nd->token) return 0;
    switch (nd->token->t_type) {
        case IF_TOKEN: {
            ast_node_t* cnd     = nd->child;
            ast_node_t* lbranch = cnd->sibling;
            ast_node_t* rbranch = lbranch->sibling;

            int nret = 0, lret = 0, rret = 0;
            _search_rexit_ast(lbranch, &lret);
            _search_rexit_ast(rbranch, &rret);
            _search_rexit_ast(nd->sibling, &nret);
            if (rret && lret) nret = 1;
            if (!rbranch) rret = 1;

            if (nret || (lret && rret)) *found = 1;
            return 1;
        }

        case SWITCH_TOKEN: {
            break;
        }

        case EXIT_TOKEN:
        case RETURN_TOKEN: {
            if (found) *found = 1;
            return 1;
        }

        default: {
            _search_rexit_ast(nd->child, found);
            _search_rexit_ast(nd->sibling, found);
            break;
        }
    }

    return 1;
}

int ASTWLKR_no_return(ast_node_t* nd, sym_table_t* smt) {
    int has_ret = 0;
    _search_rexit_ast(nd->child, &has_ret);
    if (!has_ret) {
        print_warn("Function=%s doesn't have return in all paths! [line=%i]", nd->child->token->value, nd->token->lnum);
    }

    return 1;
}

int ASTWLKR_no_exit(ast_node_t* nd, sym_table_t* smt) {
    int has_ret = 0;
    _search_rexit_ast(nd->child->child, &has_ret);
    if (!has_ret) {
        print_warn("Start doesn't have exit in all paths! [line=%i]", nd->token->lnum);
    }

    return 1;
}
