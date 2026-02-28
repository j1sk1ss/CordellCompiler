#include <ast/astgen/astgen.h>

int cpl_parse_funcdef_args(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    ast_node_t* trg = (ast_node_t*)carry;
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        ast_node_t* arg = NULL;
        if (TKN_is_decl(CURRENT_TOKEN)) {
            arg = cpl_parse_variable_declaration(it, ctx, smt, carry);
        }
        else if (CURRENT_TOKEN->t_type == VAR_ARGUMENTS_TOKEN) {
            arg = AST_create_node(CURRENT_TOKEN);
            forward_token(it, 1);
        }
        else if (CURRENT_TOKEN->t_type == ANNOTATION_TOKEN) {
            cpl_parse_annot(it, ctx, smt, carry);
            forward_token(it, 1);
            continue;
        }
        else {
            PARSE_ERROR("Error during the argument parsing! Unknown token=%i!", CURRENT_TOKEN->t_type);
            RESTORE_TOKEN_POINT;
            return 0;
        }

        if (arg) AST_add_node(trg, arg);
        else {
            PARSE_ERROR("Error during the argument parsing! (<type> <name>)!");
            RESTORE_TOKEN_POINT;
            return 0;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    }

    return 1;
}

ast_node_t* cpl_parse_function(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the function!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, FUNC_NAME_TOKEN)) {
        PARSE_ERROR("Expected 'FUNC_NAME_TOKEN' token!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    if (name) AST_add_node(base, name);
    else {
        PARSE_ERROR("Can't create a base for the function's name!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected 'OPEN_BRACKET_TOKEN' token!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* args = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (args) AST_add_node(base, args);
    else {
        PARSE_ERROR("Can't create a base for the function's arguments!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    args->sinfo.s_id = ctx->scopes.s_id;

    int local  = ctx->carry.ptr ? 1 : 0;
    int global = base->t->flags.glob;
    annotations_summary_t annots = { .section = NULL, .is_entry = 0, .is_naked = 0 };
    ANNOT_read_annotations(&ctx->annots, &annots);

    forward_token(it, 1);
    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)args)) {
        PARSE_ERROR("Can't parse function's arguments!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (consume_token(it, RETURN_TYPE_TOKEN)) {
        forward_token(it, 1);
        ast_node_t* ret_type = AST_create_node(CURRENT_TOKEN);
        AST_add_node(name, ret_type);
        forward_token(it, 1);
    }

    name->sinfo.v_id = FNTB_add_info(
        name->t->body, 
        global, local, annots.is_entry, annots.is_naked, 
        name->sinfo.s_id, args, name->c, &smt->f
    );

    if (!local) {
        if (!annots.section) annots.section = create_string(CONF_get_code_section());
        SCTB_move_to_section(annots.section, name->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
    }

    ANNOT_destroy_summary(&annots);

    if (CURRENT_TOKEN->t_type == DELIMITER_TOKEN) {
        base->t->t_type = FUNC_PROT_TOKEN;
        stack_pop(&ctx->scopes.stack, NULL);
        return base;
    }

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ body = cpl_parse_scope(it, ctx, smt, 1); }, base);
    if (body) AST_add_node(args, body);
    else {
        PARSE_ERROR("Error during the function's body parsing!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_pop(&ctx->scopes.stack, NULL);
    return base;
}