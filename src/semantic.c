#include <semantic.h>

/*
Example: val op val;
Node arch:
op
|_ val | exp
|_ val | exp

Codes:
-1, -2 - Operand without variables
*/
static int _check_exp_bitness(tree_t* r) {
    if (!r || !r->token) return 1;
    if (r->token->t_type == ASSIGN_TOKEN || VRS_isoperand(r->token) || VRS_isdecl(r->token)) {
        if (!r->first_child) return -1;
        char lbitness = _check_exp_bitness(r->first_child);
        if (!r->first_child->next_sibling) return -2;
        char rbitness = _check_exp_bitness(r->first_child->next_sibling);
        if (lbitness != rbitness) {
            print_warn(
                "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?",
                r->first_child->token->line_number, 
                lbitness, r->first_child->token->value, 
                rbitness, r->first_child->next_sibling->token->value
            );
        }

        return MAX(lbitness, rbitness);
    }

    return VRS_variable_bitness(r->token, 1);
}

int SMT_check(tree_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (tree_t* t = node->first_child; t; t = t->next_sibling) {
        if (!t->token) {
            SMT_check(t);
            continue;
        }

        switch (t->token->t_type) {
            case CALL_TOKEN: 
            case SYSCALL_TOKEN:
                if (t->next_sibling) result = SMT_check(t->next_sibling) && result; 
            break;

            case START_TOKEN: SMT_check(t->first_child); break;
            case FUNC_TOKEN: {
                tree_t* name = t->first_child;
                tree_t* args = name->next_sibling;
                tree_t* body = args->next_sibling;

                for (tree_t* param = args->first_child; param; param = param->next_sibling) {
                    if (!VRS_isdecl(param->token)) {
                        print_error("Unknown variable type at %i, (%s)", param->token->line_number, param->token->value);
                        result = 0;
                        break;
                    }
                }

                result = SMT_check(body) && result;
                if (t->next_sibling) result = SMT_check(t->next_sibling) && result;
                break;
            }

            case IF_TOKEN:
            case WHILE_TOKEN: 
                result = SMT_check(t) && result;
            break;

            case ARRAY_TYPE_TOKEN: {
                tree_t* arr_size = t->first_child;
                tree_t* el_size  = arr_size->next_sibling;
                tree_t* arr_name = el_size->next_sibling;
                tree_t* elems    = arr_name->next_sibling;

                if (elems) {
                    int count = 0;
                    for (tree_t* el = elems; el; el = el->next_sibling) count++;
                    if (count > str_atoi((char*)arr_size->token->value)) {
                        print_error("Array [%s] larger than expected size %i > %s!", arr_name->token->value, count, arr_size->token->value);
                        result = 0;
                        break;
                    }
                }

                if (t->next_sibling) result = SMT_check(t->next_sibling) && result;
                break;
            }

            default: _check_exp_bitness(t); break;
        }
    }
    
    return result;
}
