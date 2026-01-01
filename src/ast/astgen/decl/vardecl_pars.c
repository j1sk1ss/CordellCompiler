/* Declaration statement parser.
   - <type> <name> = decl */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the variable declaration!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name_node = AST_create_node(CURRENT_TOKEN);
    if (!name_node) {
        print_error("Can't create a node for the variable's name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, name_node);
    forward_token(it, 1);

    long decl_scope;
    stack_top(&ctx->scopes.stack, (void**)&decl_scope);
    name_node->sinfo.v_id = VRTB_add_info(
        name_node->t->body, node->t->t_type, decl_scope, &name_node->t->flags, &smt->v
    );

    if (CURRENT_TOKEN->t_type == ASSIGN_TOKEN) {
        forward_token(it, 1);
        ast_node_t* value_node = cpl_parse_expression(it, ctx, smt);
        if (!value_node) {
            print_error("Error during parsing of the declaration statement!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (node->t->t_type == STR_TYPE_TOKEN) {
            ARTB_add_info(name_node->sinfo.v_id, value_node->t->body->len(value_node->t->body) + 1, 0, I8_TYPE_TOKEN, &smt->a);
            STTB_update_info(value_node->sinfo.v_id, NULL, STR_ARRAY_VALUE, &smt->s);
        }

        AST_add_node(node, value_node);
    }

    var_lookup(name_node, ctx, smt);
    return node;
}
