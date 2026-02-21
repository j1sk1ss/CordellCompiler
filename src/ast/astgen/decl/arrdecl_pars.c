/* Declaration statement parser.
   - arr <name>[<type>, <size>] = decl */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_array_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the array declaration!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name_node = AST_create_node(CURRENT_TOKEN);
    if (!name_node) {
        PARSE_ERROR("Can't create a node for the array name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, name_node);

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type != OPEN_INDEX_TOKEN) {
        PARSE_ERROR("Error during array parsing! arr <name>[<type>, <size>]!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* size_node = AST_create_node(CURRENT_TOKEN);
    if (!size_node) {
        PARSE_ERROR("Can't create a node for the size!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    long long array_size = 1;
    if (size_node->t->t_type != UNKNOWN_NUMERIC_TOKEN) {
        name_node->t->flags.heap = 1;
    }
    else {
        array_size = size_node->t->body->to_llong(size_node->t->body);
    }
    
    AST_add_node(node, size_node);
    
    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type != COMMA_TOKEN) {
        PARSE_ERROR("Error during array parsing! arr <name>[<type>, <size>]!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* elem_type_node = AST_create_node(CURRENT_TOKEN);
    if (!elem_type_node) {
        PARSE_ERROR("Can't create a node for the array type!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type != CLOSE_INDEX_TOKEN) {
        PARSE_ERROR("Error during array parsing! arr <name>[<type>, <size>]!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, elem_type_node);

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type == ASSIGN_TOKEN) {
        forward_token(it, 1);
        if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
            long long act_size = 0;
            forward_token(it, 1);
            while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN) {
                if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                    forward_token(it, 1);
                    continue;
                }

                ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
                if (arg) AST_add_node(node, arg);
                else { 
                    PARSE_ERROR("Error during parsing of the array static element!");
                    AST_unload(node);
                    RESTORE_TOKEN_POINT;
                    return NULL;
                }

                array_size = MAX(array_size, act_size++);
            }

            forward_token(it, 1);
        }
    }

    /* Add variable information. Note here:
       Array, basically, is a pointer. That's why we increment the .ptr flag to 1. */
    long decl_scope;
    stack_top(&ctx->scopes.stack, (void**)&decl_scope);
    name_node->sinfo.v_id = VRTB_add_info(
        name_node->t->body, ARRAY_TYPE_TOKEN, decl_scope, &name_node->t->flags, &smt->v
    );

    ARTB_add_info(
        name_node->sinfo.v_id, array_size, name_node->t->flags.heap, 
        elem_type_node->t->t_type, &elem_type_node->t->flags, &smt->a
    );
    
    var_lookup(name_node, ctx, smt);
    return node;
}