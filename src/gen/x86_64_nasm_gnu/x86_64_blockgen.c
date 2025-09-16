#include <x86_64_gnu_nasm.h>

static int _navigation_handler(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node || !node->token) return 0;
    if (VRS_isdecl(node->token)) g->decl(node, output, ctx, g);
    if (
        VRS_isoperand(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) g->operand(node, output, ctx, g);

    switch (node->token->t_type) {
        case ASM_TOKEN:          g->asmer(node, output, ctx, g);     break;
        case IF_TOKEN:           g->ifgen(node, output, ctx, g);     break;
        case SWITCH_TOKEN:       g->switchgen(node, output, ctx, g); break;
        case WHILE_TOKEN:        g->whilegen(node, output, ctx, g);  break;
        case CALL_TOKEN:         g->funccall(node, output, ctx, g);  break;
        case FUNC_TOKEN:         g->function(node, output, ctx, g);  break;
        case RETURN_TOKEN:       g->funcret(node, output, ctx, g);   break;
        case START_TOKEN:        g->start(node, output, ctx, g);     break;
        case EXIT_TOKEN:         g->exit(node, output, ctx, g);      break;
        case SYSCALL_TOKEN:      g->syscall(node, output, ctx, g);   break;
        case ASSIGN_TOKEN:       g->assign(node, output, ctx, g);    break;
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
            if (node->token->vinfo.ref) g->ptrload(node, output, ctx, g);
            else g->load(node, output, ctx, g);
        break;
        default: break;
    }

    return 1;
}

int _deallocate_scope_heap(ast_node_t* t, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (scope_id_top(&ctx->heap) == t->info.s_id) {
        scope_elem_t hinfo;
        scope_pop_top(&ctx->heap, &hinfo);
        print_debug("Heap deallocation for scope %i, return to [rbp - %i]", t->info.s_id, hinfo.offset);
        iprintf(output, "mov rdi, [rbp - %d]\n", hinfo.offset);
        iprintf(output, "mov rax, 12\n");
        iprintf(output, "syscall\n");
    }

    return 1;
}

int x86_64_generate_elem(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node) return 0;
    if (VRS_isblock(node->token)) {
        int res = g->elemegen(node->child, output, ctx, g);
        _deallocate_scope_heap(node, output, ctx, g);
        return res;
    }

    return _navigation_handler(node, output, ctx, g);
}

int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token) && (!t->token || t->token->t_type != START_TOKEN)) {
            g->blockgen(t->child, output, ctx, g);
            _deallocate_scope_heap(t, output, ctx, g);
        }

        _navigation_handler(t, output, ctx, g);
    }

    return 1;
}
