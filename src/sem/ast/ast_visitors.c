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
    if (nd == underscore) fprintf(stdout, "\e[4m");
    if (TKN_isdecl(nd->t)) {
        fprintf(stdout, "%s %s", _fmt_tkn_type(nd->t), nd->c->t->body->body);
        if (nd->c->siblings.n) {
            fprintf(stdout, " = ");
            _restore_code(nd->c->siblings.n, underscore);
        }

        fprintf(stdout, ";");
    }
    else if (TKN_isoperand(nd->t)) {
        if (nd->c) _restore_code(nd->c, underscore);
        fprintf(stdout, " %s ", nd->t->body->body);
        if (nd->c->siblings.n) _restore_code(nd->c->siblings.n, underscore);
        fprintf(stdout, ";");
    }
    
    switch (nd->t->t_type) {
        case FUNC_TOKEN: {
            fprintf(stdout, "function %s(", nd->c->t->body->body);
            ast_node_t* p = nd->siblings.n->c;
            for (; p->t && p->t->t_type != SCOPE_TOKEN; p = p->siblings.n) {
                _restore_code(p, underscore);
                if (p->siblings.n->t && p->siblings.n->t->t_type != SCOPE_TOKEN) fprintf(stdout, ", ");
            }

            fprintf(stdout, ") ");
            if (nd->c->c) fprintf(stdout, "=> %s ", _fmt_tkn_type(nd->c->c->t));
            fprintf(stdout, "{ ... ");
            break;
        }
        case CALL_TOKEN: {
            fprintf(stdout, "%s(", nd->t->body->body);
            ast_node_t* p = nd->c;
            for (; p; p = p->siblings.n) {
                _restore_code(p, underscore);
                if (p->siblings.n) fprintf(stdout, ", ");
            }

            fprintf(stdout, ");");
            break;
        }
        case WHILE_TOKEN: fprintf(stdout, "while "); goto _default_;
        case IF_TOKEN:    fprintf(stdout, "if ");    goto _default_;
        case RETURN_TOKEN: {
            fprintf(stdout, "return ");
_default_: {}
            if (nd->c) _restore_code(nd->c, underscore);
            fprintf(stdout, ";");
            break;
        }
        default: break;
    }

    if (
        TKN_isnumeric(nd->t) || 
        TKN_isvariable(nd->t)
    ) fprintf(stdout, "%s", nd->t->body->body);

    if (nd == underscore) fprintf(stdout, "\e[0m");
    return 1;
}

#define REBUILD_CODE(nd, trg)           \
        _restore_code(nd, trg);         \
        if (trg) fprintf(stdout, "\n"); \

int ASTWLKR_ro_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;

    if (larg->t->flags.ro) {
        SEMANTIC_ERROR(
            " [line=%i] Read-only variable='%s' assign!", 
            larg->t->lnum, larg->t->body->body
        );

        REBUILD_CODE(nd, larg);
        return 0;
    }

    return 1;
}

