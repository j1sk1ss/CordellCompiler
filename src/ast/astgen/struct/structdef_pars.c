#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_structdef(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the struct define command!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (!name) {
        PARSE_ERROR("Error during struct name creation!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, name);

    /* Register a new type in the types table.
       Will return error if there is the existed type with the same name! */
    symbol_id_t tid;
    if ((tid = TPTB_add_info(name->t->body, &smt->t)) < 0) {
        PARSE_ERROR("Error during struct type registration!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_scope(it, ctx, smt, carry);
    if (!name) {
        PARSE_ERROR("Error during struct body creation!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    /* Register all declared variables as fields of the type */
    for (ast_node_t* decl = body->c; decl; decl = decl->siblings.n) {
        if (TKN_isdecl(decl->t)) {
            TPTB_info_add_entry(tid, decl->c->sinfo.v_id, &smt->t);
        }
    }

    AST_add_node(node, body);
    return node;
}
