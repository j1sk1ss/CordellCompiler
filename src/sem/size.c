#include <sem/semantic.h>

static inline unsigned long long _max_for_bits(int bitness) {
    if (bitness <= 0)  return 0;
    if (bitness >= 64) return ULLONG_MAX;
    return (1ULL << bitness) - 1;
}

int SMT_check_sizes(ast_node_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (ast_node_t* t = node; t; t = t->sibling) {
        SMT_check_sizes(t->child);
        if (!t->token) continue;

        if (VRS_isdecl(t->token) && t->child && t->child->sibling) {
            ast_node_t* value = t->child->sibling;
            if (VRS_isnumeric(value->token)) {
                unsigned long long el_msize = _max_for_bits(VRS_variable_bitness(t->token, 1));
                el_msize = VRS_issign(t->child->token) ? el_msize / 2 : el_msize;

                long val = value->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(value->token->value) : value->token->value[0];
                if (!VRS_issign(t->child->token) && val < 0) {
                    print_warn(
                        "Value %i at line=%i lower then 0 for unsigned type %s, %i < 0!", 
                        val, value->token->lnum, t->token->value, val
                    );
                }
                
                if (ABS(val) >= el_msize) {
                    print_warn(
                        "Value %i at line=%i too large for type %s (%i >= %i)!", 
                        val, value->token->lnum, t->token->value, val, el_msize
                    );
                }
            }
        }

        switch (t->token->t_type) {
            case ARRAY_TYPE_TOKEN: {
                ast_node_t* arr_name = t->child;
                ast_node_t* arr_size = arr_name->sibling;
                ast_node_t* el_size  = arr_size->sibling;

                unsigned long long el_msize = _max_for_bits(VRS_variable_bitness(el_size->token, 1));
                el_msize = VRS_issign(el_size->token) ? el_msize / 2 : el_msize;

                ast_node_t* elems = el_size->sibling;
                if (elems) {
                    int count = 0;
                    for (ast_node_t* el = elems; el; el = el->sibling) {
                        if (el->token->t_type != UNKNOWN_NUMERIC_TOKEN) continue;
                        long val = el->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(el->token->value) : el->token->value[0];
                        if (!VRS_issign(el_size->token) && val < 0) {
                            print_warn(
                                "Value %i at line=%i lower then 0 for unsigned type %s, %i < 0!", 
                                el->token->value, el->token->lnum, el_size->token->value, val
                            );
                        }
                        
                        if (ABS(val) >= el_msize) {
                            print_warn(
                                "Value %s at line=%i too large for array %s (%i >= %i)!", 
                                el->token->value, el->token->lnum, el_size->token->value, val, el_msize
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

                break;
            }

            default: break;
        }
    }
    
    return result;
}
