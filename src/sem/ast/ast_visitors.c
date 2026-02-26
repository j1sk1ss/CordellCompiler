#include <sem/ast/ast_visitors.h>

static const char* _fmt_tkn_op(token_type_t t) {
    switch (t) {
        case I0_TYPE_TOKEN:
        case F64_TYPE_TOKEN: case I64_TYPE_TOKEN: case U64_TYPE_TOKEN:
        case F32_TYPE_TOKEN: case I32_TYPE_TOKEN: case U32_TYPE_TOKEN:
        case I16_TYPE_TOKEN: case U16_TYPE_TOKEN:
        case I8_TYPE_TOKEN:  case U8_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN:    return "declaration";

        case BITORASSIGN_TOKEN:   return "|=";
        case MODULOASSIGN_TOKEN:  return "%%=";
        case BITANDASSIGN_TOKEN:  return "&=";
        case BITXORASSIGN_TOKEN:  return "^=";

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

static const char* _fmt_type_size(type_size_t s) {
    switch (s) {
        case TYPE_FULL_SIZE:    return "max";
        case TYPE_HALF_SIZE:    return "max/2";
        case TYPE_QUARTER_SIZE: return "max/4";
        default:                return "max/8";
    }
}

int ASTWLKR_ro_assign(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;

    if (larg->t->flags.ro) {
        SEMANTIC_ERROR(" %s Read-only variable='%s' assign!", format_location(&larg->t->finfo), larg->t->body->body);
        REBUILD_CODE_2TRG(nd, larg, rarg);
        return 0;
    }

    return 1;
}

/*
Get basic type token from the AST node.
If this is a cast node, extract a cast type.
Params:
    - `nd` - AST node.

Returns a token with a type.
*/
static inline token_t* _get_base_type_token(ast_node_t* nd, int* ptr) {
    switch (nd->t->t_type) {
        case REF_TYPE_TOKEN: if (ptr) *ptr = TYPE_FULL_SIZE;
        case NEGATIVE_TOKEN:
        case DREF_TYPE_TOKEN:
        case CONVERT_TOKEN:  return nd->c->t;
        default: break;
    }
    
    return nd->t;
}

int ASTWLKR_rtype_assign(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg || rarg->t->t_type != CALL_TOKEN) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(rarg->sinfo.v_id, &fi, &smt->f)) return 1;
    if (!fi.rtype) return 1;

    int ptr = 0;
    if (TKN_variable_bitness(fi.rtype->t, 1) != MAX(TKN_variable_bitness(_get_base_type_token(larg, &ptr), 1), (type_size_t)ptr)) {
        SEMANTIC_WARNING(
            " %s Function='%s' return type='%s' doesn't match to the %s type='%s'!", format_location(&larg->t->finfo), fi.name->body,
            RST_restore_type(fi.rtype->t), _fmt_tkn_op(nd->t->t_type), RST_restore_type(larg->t)
        );

        REBUILD_CODE_1TRG(nd, rarg);
        return 0;
    }

    return 1;
}

int ASTWLKR_not_init(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* larg = nd->c;
    if (!larg) return 1;

    if (
        nd->p &&      /* If we have a parent     */
        nd->p->p &&   /* The parent has a parent */
        (
            nd->p->p->t->t_type == FUNC_TOKEN ||  /* And this isn't a function              */
                                                  /* Note: We don't fire a warning          */
                                                  /*       if this is a argument from       */
                                                  /*       function's body                  */
                                                  /* Note 2: The function has a structure,  */
                                                  /*         that a arg has a parent,       */
                                                  /*         and this parent has a function */
                                                  /*         as a parent                    */
            nd->p->t->t_type == START_TOKEN ||
            nd->p->p->t->t_type == FUNC_PROT_TOKEN
        )
    ) return 1;

    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) {
        SEMANTIC_WARNING(" %s Variable='%s' without initialization!", format_location(&larg->t->finfo), larg->t->body->body);
        REBUILD_CODE_0TRG(nd);
        return 0;
    }

    return 1;
}

static type_size_t _get_token_bitness(token_t* tkn) {
    if (!TKN_isnumeric(tkn)) return TKN_variable_bitness(tkn, 1);
    else {
        long long val = tkn->body->to_llong(tkn->body);
        if (val <= UCHAR_MAX)      return TYPE_EIGHTH_SIZE;
        else if (val <= USHRT_MAX) return TYPE_QUARTER_SIZE;
        else if (val <= UINT_MAX)  return TYPE_HALF_SIZE;
        return TYPE_FULL_SIZE;
    }
}