int ASTWLKR_rtype_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg || rarg->t->t_type != CALL_TOKEN) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(rarg->sinfo.v_id, &fi, &smt->f)) return 1;
    if (!fi.rtype) return 1;

    if (TKN_variable_bitness(fi.rtype->t, 1) != TKN_variable_bitness(larg->t, 1)) {
        SEMANTIC_WARNING(
            " [line=%i] Function='%s' return type='%s' not match to the %s type='%s'!", larg->t->lnum, fi.name->body,
            _fmt_tkn_type(fi.rtype->t), _fmt_tkn_op(nd->t->t_type), _fmt_tkn_type(larg->t)
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_not_init(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) {
        SEMANTIC_WARNING(
            " [line=%i] Variable='%s' without initialization!", 
            larg->t->lnum, larg->t->body->body
        );

        REBUILD_CODE(nd, larg);
        return 0;
    }

    return 1;
}

static int _get_token_bitness(token_t* tkn) {
    if (!TKN_isnumeric(tkn)) return TKN_variable_bitness(tkn, 1);
    else {
        int num_bitness = 64;
        long long val = tkn->body->to_llong(tkn->body);
        if (val <= UCHAR_MAX) num_bitness = 8;
        else if (val <= USHRT_MAX) num_bitness = 16;
        else if (val <= UINT_MAX)  num_bitness = 32;
        return num_bitness;
    }
}

static token_t* _get_token_from_ast(ast_node_t* n) {
    if (!n || !n->t) return NULL;
    if (TKN_isoperand(n->t)) {
        token_t* l = _get_token_from_ast(n->c);
        token_t* r = _get_token_from_ast(n->c->siblings.n);

        if (!l && !r) return NULL;
        else if (!l) return r;
        else if (!r) return l;

        if (_get_token_bitness(l) < _get_token_bitness(r)) return r;
        return l;
    }

    return n->t;
}

static int _check_assign_types(const char* msg, ast_node_t* l, ast_node_t* r) {
    if (!l || !r) return 0;

    token_t* lt = _get_token_from_ast(l);
    token_t* rt = _get_token_from_ast(r);

    if (_get_token_bitness(lt) < _get_token_bitness(rt)) {
        if (TKN_isnumeric(rt)) {
            SEMANTIC_WARNING(
                " [line=%i] %s of '%s' with '%s' (Number bitness is=%i, but '%s' can handle bitness=%i)!", rt->lnum, msg,
                lt->body->body, rt->body->body, _get_token_bitness(rt), _fmt_tkn_type(lt), _get_token_bitness(lt)
            );
        }
        else {
            SEMANTIC_WARNING(
                " [line=%i] %s of '%s' with '%s'! '%s' can't handle '%s'!", rt->lnum, msg,
                lt->body->body, rt->body->body, _fmt_tkn_type(lt), _fmt_tkn_type(rt)
            );
        }

        return 0;
    }

    return 1;
}

int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;
    if (!_check_assign_types("Illegal declaration", larg, rarg)) {
        REBUILD_CODE(nd, rarg);
        return 0;
    }

    return 1;
}

static int _search_rexit_ast(ast_node_t* nd, int* found) {
    if (!nd) return 0;
    if (!nd->t) {
        _search_rexit_ast(nd->c, found);
        return 0;
    }

    switch (nd->t->t_type) {
        case IF_TOKEN:
        case WHILE_TOKEN: {
            ast_node_t* cnd     = nd->c;
            ast_node_t* lbranch = cnd->siblings.n;
            ast_node_t* rbranch = lbranch->siblings.n;

            int nret = 0, lret = 0, rret = 0;
            _search_rexit_ast(lbranch, &lret);
            _search_rexit_ast(rbranch, &rret);
            _search_rexit_ast(nd->siblings.n, &nret);
            if (rret && lret) nret = 1;
            if (!rbranch) rret = 1;

            if (nret || (lret && rret)) *found = 1;
            return 1;
        }

        case SWITCH_TOKEN: {
            int nret = 0, caseret = 0;
            ast_node_t* cnd   = nd->c;
            ast_node_t* cases = cnd->siblings.n->c;
            for (; cases; cases = cases->siblings.n) {
                int curret = 0;
                _search_rexit_ast(cases->c, &curret);
                caseret = caseret && curret;
            }

            _search_rexit_ast(nd->siblings.n, &nret);
            if (nret || caseret) *found = 1;
            break;
        }

        case EXIT_TOKEN:
        case RETURN_TOKEN: {
            if (found) *found = 1;
            return 1;
        }

        default: {
            _search_rexit_ast(nd->c, found);
            _search_rexit_ast(nd->siblings.n, found);
            break;
        }
    }

    return 1;
}

int ASTWLKR_no_return(AST_VISITOR_ARGS) {
    int has_ret = 0;
    _search_rexit_ast(nd->c, &has_ret);
    if (!has_ret) {
        SEMANTIC_WARNING(
            " [line=%i] Function='%s' doesn't have the return statement in all paths!", 
            nd->t->lnum, nd->c->t->body->body
        );

        REBUILD_CODE(nd, NULL);
    }

    return 1;
}

