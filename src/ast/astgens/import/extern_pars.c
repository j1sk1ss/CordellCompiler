/* Import logic statements parser
   - extern <type> <something> */
#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_extern(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the extern statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != DELIMITER_TOKEN) {
        if (!TKN_isdecl(CURRENT_TOKEN) && CURRENT_TOKEN->t_type != FUNC_NAME_TOKEN) forward_token(it, 1);
        else if (TKN_isdecl(CURRENT_TOKEN)) {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("Extern variable declaration error! extern <type> <name>!");
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(node, arg); 
        }
        else if (CURRENT_TOKEN->t_type == FUNC_NAME_TOKEN) {
            ast_node_t* fname = AST_create_node(CURRENT_TOKEN);
            if (!fname) {
                print_error("Extern function declaration error! extern <name>!");
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(node, fname);
            FNTB_add_info(fname->t->body, 1, 1, 0, NULL, NULL, &smt->f);
            forward_token(it, 1);
        }
    }

    return node;
}