static token_t* _get_token_from_ast(ast_node_t* n, int* ptr) {
    if (!n || !n->t) return NULL;
    if (TKN_isoperand(n->t)) {
        int lptr = 0, rptr = 0;
        token_t* l = _get_token_from_ast(n->c, &lptr);
        token_t* r = _get_token_from_ast(n->c->siblings.n, &rptr);

        if (!l && !r) return NULL;
        else if (!l) return r;
        else if (!r) return l;

        if (MAX(_get_token_bitness(l), (type_size_t)lptr) < MAX(_get_token_bitness(r), (type_size_t)rptr)) return r;
        return l;
    }

    return _get_base_type_token(n, ptr);
}

static int _check_assign_types(const char* msg, ast_node_t* l, ast_node_t* r) {
    if (!l || !r) return 0;

    int ltptr = 0, rtptr = 0;
    token_t* lt = _get_token_from_ast(l, &ltptr);
    token_t* rt = _get_token_from_ast(r, &rtptr);

    if (MAX(_get_token_bitness(lt), (type_size_t)ltptr) < MAX(_get_token_bitness(rt), (type_size_t)rtptr)) {
        if (TKN_isnumeric(rt)) {
            SEMANTIC_WARNING(
                " %s %s of '%s' with '%s' (Number's bitness is=%s, but '%s' can handle bitness=%s)!", format_location(&rt->finfo), msg,
                lt->body->body, rt->body->body, _fmt_type_size(_get_token_bitness(rt)), 
                RST_restore_type(lt), _fmt_type_size(_get_token_bitness(lt))
            );
        }
        else {
            SEMANTIC_WARNING(
                " %s %s of '%s' with '%s'! '%s' can't handle bitness=%s!", format_location(&rt->finfo), msg,
                lt->body->body, rt->body->body, RST_restore_type(lt), _fmt_type_size(MAX(_get_token_bitness(rt), (type_size_t)rtptr))
            );
        }

        return 0;
    }

    return 1;
}

int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;
    if (!_check_assign_types("Illegal declaration", larg, rarg)) {
        REBUILD_CODE_1TRG(nd, rarg);
        return 0;
    }

    return 1;
}

/*
Search for the 'return' or for the 'exit' statement in the provided AST node.
Params:
    - `nd` - Target AST node.
    - `found` - External flag. Will hold the `1` value if this function
                finds the 'return' or the 'exit' statement.
    - `type` - External flag. Will hold the next list of values:
                - 1 - Found the 'exit' statement.
                - 2 - Found the 'return' statement.

Returns 1 if succeeds.
*/
static int _search_term_node(ast_node_t* nd, int* found, int* type) {
    if (!nd) return 0;
    if (!nd->t) {
        _search_term_node(nd->c, found, type);
        return 1;
    }

    switch (nd->t->t_type) {
        case IF_TOKEN:
        case LOOP_TOKEN:
        case WHILE_TOKEN: {
            ast_node_t* cnd     = nd->c;
            ast_node_t* lbranch = cnd->siblings.n;
            ast_node_t* rbranch = lbranch->siblings.n;

            int nret = 0, lret = 0, rret = 0;
            _search_term_node(lbranch->c, &lret, type);
            if (rbranch) _search_term_node(rbranch->c, &rret, type);
            _search_term_node(nd->siblings.n, &nret, type);

            if (!rbranch)      rret = nret;
            if (rret && lret)  nret = 1;
            if (found && nret) *found = 1;
            return 1;
        }

        case SWITCH_TOKEN: {
            int nret = 0, caseret = 1;
            ast_node_t* cnd   = nd->c;
            ast_node_t* cases = cnd->siblings.n->c;
            for (; cases; cases = cases->siblings.n) {
                int curret = 0;
                _search_term_node(cases->c, &curret, type);
                caseret = caseret && curret;
            }

            _search_term_node(nd->siblings.n, &nret, type);
            if (nret || caseret) *found = 1;
            break;
        }

        case EXIT_TOKEN:   if (type) *type = 1; goto _set_found_flag;
        case RETURN_TOKEN: if (type) *type = 2;
        {
_set_found_flag: {}
            if (found) *found = 1;
            return 1;
        }

        default: {
            _search_term_node(nd->c, found, type);
            if (found && *found) break;
            _search_term_node(nd->siblings.n, found, type);
            break;
        }
    }

    return 1;
}

