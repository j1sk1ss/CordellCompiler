#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <std/str.h>
#include <prep/token_types.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <ast/ast.h>
#include <ast/synctx.h>
#include <symtab/symtab.h>

static int var_lookup(ast_node_t* node, syntax_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    var_lookup(node->sibling, ctx, smt);
    var_lookup(node->child, ctx, smt);
    if (!node->token) return 0;

    if (TKN_isvariable(node->token)) {
        variable_info_t varinfo = { .type = UNKNOWN_NUMERIC_TOKEN };
        for (int s = ctx->scopes.stack.top; s >= 0; s--) {
            int s_id = ctx->scopes.stack.data[s].id;
            if (VRTB_get_info(node->token->value, s_id, &varinfo, &smt->v)) {
                node->sinfo.v_id        = varinfo.v_id;
                node->sinfo.s_id        = varinfo.s_id;
                node->token->flags.heap = varinfo.heap;
                return 1;
            }
        }
    }

    if (node->token->t_type == STRING_VALUE_TOKEN) {
        str_info_t strinfo;
        if (STTB_get_info(node->token->value, &strinfo, &smt->s)) {
            node->sinfo.v_id = strinfo.id;
            return 1;
        }
        else {
            node->sinfo.v_id = STTB_add_info(node->token->value, STR_INDEPENDENT, &smt->s);
            return 1;
        }
    }

    return 0;
}

/* cpl_block.c */
ast_node_t* cpl_parse_block(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt, token_type_t ex);

/* cpl_asm.c */
ast_node_t* cpl_parse_asm(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_cond.c */
ast_node_t* cpl_parse_switch(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_condop(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_decl.c */
ast_node_t* cpl_parse_array_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_func.c */
ast_node_t* cpl_parse_extern(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_rexit(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_funccall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_function(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_import.c */
ast_node_t* cpl_parse_import(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_op.c */
ast_node_t* cpl_parse_expression(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_scope.c */
ast_node_t* cpl_parse_scope(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_start.c */
ast_node_t* cpl_parse_start(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_syscall.c */
ast_node_t* cpl_parse_syscall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

#endif