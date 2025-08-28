#include <x86_64_gnu_nasm.h>

int x86_64_generate_elem(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node) return 0;
    if (VRS_isblock(node->token)) {
        return ctx->elemegen(node->child, output, ctx);
    }

    if (VRS_isdecl(node->token)) ctx->decl(node, output, ctx);
    if (
        VRS_isoperand(node->token) && 
        node->token->t_type != ASSIGN_TOKEN
    ) ctx->operand(node, output, ctx);

    switch (node->token->t_type) {
        case IF_TOKEN:      ctx->ifgen(node, output, ctx);     break;
        case SWITCH_TOKEN:  ctx->switchgen(node, output, ctx); break;
        case WHILE_TOKEN:   ctx->whilegen(node, output, ctx);  break;
        case CALL_TOKEN:    ctx->funccall(node, output, ctx);  break;
        case FUNC_TOKEN:    ctx->function(node, output, ctx);  break;
        case RETURN_TOKEN:  ctx->funcret(node, output, ctx);   break;
        case START_TOKEN:   ctx->start(node, output, ctx);     break;
        case EXIT_TOKEN:    ctx->exit(node, output, ctx);      break;
        case SYSCALL_TOKEN: ctx->syscall(node, output, ctx);   break;
        case ASSIGN_TOKEN:  ctx->assign(node, output, ctx);    break;
        case UNKNOWN_NUMERIC_TOKEN:
        case CHAR_VALUE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: ctx->load(node, output, ctx); break;
    }

    return 1;
}

int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            ctx->blockgen(t->child, output, ctx);
            continue;
        }

        ctx->elemegen(t, output, ctx);
    }

    return 1;
}