int ASTWLKR_no_return(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(nd->c->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' isn't registered for some reason! Check previous logs!",
            format_location(&nd->c->t->finfo), nd->c->t->body->body
        );

        return 0;
    }

    int has_ret = 0;
    _search_term_node(nd->c, &has_ret, NULL); 
    if (!has_ret && fi.rtype->t->t_type != I0_TYPE_TOKEN) {
        SEMANTIC_WARNING(
            " %s Function='%s' doesn't have the 'return' statement in all paths!", 
            format_location(&nd->t->finfo), nd->c->t->body->body
        );

        REBUILD_CODE_1TRG(nd, NULL);
        return 0;
    }

    return 1;
}

int ASTWLKR_no_exit(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    int has_ret = 0;
    _search_term_node(nd->c, &has_ret, NULL);
    if (!has_ret) {
        SEMANTIC_WARNING(" %s Start doesn't have the 'exit' statement in all paths!", format_location(&nd->t->finfo));
        REBUILD_CODE_1TRG(nd, NULL);
        return 0;
    }

    return 1;
}

int ASTWLKR_not_enough_args(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' isn't registered for some reason! Check previous logs!",
            format_location(&nd->c->t->finfo), nd->c->t->body->body
        );

        return 0;
    }

    ast_node_t* provided_arg = nd->c->c;
    ast_node_t* expected_arg = fi.args->c;
    for (
        ; provided_arg && expected_arg && expected_arg->t->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->siblings.n, expected_arg = expected_arg->siblings.n
    );

    if (!provided_arg && (expected_arg && expected_arg->t->t_type != SCOPE_TOKEN)) {
        SEMANTIC_ERROR(" %s Not enough arguments for the function='%s'!", format_location(&nd->t->finfo), nd->t->body->body);
        REBUILD_CODE_1TRG(nd, NULL);
        return 0;
    }

    if (provided_arg && (!expected_arg || expected_arg->t->t_type == SCOPE_TOKEN)) {
        SEMANTIC_ERROR(" %s Too many arguments for the function='%s'!", format_location(&nd->t->finfo), nd->t->body->body);
        REBUILD_CODE_1TRG(nd, NULL);
        return 0;
    }

    return 1;
}

int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' isn't registered for some reason! Check previous logs!",
            format_location(&nd->c->t->finfo), nd->c->t->body->body
        );

        return 0;
    }

    ast_node_t* provided_arg = nd->c->c;
    ast_node_t* expected_arg = fi.args->c;
    for (
        ; provided_arg && expected_arg && expected_arg->t->t_type != SCOPE_TOKEN; 
        provided_arg = provided_arg->siblings.n, expected_arg = expected_arg->siblings.n
    ) {
        if (!_check_assign_types("Illegal argument", expected_arg, provided_arg)) {
            REBUILD_CODE_1TRG(nd, provided_arg);
        }
    }

    return 1;
}

/*
Find if there is a final consumer for the 'src' node.
Consumer in this terms implies a some sort of the end destination for a value.
For instance:
```cpl
i32 a = b + c;
```

The 'a' variable consumes the product of a sum of the 'b' and the 'c' variable.
But if we consider the next example:
```cpl
i32 a;
b + c;
```

It still a correct expression, but it doesn't consumed by any final destination.
Similar situation with return values from function. For instance:
```cpl
function abc() -> i32;
i32 a = abc(); : Value was consumed here      :
abc();         : Value wasn't consumed at all :
```

Params:
    - `src` - The source value.
    - `found` - Output node that contans additional info about
                the consumer.
                Note: If it's value below or equals zero - Consumer
                      isn't found

Returns 1 if it has found the consumer. Otherwise will return 0.
*/
static int _find_consumer(ast_node_t* src, int* found) {
    if (!src) return 0;
    if (*found > 0)      return 1;
    else if (*found < 0) return 0;

    switch (src->t->t_type) {
        case SCOPE_TOKEN:
        case FUNC_TOKEN:
        case START_TOKEN: *found = -1; return 0;
        default: break;
    }

    /* Consumed by a variable
       For instance:
        - indexing */
    if (TKN_isvariable(src->t)) {
        *found = 1;
        return 1;
    }

    /* Consumed by a variable declaration
     */
    if (TKN_is_decl(src->t)) {
        *found = 2;
        return 1;
    }

    /* Consumed by a variable update such as:
        - +=
        - -=
        - /=
        etc. */
    if (TKN_update_operator(src->t)) {
        *found = 3;
        return 1;
    }

    switch (src->t->t_type) {
        case IF_TOKEN:
        case WHILE_TOKEN:
        case CALL_TOKEN: *found = 4; return 1;
        default: break;
    }

    _find_consumer(src->p, found);
    return 0;
}

