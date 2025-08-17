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
    if (VRS_isoperand(r->token)) {
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

/*
Example: type name = value;
Node arch:
type
|_ name
|_ value | exp

Codes:
-1 - No name and value
-2 - No value
*/
static int _check_decl(tree_t* r) {
    tree_t* type = r;
    tree_t* name = type->first_child;
    if (!name) return -1;
    tree_t* value = name->next_sibling;
    if (!value) return -2;

    char tbitness = VRS_variable_bitness(name, 1);
    char vbitness = _check_exp_bitness(value);

    if (tbitness != vbitness) {
        if (tbitness > vbitness) {
            print_warn(
                "Variable %s at line %i assign to wrong size value (%i!=%i). Did you expect this?",
                name->token->value, name->token->line_number tbitness, vbitness
            );
        }
        else {
            print_warn(
                "Variable %s at line %i assign with info loss (%i!=%i)!",
                name->token->value, name->token->line_number tbitness, vbitness
            );
        }
    }

    return 1;
}

int check_semantic(tree_t* node) {
    if (!node) return 1;
    int result = 1;
    if (node->token) {
        switch (node->token->t_type) {
            case SYSCALL_TOKEN:
            case CALL_TOKEN: if (node->next_sibling) result = check_semantic(node->next_sibling) && result; break;

            case START_TOKEN: check_semantic(node->first_child->first_child); break;
            case FUNC_TOKEN: {
                tree_t* name = node->first_child;
                tree_t* args = name->next_sibling;
                tree_t* body = args->next_sibling;

                for (tree_t* param = args->first_child; param; param = param->next_sibling) {
                    if (!VRS_isdecl(param->token->t_type)) {
                        print_error("Unknown variable type at %i, (%s)", param->token->line_number, param->token->value);
                        result = 0;
                        break;
                    }
                }

                result = check_semantic(body->first_child) && result;
                if (node->next_sibling) result = check_semantic(node->next_sibling) && result;
                break;
            }

            case IF_TOKEN:
            case WHILE_TOKEN: {
                tree_t* cond = node->first_child;
                tree_t* body = cond->next_sibling->first_child;
                result = check_semantic(cond) && result;
                result = check_semantic(body) && result;
                if (node->next_sibling) result = check_semantic(node->next_sibling) && result;
                break;
            }

            case ARRAY_TYPE_TOKEN: {
                tree_t* arr_size = node->first_child;
                tree_t* el_size  = arr_size->next_sibling;
                tree_t* arr_name = el_size->next_sibling;
                tree_t* elems    = arr_name->next_sibling;

                if (elems) {
                    int count = 0;
                    for (tree_t* el = elems; el; el = el->next_sibling) count++;
                    if (count != str_atoi((char*)arr_size->token->value)) {
                        print_error("Static array [%s] not full initialized at %i!", arr_name->token->value, arr_name->token->line_number);
                        result = 0;
                        break;
                    }
                }

                if (node->next_sibling) result = check_semantic(node->next_sibling) && result;
                break;
            }

            case LONG_TYPE_TOKEN:
            case INT_TYPE_TOKEN:
            case STR_TYPE_TOKEN:
            case SHORT_TYPE_TOKEN:
            case CHAR_TYPE_TOKEN:
            case BITMOVE_LEFT_TOKEN:
            case BITMOVE_RIGHT_TOKEN:
            case BITOR_TOKEN:
            case BITAND_TOKEN:
            case ASIGN_TOKEN:
            case MULTIPLY_TOKEN:
            case DIVIDE_TOKEN:
            case MINUS_TOKEN:
            case PLUS_TOKEN: {
                tree_t* left = node->first_child;
                if (!left) {
                    print_error("Line %i, incorrect operand struct! Expected a <op> b", node->token->line_number);
                    result = 0;
                    break;
                }

                tree_t* right = left->next_sibling;
                if (!right) {
                    print_error("Line %i, incorrect operand struct! Expected a <op> b, but got a <op>", node->token->line_number);
                    result = 0;
                    break;
                }

                if (VRS_isoperand(right->token->t_type)) {
                    result = check_semantic(right) && result;
                    right = right->first_child;
                }

                if (result) {
                    int left_size = VRS_variable_bitness(left->token, 1);
                    int right_size = VRS_variable_bitness(right->token, 1);
                    if (left_size != right_size) {
                        print_warn(
                            "Danger shadow type cast at line %i. Different size [%i] (%s) and [%i] (%s). Did you expect this?", 
                            node->token->line_number, left_size, left->token->value, right_size, right->token->value
                        );
                    }
                }
                
                if (node->next_sibling) result = check_semantic(node->next_sibling) && result;
                break;
            }

            default: break;
        }
    }
    
    return result;
}