int ASTWLKR_no_exit(AST_VISITOR_ARGS) {
    int has_ret = 0;
    _search_rexit_ast(nd->c, &has_ret);
    if (!has_ret) {
        SEMANTIC_WARNING(" [line=%i] Start doesn't have the exit statement in all paths!", nd->t->lnum);
        REBUILD_CODE(nd, NULL);
    }

    return 1;
}

int ASTWLKR_not_enough_args(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;

    ast_node_t* provided_arg = nd->c;
    ast_node_t* expected_arg = fi.args->c;
    for (
        ; provided_arg && expected_arg && expected_arg->t->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->siblings.n, expected_arg = expected_arg->siblings.n
    );

    if (!provided_arg && (expected_arg && expected_arg->t->t_type != SCOPE_TOKEN)) {
        SEMANTIC_ERROR(
            " [line=%i] Not enough arguments for function='%s'!", 
            nd->t->lnum, nd->t->body->body
        );

        REBUILD_CODE(nd, NULL);
        return 0;
    }

    if (provided_arg && (!expected_arg || expected_arg->t->t_type == SCOPE_TOKEN)) {
        SEMANTIC_ERROR(
            " [line=%i] Too many arguments for function='%s'!", 
            nd->t->lnum, nd->t->body->body
        );

        REBUILD_CODE(nd, NULL);
        return 0;
    }

    return 1;
}

int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;

    ast_node_t* provided_arg = nd->c;
    ast_node_t* expected_arg = fi.args->c;
    for (
        ; provided_arg && expected_arg && expected_arg->t->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->siblings.n, expected_arg = expected_arg->siblings.n
    ) {
        _check_assign_types("Illegal argument", expected_arg, provided_arg);
        REBUILD_CODE(nd, provided_arg);
    }

    return 1;
}

int ASTWLKR_unused_rtype(AST_VISITOR_ARGS) {
    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) return 0;
    if (!fi.rtype) return 1;

    if (fi.rtype->t->t_type != I0_TYPE_TOKEN) {
        if (nd->p && nd->p->t) switch(nd->p->t->t_type) {
            case SCOPE_TOKEN:
            case IF_TOKEN:
            case WHILE_TOKEN:
            case START_TOKEN:
            case FUNC_TOKEN: {
                SEMANTIC_WARNING(" [line=%i] Unused function='%s' result!", nd->t->lnum, fi.name->body);
                return 0;
            }

            default: return 1;
        } 

        return 1;
    }

    return 1;
}

