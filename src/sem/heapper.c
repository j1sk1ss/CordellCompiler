#include <sem/semantic.h>

int SMT_check_heap_usage(ast_node_t* node) {
    if (!node) return 1;
    int result = 1;
    
    for (ast_node_t* t = node; t; t = t->sibling) {
        result = SMT_check_heap_usage(t->child) && result;
        if (!t->token) continue;
        if (t->token->t_type != ASSIGN_TOKEN) continue;
        if (t->child->token->flags.heap) {
            print_error("Heap owner variable [%s] lose pointer here! line=%i", t->child->token->value, t->token->lnum);
            result = 0;
            return result;
        }
    }
    
    return result;
}
