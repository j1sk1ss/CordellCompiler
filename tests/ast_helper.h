#ifndef ASTHELPER_H_
#define ASTHELPER_H_
#include <ast/syntax.h>
static int print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("   ");
    if (node->token && node->token->t_type != SCOPE_TOKEN) {
        printf(
            "[%s] (t=%d,%s%s%s s_id=%i%s%s%s%s)\n", 
            node->token->value, node->token->t_type, 
            node->token->flags.ptr ? " ptr, " : " ",
            node->token->flags.ref ? "ref, " : "",
            node->token->flags.dref ? "dref, " : "",
            node->sinfo.s_id,
            node->token->flags.ro ? ", ro" : "",
            node->token->flags.ext ? ", ext" : "",
            node->token->flags.glob ? ", glob" : "",
            node->token->flags.heap ? ", heap" : ""
        );
    }
    else if (node->token && node->token->t_type == SCOPE_TOKEN) {
        printf("{ scope, id=%i }\n", node->sinfo.s_id);
    }
    else {
        printf("[ block ]\n");
    }
    
    ast_node_t* child = node->child;
    while (child) {
        print_ast(child, depth + 1);
        child = child->sibling;
    }
    
    return 1;
}
#endif