int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS) {
    ast_node_t* index = nd->c;
    if (!index || index->t->t_type != UNKNOWN_NUMERIC_TOKEN) return 1;

    array_info_t ai;
    if (!ARTB_get_info(nd->sinfo.v_id, &ai, &smt->a)) return 0;

    long long idx = index->t->body->to_llong(index->t->body);
    if (ai.size < idx) {
        SEMANTIC_ERROR(
            " [line=%i] Array='%s' accessed with index=%lli, that larger than the array size!", 
            nd->t->lnum, nd->t->body->body, idx
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_duplicated_branches(AST_VISITOR_ARGS) {
    ast_node_t* lbranch = nd->c->siblings.n;
    if (!lbranch) return 1;
    ast_node_t* rbranch = lbranch->siblings.n;
    if (!rbranch) return 1;

    if (AST_hash_node(lbranch) == AST_hash_node(rbranch)) {
        SEMANTIC_WARNING(
            " Possible branch redundancy! The branch at [line=%i] is similar to the branch at [line=%i]!",
            lbranch->t->lnum, rbranch->t->lnum
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
Check the string's case style.
Params: 
    - `s` - String.
    
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
    ast_node_t* fname = nd->c;
    if (!fname) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(fname->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " [line=%i] Function='%s' is not registered for some reason! Check logs!",
            fname->t->lnum, fname->t->body->body
        );

        return 0;
    }

    if (fi.name->body[0] == '_') {
        SEMANTIC_WARNING(
            " [line=%i] Function='%s' has underscore at name start!",
            fname->t->lnum, fname->t->body->body
        );
    }

    if (fi.name->requals(fi.name, "chloe")) {
        SEMANTIC_INFO(" [line=%i] Used Chloe as a function name!", fname->t->lnum);
    }
    else if (fi.name->requals(fi.name, "max&chloe")) {
        SEMANTIC_INFO(" [line=%i] Used Max and Chloe as a function name!", fname->t->lnum);
    }
    else if (fi.name->requals(fi.name, "fang")) {
        SEMANTIC_INFO(" [line=%i] Used Fang as a function dragon-name!", fname->t->lnum);
    }
    
    int name_format = _determine_string_style(fi.name->body);
    if (name_format != 3) {
        SEMANTIC_WARNING(
            " [line=%i] Function name='%s' isn't in sneaky_case! (%s)", 
            fname->t->lnum, fi.name->body, _format_name(name_format)
        );
    }

    return 1;
}

/*
Check if the function's return type matches to the actual return value's type.
Params:
    - `fname` - Target function name.
    - `nd` - Base function node.
    - `rtype` - Function return type.

Returns 1 if return types are equals.
*/
static int _check_return_statement(const char* fname, ast_node_t* nd, token_t* rtype) {
    if (!nd) return 0;
    if (!nd->t) {
        _check_return_statement(fname, nd->c, rtype);
        return 0;
    }

    switch (nd->t->t_type) {
        case EXIT_TOKEN:
        case RETURN_TOKEN: {
            token_t* rval = _get_token_from_ast(nd->c);
            if (!rval && rtype->t_type == I0_TYPE_TOKEN) return 1;
            if (rval && rtype->t_type == I0_TYPE_TOKEN) {
                SEMANTIC_WARNING(" [line=%i] Function='%s' has the return value, but isn't suppose to!", rval->lnum, fname);
                REBUILD_CODE(nd, nd->c);
                return 0;
            }

            if (_get_token_bitness(rval) > _get_token_bitness(rtype)) {
                SEMANTIC_WARNING(" [line=%i] Function='%s' has the wrong return value!='%s'!", rval->lnum, fname, _fmt_tkn_type(rtype));
                REBUILD_CODE(nd, nd->c);
                return 0;
            }
        }

        default: {
            _check_return_statement(fname, nd->c, rtype);
            _check_return_statement(fname, nd->siblings.n, rtype);
            break;
        }
    }

    return 1;
}

int ASTWLKR_wrong_rtype(AST_VISITOR_ARGS) {
    ast_node_t* fname = nd->c;
    if (!fname) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(fname->sinfo.v_id, &fi, &smt->f)) return 0;
    if (!fi.rtype) {
        SEMANTIC_INFO(" [line=%i] Consider to add a return type for the function=%s!", nd->t->lnum, fname->t->body->body);
        return 1;
    }

    return _check_return_statement(fi.name->body, nd->c, fi.rtype->t);
}

int ASTWLKR_deadcode(AST_VISITOR_ARGS) {
    if (nd->siblings.n) {
        SEMANTIC_WARNING(" [line=%i] Possible dead code after the term statement!", nd->t->lnum);
        return 0;
    }

    return 1;
}

int ASTWLKR_implict_convertion(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;
    if (!_check_assign_types("Implict convertion detected", larg, rarg)) {
        REBUILD_CODE(nd, rarg);
        return 0;
    }

    return 1;
}

int ASTWLKR_inefficient_while(AST_VISITOR_ARGS) {
    ast_node_t* cond = nd->c;
    if (!cond || cond->t->t_type != UNKNOWN_NUMERIC_TOKEN) return 0;
    if (cond->t->body->to_llong(cond->t->body)) {
        SEMANTIC_INFO(" [line=%i] Consider a usage of the 'loop' statement!", nd->t->lnum);
        REBUILD_CODE(nd, cond);
        return 0;
    }

    return 1;
}