int ASTWLKR_unused_rtype(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' isn't registered for some reason! Check previous logs!",
            format_location(&nd->c->t->finfo), nd->c->t->body->body
        );

        return 0;
    }

    if (!fi.rtype) return 1;
    if (fi.rtype->t->t_type != I0_TYPE_TOKEN) {
        int consumed = 0;
        _find_consumer(nd->p, &consumed);
        if (consumed <= 0) {
            SEMANTIC_WARNING(" %s Unused the function='%s's result!", format_location(&nd->t->finfo), fi.name->body);
            REBUILD_CODE_1TRG(nd->p, nd);
            return 0;
        }

        return 1;
    }

    return 1;
}

int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* body  = nd->c;
    ast_node_t* index = nd->c->siblings.n;
    if (
        !index || 
        index->t->t_type != UNKNOWN_NUMERIC_TOKEN
    ) return 1;

    array_info_t ai;
    if (!ARTB_get_info(body->sinfo.v_id, &ai, &smt->a)) return 1;

    long long idx = index->t->body->to_llong(index->t->body);
    if (idx < 0) {
        SEMANTIC_ERROR(
            " %s Array='%s' accessed with a negative index!", 
            format_location(&index->t->finfo), body->t->body->body
        );

        REBUILD_CODE_1TRG(nd, index);
        return 0;
    }
    
    if (ai.size < idx) {
        SEMANTIC_ERROR(
            " %s Array='%s' accessed with the index=%lli that is larger than the array size=%li!", 
            format_location(&index->t->finfo), body->t->body->body, idx, ai.size
        );

        REBUILD_CODE_1TRG(nd, index);
        return 0;
    }

    return 1;
}

int ASTWLKR_duplicated_branches(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* lbranch = nd->c->siblings.n;
    if (!lbranch) return 1;
    ast_node_t* rbranch = lbranch->siblings.n;
    if (!rbranch) return 1;
    
    if (AST_hash_node(lbranch) == AST_hash_node(rbranch)) {
        SEMANTIC_WARNING(
            " Possible branch redundancy! The branch at %s is similar to the branch at %s!",
            format_location(&lbranch->t->finfo), format_location(&rbranch->t->finfo)
        );

        REBUILD_CODE_2TRG(nd, lbranch, rbranch);
        return 0;
    }

    return 1;
}

