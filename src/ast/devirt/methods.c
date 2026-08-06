#include <ast/devirt.h>

static int _find_and_cut_container_function(ast_node_t* node, string_t* container, queue_t* funcs) {
    if (!node) return 0;
    int found_func = 0;
    if (
        node->t && container &&
        (node->t->t_type == FUNC_PROT_TOKEN || node->t->t_type == FUNC_TOKEN)
    ) found_func = 1;
    
    _find_and_cut_container_function(node->siblings.n, container, funcs);
    if (found_func) {
        if (node->p) AST_remove_node(node->p, node);
        method_t* pack = (method_t*)mm_malloc(sizeof(method_t));
        if (pack) {
            pack->func   = node;
            pack->prefix = container;
            queue_push(funcs, pack);
        }

        return 1;
    }

    if (
        node->t && 
        node->t->t_type == CONTAINER_TOKEN
    ) container = node->c->t->body;
    _find_and_cut_container_function(node->c, container, funcs);
    return 1;
}

int AST_DVRT_move_container_functions(ast_node_t* root, queue_t* out) {
    return _find_and_cut_container_function(root, NULL, out);
}
