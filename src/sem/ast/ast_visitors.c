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
        case ARR_VARIABLE_TOKEN:    return "array";
        case STR_VARIABLE_TOKEN:    return "string";
        case UNKNOWN_NUMERIC_TOKEN: return "number";
        case I0_TYPE_TOKEN:         return !t->flags.ptr ? I0_VARIABLE  : "ptr i0";
        case I8_VARIABLE_TOKEN:
        case I8_TYPE_TOKEN:         return !t->flags.ptr ? I8_VARIABLE  : "ptr i8";
        case U8_VARIABLE_TOKEN:
        case U8_TYPE_TOKEN:         return !t->flags.ptr ? U8_VARIABLE  : "ptr u8";
        case I16_VARIABLE_TOKEN:
        case I16_TYPE_TOKEN:        return !t->flags.ptr ? I16_VARIABLE : "ptr i16";
        case U16_VARIABLE_TOKEN:
        case U16_TYPE_TOKEN:        return !t->flags.ptr ? U16_VARIABLE : "ptr u16";
        case I32_VARIABLE_TOKEN:
        case I32_TYPE_TOKEN:        return !t->flags.ptr ? I32_VARIABLE : "ptr i32";
        case U32_VARIABLE_TOKEN:
        case U32_TYPE_TOKEN:        return !t->flags.ptr ? U32_VARIABLE : "ptr u32";
        case F32_VARIABLE_TOKEN:
        case F32_TYPE_TOKEN:        return !t->flags.ptr ? F32_VARIABLE : "ptr f32";
        case I64_VARIABLE_TOKEN:
        case I64_TYPE_TOKEN:        return !t->flags.ptr ? I64_VARIABLE : "ptr i64";
        case U64_VARIABLE_TOKEN:
        case U64_TYPE_TOKEN:        return !t->flags.ptr ? U64_VARIABLE : "ptr u64";
        case F64_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:        return !t->flags.ptr ? F64_VARIABLE : "ptr f64";
        default: return "";
    }
}

static int _restore_code(ast_node_t* nd, ast_node_t* underscore) {
    if (nd == underscore) fprintf(stdout, "--->");
    if (TKN_isdecl(nd->token)) {
        fprintf(stdout, "%s %s", _fmt_tkn_type(nd->token), nd->child->token->value);
        if (nd->child->sibling) {
            fprintf(stdout, " = ");
            _restore_code(nd->child->sibling, underscore);
        }

        fprintf(stdout, ";");
    }
    else if (TKN_isoperand(nd->token)) {
        if (nd->child) _restore_code(nd->child, underscore);
        fprintf(stdout, " %s ", nd->token->value);
        if (nd->child->sibling) _restore_code(nd->child->sibling, underscore);
        fprintf(stdout, ";");
    }
    else if (nd->token->t_type == FUNC_TOKEN) {
        fprintf(stdout, "function %s(", nd->child->token->value);
        ast_node_t* p = nd->sibling->child;
        for (; p->token && p->token->t_type != SCOPE_TOKEN; p = p->sibling) {
            _restore_code(p, underscore);
            if (p->sibling->token && p->sibling->token->t_type != SCOPE_TOKEN) fprintf(stdout, ", ");
        }

        fprintf(stdout, ") ");
        if (nd->child->child) fprintf(stdout, "=> %s ", _fmt_tkn_type(nd->child->child->token));
        fprintf(stdout, "{ ... ");
    }
    else if (nd->token->t_type == CALL_TOKEN) {
        fprintf(stdout, "%s(", nd->token->value);
        ast_node_t* p = nd->child;
        for (; p; p = p->sibling) {
            _restore_code(p, underscore);
            if (p->sibling) fprintf(stdout, ", ");
        }

        fprintf(stdout, ");");
    }

    if (
        TKN_isnumeric(nd->token) || 
        TKN_isvariable(nd->token)
    ) fprintf(stdout, "%s", nd->token->value);

    return 1;
}

int ASTWLKR_ro_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) return 1;

    if (larg->token->flags.ro) {
        SEMANTIC_ERROR(
            " [line=%i] Read-only variable=%s assign!", 
            larg->token->lnum, larg->token->value
        );

        _restore_code(nd, larg);
        fprintf(stdout, "\n\n");
        return 0;
    }

    return 1;
}

