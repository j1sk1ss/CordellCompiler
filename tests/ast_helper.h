#ifndef ASTHELPER_H_
#define ASTHELPER_H_
#include <syntax.h>
static int print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("    ");
    if (node->token && node->token->t_type != SCOPE_TOKEN) {
        printf(
            "[%s] (t=%d, size=%i,%s%s%soff=%i, s_id=%i%s%s%s)\n", 
            node->token->value, node->token->t_type, node->info.size, 
            node->token->vinfo.ptr ? " ptr, " : " ",
            node->token->vinfo.ref ? "ref, " : "",
            node->token->vinfo.dref ? "dref, " : "",
            node->info.offset, node->info.s_id,
            node->token->vinfo.ro ? ", ro" : "",
            node->token->vinfo.ext ? ", ext" : "",
            node->token->vinfo.glob ? ", glob" : ""
        );
    }
    else if (node->token && node->token->t_type == SCOPE_TOKEN) {
        printf("{ scope, id=%i }\n", node->info.s_id);
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