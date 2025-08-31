#include <semantic.h>

static int _check_exp_bitness(ast_node_t* r) {
    if (!r || !r->token) return 1;
    if (VRS_isoperand(r->token) || VRS_isdecl(r->token)) {
        if (!r->child) return -1;
        char lbitness = _check_exp_bitness(r->child);
        if (!r->child->sibling) return -2;
        char rbitness = _check_exp_bitness(r->child->sibling);
        if (lbitness != rbitness) {
            print_warn(
                "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?",
                r->child->token->lnum, lbitness, r->child->token->value, rbitness, r->child->sibling->token->value
            );
        }

        return MAX(lbitness, rbitness);
    }

    return VRS_variable_bitness(r->token, 1);
}

static inline unsigned long long _max_for_bits(int bitness) {
    if (bitness <= 0)  return 0;
    if (bitness >= 64) return ULLONG_MAX;
    return (1ULL << bitness) - 1;
}

int SMT_check(ast_node_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        SMT_check(t);
        if (!t->token) continue;

        if (VRS_isdecl(t->token) && t->child && t->child->sibling) {
            ast_node_t* value = t->child->sibling;
            if (value->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                unsigned long long el_msize = _max_for_bits(VRS_variable_bitness(t->token, 1));
                if (str_atoi(value->token->value) >= el_msize) {
                    print_warn(
                        "Value %s at line [%i] too large for variable [%s]!", 
                        value->token->value, value->token->lnum, t->child->token->value
                    );
                }
            }
        }

        switch (t->token->t_type) {
            case ASSIGN_TOKEN:
                if (t->child->token->vinfo.ro) {
                    print_error("Read only value assigned here=%i", t->token->lnum);
                    result = 0;
                    return 1;
                }
            break;
            
            case FUNC_TOKEN: {
                ast_node_t* name = t->child;
                if (!name) {
                    print_error("Function name at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                ast_node_t* body = name->sibling;
                if (!body) {
                    print_error("Function body at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                ast_node_t* args = body->child;
                if (!args) {
                    print_error("Function args at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                for (ast_node_t* arg = args; arg && arg->token->t_type != SCOPE_TOKEN; arg = arg->sibling) {
                    if (!VRS_isdecl(arg->token)) {
                        print_error("Unknown variable type line=%i, (%s)", arg->token->lnum, arg->token->value);
                        result = 0;
                        return 1;
                    }

                    if (arg->child->sibling) {
                        print_warn(
                            "Current version of CPL don't support default values for [%s] var at line=%i", 
                            arg->child->token->value, arg->token->lnum
                        );
                    }
                }

                result = SMT_check(body) && result;
                if (t->sibling) result = SMT_check(t->sibling) && result;
                break;
            }

            case ARRAY_TYPE_TOKEN: {
                ast_node_t* arr_name = t->child;
                if (!arr_name) {
                    print_error("Array name at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                ast_node_t* arr_size = arr_name->sibling;
                if (!arr_size) {
                    print_error("Array size at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                if (arr_size->token->t_type != UNKNOWN_NUMERIC_TOKEN) {
                    print_error("Stack array [%s] declaration with unknown value [%s]!", arr_name->token->value, arr_size->token->value);
                    result = 0;
                    return 1;
                }

                ast_node_t* el_size = arr_size->sibling;
                if (!el_size) {
                    print_error("Array type at line=%i not found!", t->token->lnum);
                    result = 0;
                    return 1;
                }

                ast_node_t* elems   = el_size->sibling;
                unsigned long long el_msize = _max_for_bits(VRS_variable_bitness(el_size->token, 1));
                if (elems) {
                    int count = 0;
                    for (ast_node_t* el = elems; el; el = el->sibling) {
                        if (el->token->t_type != UNKNOWN_NUMERIC_TOKEN) continue;
                        if (str_atoi(el->token->value) >= el_msize) {
                            print_warn(
                                "Value %s at line [%i] too large for array [%s]!", 
                                el->token->value, el->token->lnum, arr_name->token->value
                            );
                        }

                        count++;
                    }

                    if (count > str_atoi(arr_size->token->value)) {
                        print_warn(
                            "Array [%s] larger than expected size %i > %s!", 
                            arr_name->token->value, count, arr_size->token->value
                        );
                    }
                }

                if (t->sibling) result = SMT_check(t->sibling) && result;
                break;
            }

            default: _check_exp_bitness(t); break;
        }
    }
    
    return result;
}
