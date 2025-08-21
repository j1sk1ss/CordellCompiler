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
static int _check_exp_bitness(ast_node_t* r) {
    if (!r || !r->token) return 1;
    if (r->token->t_type == ASSIGN_TOKEN || VRS_isoperand(r->token) || VRS_isdecl(r->token)) {
        if (!r->child) return -1;
        char lbitness = _check_exp_bitness(r->child);
        if (!r->child->sibling) return -2;
        char rbitness = _check_exp_bitness(r->child->sibling);
        if (lbitness != rbitness) {
            print_warn(
                "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?",
                r->child->token->lnum, 
                lbitness, r->child->token->value, 
                rbitness, r->child->sibling->token->value
            );
        }

        return MAX(lbitness, rbitness);
    }

    return VRS_variable_bitness(r->token, 1);
}

int SMT_check(ast_node_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (!t->token) {
            SMT_check(t);
            continue;
        }

        switch (t->token->t_type) {
            case CALL_TOKEN: 
            case SYSCALL_TOKEN:
                if (t->sibling) result = SMT_check(t->sibling) && result; 
            break;

            case START_TOKEN: SMT_check(t->child); break;
            case FUNC_TOKEN: {
                ast_node_t* name = t->child;
                ast_node_t* args = name->sibling;
                ast_node_t* body = args->sibling;

                for (ast_node_t* param = args->child; param; param = param->sibling) {
                    if (!VRS_isdecl(param->token)) {
                        print_error("Unknown variable type at %i, (%s)", param->token->lnum, param->token->value);
                        result = 0;
                        break;
                    }
                }

                result = SMT_check(body) && result;
                if (t->sibling) result = SMT_check(t->sibling) && result;
                break;
            }

            case IF_TOKEN:
            case WHILE_TOKEN: 
                result = SMT_check(t) && result;
            break;

            case ARRAY_TYPE_TOKEN: {
                ast_node_t* arr_size = t->child;
                ast_node_t* el_size  = arr_size->sibling;
                ast_node_t* arr_name = el_size->sibling;
                ast_node_t* elems    = arr_name->sibling;

                unsigned long long el_msize = (unsigned long long)pow(2, VRS_variable_bitness(el_size->token, 1));

                if (elems) {
                    int count = 0;
                    for (ast_node_t* el = elems; el; el = el->sibling) {
                        if (str_atoi(el->token->value) >= el_msize) {
                            print_warn(
                                "Value %s at line [%i] too large for array [%s]!", 
                                el->token->value, el->token->lnum, arr_name->token->value
                            );
                        }

                        count++;
                    }

                    if (count > str_atoi((char*)arr_size->token->value)) {
                        print_error("Array [%s] larger than expected size %i > %s!", arr_name->token->value, count, arr_size->token->value);
                        result = 0;
                        break;
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