int ASTWLKR_rtype_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg || rarg->token->t_type != CALL_TOKEN) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(rarg->sinfo.v_id, &fi, &smt->f)) return 1;
    if (!fi.rtype) return 1;

    if (TKN_variable_bitness(fi.rtype->token, 1) != TKN_variable_bitness(larg->token, 1)) {
        SEMANTIC_WARNING(
            " [line=%i] Function=%s return type=%s not match to %s type=%s!", larg->token->lnum, fi.name,
            _fmt_tkn_type(fi.rtype->token), _fmt_tkn_op(nd->token->t_type), _fmt_tkn_type(larg->token)
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_not_init(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) {
        SEMANTIC_WARNING(
            " [line=%i] Variable=%s without initialization!", 
            larg->token->lnum, larg->token->value
        );

        _restore_code(nd, larg);
        fprintf(stdout, "\n\n");
        return 0;
    }

    return 1;
}

static int _get_token_bitness(token_t* tkn) {
    if (!TKN_isnumeric(tkn)) return TKN_variable_bitness(tkn, 1);
    else {
        int num_bitness = 64;
        int val = str_atoi(tkn->value);
        if (val <= UCHAR_MAX) num_bitness = 8;
        else if (val <= USHRT_MAX) num_bitness = 16;
        else if (val <= UINT_MAX)  num_bitness = 32;
        return num_bitness;
    }
}

static int _check_assign_types(const char* msg, token_t* l, token_t* r) {
    if (!l || !r) return 0;
    if (_get_token_bitness(l) < _get_token_bitness(r)) {
        if (TKN_isnumeric(r)) SEMANTIC_WARNING(
            " [line=%i] %s of %s with %s (Number bitness is=%i, but %s can handle bitness=%i)!", r->lnum, msg,
            l->value, r->value, _get_token_bitness(r), _fmt_tkn_type(l), _get_token_bitness(l)
        );
        else SEMANTIC_WARNING(
            " [line=%i] %s of %s with %s! %s can't handle %s!", r->lnum, msg,
            l->value, r->value, _fmt_tkn_type(l), _fmt_tkn_type(r)
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) return 1;
    if (!_check_assign_types("Illegal declaration", larg->token, rarg->token)) {
        _restore_code(nd, rarg);
        fprintf(stdout, "\n\n");
        return 0;
    }

    return 1;
}

static int _search_rexit_ast(ast_node_t* nd, int* found) {
    if (!nd) return 0;
    if (!nd->token) {
        _search_rexit_ast(nd->child, found);
        return 0;
    }

    switch (nd->token->t_type) {
        case IF_TOKEN:
        case WHILE_TOKEN: {
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
            int nret = 0, caseret = 0;
            ast_node_t* cnd   = nd->child;
            ast_node_t* cases = cnd->sibling->child;
            for (; cases; cases = cases->sibling) {
                int curret = 0;
                _search_rexit_ast(cases->child, &curret);
                caseret = caseret && curret;
            }

            _search_rexit_ast(nd->sibling, &nret);
            if (nret || caseret) *found = 1;
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

int ASTWLKR_no_return(AST_VISITOR_ARGS) {
    int has_ret = 0;
    _search_rexit_ast(nd->child, &has_ret);
    if (!has_ret) {
        SEMANTIC_WARNING(
            " [line=%i] Function=%s doesn't have return in all paths!", 
            nd->token->lnum, nd->child->token->value
        );

        _restore_code(nd, NULL);
        fprintf(stdout, "\n\n");
    }

    return 1;
}

int ASTWLKR_no_exit(AST_VISITOR_ARGS) {
    int has_ret = 0;
    _search_rexit_ast(nd->child, &has_ret);
    if (!has_ret) {
        SEMANTIC_WARNING(" [line=%i] Start doesn't have exit in all paths!", nd->token->lnum);
        _restore_code(nd, NULL);
        fprintf(stdout, "\n\n");
    }

    return 1;
}

int ASTWLKR_not_enough_args(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;

    ast_node_t* provided_arg = nd->child;
    ast_node_t* expected_arg = fi.args->child;
    for (
        ; provided_arg && expected_arg && expected_arg->token->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->sibling, expected_arg = expected_arg->sibling
    );

    if (!provided_arg && (expected_arg && expected_arg->token->t_type != SCOPE_TOKEN)) {
        SEMANTIC_ERROR(
            " [line=%i] Not enough arguments for function=%s!", 
            nd->token->lnum, nd->token->value
        );

        _restore_code(nd, NULL);
        fprintf(stdout, "\n\n");
        return 0;
    }

    if (provided_arg && (!expected_arg || expected_arg->token->t_type == SCOPE_TOKEN)) {
        SEMANTIC_ERROR(
            " [line=%i] Too many arguments for function=%s!", 
            nd->token->lnum, nd->token->value
        );

        _restore_code(nd, NULL);
        fprintf(stdout, "\n\n");
        return 0;
    }

    return 1;
}

int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;

    ast_node_t* provided_arg = nd->child;
    ast_node_t* expected_arg = fi.args->child;
    for (
        ; provided_arg && expected_arg && expected_arg->token->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->sibling, expected_arg = expected_arg->sibling
    ) {
        _check_assign_types("Illegal argument", expected_arg->token, provided_arg->token);
        _restore_code(nd, provided_arg);
        fprintf(stdout, "\n\n");
    }

    return 1;
}

int ASTWLKR_unused_rtype(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;
    if (!fi.rtype) return 1;

    if (fi.rtype->token->t_type != I0_TYPE_TOKEN) {
        if (nd->parent && nd->parent->token) switch(nd->parent->token->t_type) {
            case SCOPE_TOKEN:
            case IF_TOKEN:
            case WHILE_TOKEN:
            case START_TOKEN:
            case FUNC_TOKEN: {
                SEMANTIC_WARNING(" [line=%i] Unused function=%s result!", nd->token->lnum, fi.name);
                return 0;
            }

            default: return 1;
        } 

        return 1;
    }

    return 1;
}

int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS) {
    ast_node_t* index = nd->child;
    if (!index || index->token->t_type != UNKNOWN_NUMERIC_TOKEN) return 1;

    array_info_t ai;
    if (!ARTB_get_info(nd->sinfo.v_id, &ai, &smt->a)) return 0;

    int idx = str_atoi(index->token->value);
    if (ai.size < str_atoi(index->token->value)) {
        SEMANTIC_ERROR(
            " [line=%i] Array=%s used with index=%i, that larger than array size!", 
            nd->token->lnum, nd->token->value, idx
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_duplicated_branches(AST_VISITOR_ARGS) {
    ast_node_t* lbranch = nd->child->sibling;
    if (!lbranch) return 1;
    ast_node_t* rbranch = lbranch->sibling;
    if (!rbranch) return 1;

    if (AST_hash_node(lbranch) == AST_hash_node(rbranch)) {
        SEMANTIC_WARNING(
            " Possible branch redundancy! The branch at [line=%i] is similar to the branch at [line=%i]!",
            lbranch->token->lnum, rbranch->token->lnum
        );

        return 0;
    }

    return 1;
}

static char* _format_name(int t) {
    switch (t) {
        case 0: return "camelCase";
        case 1: return "PascalCase";
        case 2: return "Kebab-Case";
        case 3: return "snake_case";
    }

    return "";
}

/*
Return -1 when encounter with unknown format
Return 0 if input string format is camelCase
Return 1 if input string format is PascalCase
Return 2 if input string format is kebab-case
Return 3 if input string format is snake_case
*/
static int _determine_string_style(const char* s) {
    int has_upper = 0;
    int has_hyphen = 0;
    int has_underscore = 0;
    if (!s || !*s) return -1;

    for (const char *p = s; *p; p++) {
        if (str_isupper(*p)) has_upper      = 1;
        if (*p == '-')       has_hyphen     = 1;
        if (*p == '_')       has_underscore = 1;
    }

    if (has_hyphen && !has_underscore) {
        for (const char *p = s; *p; p++) {
            if (*p != '-' && !str_islower((unsigned char)*p) && !str_isdigit((unsigned char)*p)) {
                return -1;
            }
        }

        return 2;
    }

    if ((!has_upper || has_underscore) && !has_hyphen) {
        for (const char *p = s; *p; p++) {
            if (*p != '_' && !str_islower((unsigned char)*p) && !str_isdigit((unsigned char)*p)) {
                return -1;
            }
        }

        return 3;
    }

    if (!has_hyphen && !has_underscore && str_islower((unsigned char)s[0])) {
        for (const char *p = s; *p; p++) {
            if (!str_isalnum((unsigned char)*p)) {
                return -1;
            }
        }

        return 0;
    }

    if (!has_hyphen && !has_underscore && str_isupper((unsigned char)s[0])) {
        for (const char *p = s; *p; p++) {
            if (!str_isalnum((unsigned char)*p)) {
                return -1;
            }
        }

        return 1;
    }

    return -1;
}

int ASTWLKR_valid_function_name(AST_VISITOR_ARGS) {
    ast_node_t* fname = nd->child;
    if (!fname) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(fname->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " [line=%i] Function=%s is not registered for some reason! Check logs!",
            fname->token->lnum, fname->token->value
        );

        return 0;
    }

    if (fi.name[0] == '_') {
        SEMANTIC_WARNING(
            " [line=%i] Function=%s has underscore at name start!",
            fname->token->lnum, fname->token->value
        );
    }

    if (!str_strcmp(fi.name, "chloe")) {
        SEMANTIC_INFO(" [line=%i] Used Chloe as a function name!", fname->token->lnum);
    }
    else if (!str_strcmp(fi.name, "max&chloe")) {
        SEMANTIC_INFO(" [line=%i] Used Max and Chloe as a function name!", fname->token->lnum);
    }
    else if (!str_strcmp(fi.name, "fang")) {
        SEMANTIC_INFO(" [line=%i] Used Fang as a function dragon-name!", fname->token->lnum);
    }
    
    int name_format = _determine_string_style(fi.name);
    if (name_format != 3) {
        SEMANTIC_WARNING(
            " [line=%i] Function name=%s isn't in sneaky_case! (%s)", 
            fname->token->lnum, fi.name, _format_name(name_format)
        );
    }

    return 1;
}
