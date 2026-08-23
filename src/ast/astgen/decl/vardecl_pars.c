#include <ast/astgen/astgen.h>

static symbol_id_t _resolve_plain_type(token_t* t, ast_ctx_t* ctx, sym_table_t* smt) {
    symbol_id_t type = type_lookup(t, ctx, smt);
    if (type != NO_SYMBOL_ID)   return type;
    if (TKN_is_builtin_type(t)) return TPTB_add_info_from_token(NO_SYMBOL_ID, t, NO_SYMBOL_ID, &smt->t);
    return NO_SYMBOL_ID;
}

static symbol_id_t _parse_type_id(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

static symbol_id_t _intern_signature_type(list_t* args, symbol_id_t ret, typetab_ctx_t* ctx) {
    symbol_id_t signature = TPTB_get_signature(args, ret, ctx);
    if (signature != NO_SYMBOL_ID) return signature;
    return TPTB_add_signature(args, ret, ctx);
}

static symbol_id_t _parse_signature_type(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!CURRENT_TOKEN || CURRENT_TOKEN->t_type != SIGNATURE_TOKEN) return NO_SYMBOL_ID;
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Signature type parse error! Expected '(' after 'fn'.");
        return NO_SYMBOL_ID;
    }

    list_t args;
    list_init(&args);
#define SIGNATURE_PARSE_ASSERT(action, message) \
    if (action) { \
        PARSE_ERROR("Signature type parse error! Expected argument list."); \
        list_free(&args); \
        return NO_SYMBOL_ID; \
    }
    
    SIGNATURE_PARSE_ASSERT(!forward_token(it, 1), "Signature type parse error! Expected argument list.");
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        symbol_id_t arg_type = _parse_type_id(it, ctx, smt);
        SIGNATURE_PARSE_ASSERT(arg_type == NO_SYMBOL_ID, "Signature type parse error! Unknown argument type.");

        list_add(&args, (void*)arg_type);
        if (consume_token(it, COMMA_TOKEN)) {
            SIGNATURE_PARSE_ASSERT(!forward_token(it, 1), "Signature type parse error! Expected argument type after ','.");
            continue;
        }

        SIGNATURE_PARSE_ASSERT(
            !CURRENT_TOKEN || CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN, 
            "Signature type parse error! Expected ',' or ')'."
        );
    }

    SIGNATURE_PARSE_ASSERT(!CURRENT_TOKEN || !forward_token(it, 1), "Signature type parse error! Expected return type.");
    symbol_id_t ret_type = _parse_type_id(it, ctx, smt);
    SIGNATURE_PARSE_ASSERT(ret_type == NO_SYMBOL_ID, "Signature type parse error! Unknown return type.");
#undef SIGNATURE_PARSE_ASSERT
    symbol_id_t signature = _intern_signature_type(&args, ret_type, &smt->t);
    list_free(&args);
    return signature;
}

static symbol_id_t _parse_type_id(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    return CURRENT_TOKEN->t_type == SIGNATURE_TOKEN ? 
                _parse_signature_type(it, ctx, smt) : 
                _resolve_plain_type(CURRENT_TOKEN, ctx, smt);
}

DEFINE_PARSER(cpl_parse_variable_declaration, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for the variable's declaration type! <type> <name>!");

    if (CURRENT_TOKEN->t_type == SIGNATURE_TOKEN) {
        base->sinfo.t_id = _parse_signature_type(it, ctx, smt);
        PARSER_ASSERT_DO(
            base->sinfo.t_id == NO_SYMBOL_ID, "Can't parse a function signature type! Expected fn(<args>)<ret>.",
            { AST_unload(base); }
        );
    }

    annotations_summary_t annots = { 
        .align  = CONF_get_full_bytness(), .section = NULL, 
        .salign = SMT_NULL,                .reg     = SMT_NULL 
    };

    ANNOT_read_annotations(&ctx->annots, &annots);
    if (annots.is_argpop) list_add(&base->annots, ANNOT_create_annotation(POPARG_ANNOTATION, NULL, 0));

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT_DO(
        !name, "Can't create a base for the variable's name! <type> <name>!", 
        { ANNOT_destroy_summary(&annots); AST_unload(base); }
    );
    AST_add_node(base, name);

    if (carry != NO_SYMBOL_ID) {
        base->sinfo.t_id = carry;
        base->t->t_type  = EXTRACT_TYPE_TYPE(carry, smt);
    }

    /* Register a variable in the symtable and update its type
       from the carry, if we're working with a dynamic type. */
    stack_top(&ctx->scopes.stack, (void**)&name->sinfo.s_id);
    name->sinfo.v_id = VRTB_add_info(name->t->body, base->t->t_type, name->sinfo.s_id, base->t->flags, &smt->v);
    if (CONF_is_symtab_error()) {
        PARSE_ERROR("Can't register a variable with the name '%s'", name->t->body->body);
        CONF_set_parser_error();
    }
    
    if (annots.is_notnull) VRTB_set_not_null(name->sinfo.v_id, &smt->v);
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, carry, &smt->v);

    /* If this is a custom type variable, register it as an array as well. */
    type_info_t ti;
    if (
        base->t->t_type == CUSTOM_TYPE_TOKEN && 
        TPTB_get_info_id(carry, &ti, &smt->t)
    ) ARTB_add_info(name->sinfo.v_id, TPTB_get_memory_size_id(ti.id, &smt->t), 0, U8_TYPE_TOKEN, base->t->flags, &smt->a);

    /* Update variable's memory flags according to the provided annotations
       and move it to a corresponding section. */
    var_lookup(name, ctx, smt);
    VRTB_update_memory(name->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, annots.reg, annots.align, &smt->v);
    if (!TKN_in_stack(name->t)) {
        if (!annots.section) annots.section = create_string(name->t->flags.glob ? CONF_get_glob_section() : CONF_get_ro_section());
        SCTB_move_to_section(annots.section, annots.salign, name->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
    }

    ast_node_t* init_values = cpl_parse_declaration_value(it, ctx, smt, 0);
    if (init_values) AST_add_node(base, init_values);
    
    /* Register the variable as a basic type of the parent type,
       if this is a declaraion in a type. */
    symbol_id_t declared_type = base->sinfo.t_id;
    if (declared_type == NO_SYMBOL_ID) declared_type = type_lookup(base->t, ctx, smt);
    if (declared_type == NO_SYMBOL_ID) {
        declared_type = TPTB_add_info_from_token(base->sinfo.s_id, base->t, NO_SYMBOL_ID, &smt->t);
    }

    base->sinfo.t_id = TPTB_add_copy(declared_type, base->t->flags.ptr, &smt->t);
    VRTB_update_type(name->sinfo.v_id, FIELD_NO_CHANGE, base->sinfo.t_id, &smt->v);

    symbol_id_t pt_id;
    if (stack_top(&ctx->types, (void**)&pt_id)) {
        TPTB_add_as_child(pt_id, base->sinfo.t_id, name->t->body, base->t->flags.ptr ? CONF_get_full_bytness() : SMT_NULL, &smt->t);
    }

    ANNOT_destroy_summary(&annots);
    return base;
})
