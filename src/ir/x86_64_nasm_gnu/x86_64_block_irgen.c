#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

static int _navigation_handler(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node || !node->token) return 0;
    if (VRS_isdecl(node->token)) g->decl(node, g, ctx);
    if (
        VRS_isoperand(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) g->operand(node, g, ctx);
    switch (node->token->t_type) {
        case IF_TOKEN:           g->ifgen(node, g, ctx);     break;
        case ASM_TOKEN:          g->asmer(node, g, ctx);     break;
        case CALL_TOKEN:         g->funccall(node, g, ctx);  break;
        case FUNC_TOKEN:         g->function(node, g, ctx);  break;
        case EXIT_TOKEN:         g->exit(node, g, ctx);      break;
        case WHILE_TOKEN:        g->whilegen(node, g, ctx);  break;
        case START_TOKEN:        g->start(node, g, ctx);     break;
        case SWITCH_TOKEN:       g->switchgen(node, g, ctx); break;
        case RETURN_TOKEN:       g->funcret(node, g, ctx);   break;
        case ASSIGN_TOKEN:       g->assign(node, g, ctx);    break;
        case SYSCALL_TOKEN:      g->syscall(node, g, ctx);   break;
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN:
            if (node->token->flags.ref) g->ptrload(node, g, ctx);
            else g->load(node, g, ctx);
        break;
        default: break;
    }

    return 1;
}

int IR_generate_elem_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node) return 0;
    if (VRS_isblock(node->token)) {
        int res = g->elemegen(node->child, g, ctx);
        IR_deallocate_scope_heap(node, ctx);
        return res;
    }

    return _navigation_handler(node, g, ctx);
}

int IR_generate_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token) && (!t->token || t->token->t_type != START_TOKEN)) {
            g->blockgen(t->child, g, ctx);
            IR_deallocate_scope_heap(t, ctx);
        }

        _navigation_handler(t, g, ctx);
    }

    return 1;
}