/*
Format format name by the provided type.
Params:
    - `t` - Case type.

Returns a formatted case type. 
*/
static inline char* _format_name(int t) {
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
    AST_VISITOR_ARGS_USE;
    ast_node_t* fname = nd->c;
    if (!fname) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(fname->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' is not registered for some reason! Check previous logs!",
            format_location(&fname->t->finfo), fname->t->body->body
        );

        return 0;
    }

    if (
        fi.name->body[0] == '_' && 
        (fi.name->len(fi.name) > 0 && fi.name->body[1] == '_')
    ) {
        SEMANTIC_WARNING(
            " %s Function='%s' has an underscore symbol at the name's start!",
            format_location(&fname->t->finfo), fname->t->body->body
        );
    }

    if (fi.name->requals(fi.name, "chloe")) {
        SEMANTIC_INFO(" %s Used 'Chloe' as a function name!", format_location(&fname->t->finfo));
    }
    else if (fi.name->requals(fi.name, "max&chloe")) {
        SEMANTIC_INFO(" %s Used 'Max' and 'Chloe' as a function name!", format_location(&fname->t->finfo));
    }
    else if (fi.name->requals(fi.name, "fang")) {
        SEMANTIC_INFO(" %s Used 'Fang' as a function dragon-name!", format_location(&fname->t->finfo));
    }
    
    int name_format = _determine_string_style(fi.name->body);
    if (name_format != 3) {
        SEMANTIC_INFO(
            " %s Function name='%s' isn't in the sneaky_case! '%s'", 
            format_location(&fname->t->finfo), fi.name->body, _format_name(name_format)
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
            int ptr = 0;
            token_t* rval = _get_token_from_ast(nd->c, &ptr);
            if (!rval && rtype->t_type == I0_TYPE_TOKEN) return 1;
            if (rval && rtype->t_type == I0_TYPE_TOKEN) {
                SEMANTIC_WARNING(
                    " %s Function='%s' has the return value, but isn't supposed to!", 
                    format_location(&rval->finfo), fname
                );

                REBUILD_CODE_1TRG(nd, nd->c);
                return 0;
            }

            if (MAX(_get_token_bitness(rval), (type_size_t)ptr) > _get_token_bitness(rtype)) {
                SEMANTIC_WARNING(
                    " %s Function='%s' has the wrong return value!='%s'!", 
                    format_location(&rval->finfo), fname, RST_restore_type(rtype)
                );

                REBUILD_CODE_1TRG(nd, nd->c);
                return 0;
            }

            break;
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
    AST_VISITOR_ARGS_USE;
    ast_node_t* fname = nd->c;
    if (!fname) return 1;

    func_info_t fi;
    if (!FNTB_get_info_id(fname->sinfo.v_id, &fi, &smt->f)) return 0;
    if (!fi.rtype) {
        SEMANTIC_INFO(" %s Consider to add a return type to the function='%s'!", format_location(&nd->t->finfo), fname->t->body->body);
        return 1;
    }

    return _check_return_statement(fi.name->body, nd->c, fi.rtype->t);
}

int ASTWLKR_deadcode(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    if (nd->siblings.n) {
        SEMANTIC_WARNING(" %s 'Dead Code' after the termination statement!", format_location(&nd->t->finfo));
        REBUILD_CODE_1TRG(nd->p, nd->siblings.n);
        return 0;
    }

    return 1;
}

int ASTWLKR_implict_convertion(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* larg = nd->c;
    if (!larg) return 1;
    ast_node_t* rarg = larg->siblings.n;
    if (!rarg) return 1;
    if (!_check_assign_types("Implict convertion detected", larg, rarg)) {
        REBUILD_CODE_1TRG(nd, rarg);
        return 0;
    }

    return 1;
}

int ASTWLKR_inefficient_while(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    ast_node_t* cond = nd->c;
    if (!cond || cond->t->t_type != UNKNOWN_NUMERIC_TOKEN) return 0;
    if (cond->t->body->to_llong(cond->t->body)) {
        SEMANTIC_INFO(" %s Consider the usage of the 'loop' statement!", format_location(&nd->t->finfo));
        REBUILD_CODE_1TRG(nd, cond);
        return 0;
    }

    return 1;
}

int ASTWLKR_wrong_exit(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    long fid = 0;
    if (nd->t->t_type == START_TOKEN) fid = nd->sinfo.v_id;
    else fid = nd->c->sinfo.v_id;

    func_info_t fi;
    if (
        !FNTB_get_info_id(fid, &fi, &smt->f) || 
        !fi.flags.entry
    ) return 1;

    int f = 0, t = 0;
    _search_term_node(nd->c, &f, &t);
    if (f && t == 2) {
        SEMANTIC_INFO(
            " %s The function '%s' is an entry point! Consider the usage of the 'exit' statement over the 'return' statement!", 
            format_location(&nd->t->finfo), fi.name->body
        );

        return 0;
    }

    return 1;
}

int ASTWLKR_break_without_statement(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;
    if (
        !flags->in_loop  && 
        !flags->in_while &&
        !flags->in_switch
    ) {
        SEMANTIC_WARNING(" %s The 'break' statement without any statement that uses it!", format_location(&nd->t->finfo));
        REBUILD_CODE_1TRG(nd->p, nd);
        return 0;
    }

    return 1;
}

int ASTWLKR_noret_assign(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(nd->sinfo.v_id, &fi, &smt->f)) {
        SEMANTIC_ERROR(
            " %s Function='%s' isn't registered for some reason! Check previous logs!",
            format_location(&nd->c->t->finfo), nd->c->t->body->body
        );

        return 0;
    }

    if (!fi.rtype) return 1;
    if (fi.rtype->t->t_type == I0_TYPE_TOKEN) {
        int consumed = 0;
        _find_consumer(nd->p, &consumed);
        if (consumed > 0) {
            SEMANTIC_WARNING(
                " %s The function='%s' doesn't return anything, but result is used!", 
                format_location(&nd->t->finfo), fi.name->body
            );

            REBUILD_CODE_1TRG(nd->p, nd);
            return 0;
        }

        return 1;
    }

    return 1;
}

int ASTWLKR_unused_expression(AST_VISITOR_ARGS) {
    AST_VISITOR_ARGS_USE;

    int consumed = 0;
    _find_consumer(nd, &consumed);
    if (consumed <= 0) {
        SEMANTIC_WARNING(" %s The expression returns value that never assigns!", format_location(&nd->t->finfo));
        REBUILD_CODE_1TRG(nd->p, nd);
        return 0;
    }

    return 1;
}
