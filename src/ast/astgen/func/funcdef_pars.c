#include <ast/astgen/astgen.h>

int cpl_parse_funcdef_args(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    ast_node_t* trg = (ast_node_t*)carry;
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        ast_node_t* arg = NULL;
        symbol_id_t arg_type = type_lookup(CURRENT_TOKEN, ctx, smt);
        if (
            TKN_is_builtin_type(CURRENT_TOKEN) || arg_type != NO_SYMBOL_ID
        ) arg = cpl_parse_variable_declaration(it, ctx, smt, arg_type);
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

    if (!consume_token(it, UNKNOWN_STRING_TOKEN)) {
        PARSE_ERROR("Expected 'UNKNOWN_STRING_TOKEN' token!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    name->t->t_type = FUNC_NAME_TOKEN;
    if (name) AST_add_node(base, name);
    else {
        PARSE_ERROR("Can't create a base for the function's name!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));

    list_t generic_types;
    list_init(&generic_types);
    
    forward_token(it, 1);
    switch (CURRENT_TOKEN->t_type) {
        case OPEN_BRACKET_TOKEN: break;
        case LOWER_TOKEN: {
            forward_token(it, 1);
            do {
                symbol_id_t t_id = TPTB_add_info(CURRENT_TOKEN->body, ctx->scopes.s_id, TYPE_GENERICS, FIELD_NO_CHANGE, 0, &smt->t);
                if (t_id != NO_SYMBOL_ID) list_add(&generic_types, (void*)t_id);
                if (consume_token(it, COMMA_TOKEN)) forward_token(it, 1);
            } while (CURRENT_TOKEN->t_type != LARGER_TOKEN);
            forward_token(it, 1);
            break;
        }
        default: {
            PARSE_ERROR("Expected either the 'OPEN_BRACKET_TOKEN' or 'LOWER_TOKEN' (<) tokens!");
            AST_unload(base);
            list_free(&generic_types);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }

    ast_node_t* args = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (args) {
        AST_add_node(base, args);
        args->sinfo.s_id = ctx->scopes.s_id;
    }
    else {
        PARSE_ERROR("Can't create a base for the function's arguments!");
        AST_unload(base);
        list_free(&generic_types);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    annotations_summary_t annots = { .section = NULL, .is_entry = 0, .is_naked = 0 };
    ANNOT_read_annotations(&ctx->annots, &annots);

    symbol_id_t preserved_tid = ctx->t_id;
    ctx->t_id = NO_SYMBOL_ID;
    
    forward_token(it, 1);
    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)args)) {
        PARSE_ERROR("Can't parse function's arguments!");
        AST_unload(base);
        list_free(&generic_types);
        ANNOT_destroy_summary(&annots);
        ctx->t_id = preserved_tid;
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (consume_token(it, RETURN_TYPE_TOKEN)) {
        forward_token(it, 1);
        ast_node_t* ret_type = AST_create_node(CURRENT_TOKEN);
        ret_type->sinfo.t_id = type_lookup(ret_type->t, ctx, smt);
        if (ret_type->sinfo.t_id != NO_SYMBOL_ID) {
            ret_type->t->t_type = EXTRACT_TYPE_TYPE(ret_type->sinfo.t_id, smt);
        }
        
        AST_add_node(name, ret_type);
        forward_token(it, 1);
    }

    string_t* virt_name = NULL;
    if (annots.is_entry) {
        if (!annots.fname) annots.fname = create_string(CONF_get_entry_name());  
        virt_name = annots.fname;
    }

    int vargs = 0;
    int local = ctx->carry.pfunc != NO_SYMBOL_ID ? 1 : 0;

    ast_node_t* t;
    for (t = args->c; t && t->t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
        if (t->t->t_type == VAR_ARGUMENTS_TOKEN) {
            vargs = 1;
            break;
        }
    }

    name->sinfo.v_id = FNTB_add_info(
        name->t->body, virt_name, 
        (func_info_flags_t) {
            .global = base->t->flags.glob, .local = local, .entry = annots.is_entry, .naked = annots.is_naked != 0, .vargs = vargs, 
            .generic = list_size(&generic_types) != 0, .inln = annots.do_inline, .self = annots.is_self, .abi = annots.is_abi, .weak = annots.is_weak
        },
        name->sinfo.s_id, args, name->c, &smt->f
    );

    if (preserved_tid != NO_SYMBOL_ID) {
        symbol_id_t type = TPTB_add_info_from_token(base->sinfo.s_id, base->t, base->c->sinfo.v_id, &smt->t);
        TPTB_add_as_child(preserved_tid, type, name->t->body, FIELD_NO_CHANGE, &smt->t);
    }

    if (local) FNTB_add_local(ctx->carry.pfunc, name->sinfo.v_id, &smt->f);
    else { /* Local function doesn't have a section. It copies position of its parent */
        if (annots.is_nosec)      annots.section = create_string(CONF_get_no_section());
        else if (!annots.section) annots.section = create_string(CONF_get_code_section());
        SCTB_move_to_section(annots.section, name->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
    }

    ANNOT_destroy_summary(&annots);

    /* Prototype detected */
    if (CURRENT_TOKEN->t_type == DELIMITER_TOKEN) {
        if (!FNTB_has_generic_types(name->sinfo.v_id, &smt->f)) {
            foreach (symbol_id_t t_id, &generic_types) {
                FNTB_register_generic_type(name->sinfo.v_id, t_id, &smt->f);
            }
        }
        
        base->t->t_type = FUNC_PROT_TOKEN;
        stack_pop(&ctx->scopes.stack, NULL);
        ctx->t_id = preserved_tid;
        list_free(&generic_types);
        return base;
    }

    /* Implementation rewrites prototype's types */
    FNTB_clear_generic_types(name->sinfo.v_id, &smt->f);
    foreach (symbol_id_t t_id, &generic_types) {
        FNTB_register_generic_type(name->sinfo.v_id, t_id, &smt->f);
    }

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ body = cpl_parse_scope(it, ctx, smt, 1); }, name->sinfo.v_id);
    if (body) AST_add_node(args, body);
    else {
        PARSE_ERROR("Error during the function's body parsing!");
        AST_unload(base);
        list_free(&generic_types);
        ctx->t_id = preserved_tid;
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ctx->t_id = preserved_tid;
    list_free(&generic_types);
    stack_pop(&ctx->scopes.stack, NULL);
    return base;
}