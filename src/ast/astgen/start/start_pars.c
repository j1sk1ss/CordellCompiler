#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_start(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the 'start' statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the '(' token during a parse of the 'start' statement!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    annotations_summary_t annots = { .section = NULL };
    ANNOT_read_annotations(&ctx->annots, &annots); 

    forward_token(it, 1);
    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)base)) {
        PARSE_ERROR("Can't parse start's arguments!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the '{' in a body of the 'start' statement! start( ... ) { ... }!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    string_t* main_name = create_string(CONF_get_entry_name());
    if (FNTB_get_info(main_name, -1, NULL, &smt->f)) {
        PARSE_ERROR("The main function already exists!");
        AST_unload(base);
        destroy_string(main_name);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!annots.fname) {
        annots.fname = main_name->copy(main_name);
    }

    string_t* virt_name = annots.fname;
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    base->sinfo.v_id = FNTB_add_info(
        main_name, virt_name, 
        (func_info_flags_t){ .entry = 1, .global = 1, .naked = annots.is_naked, .weak = annots.is_weak }, 
        base->sinfo.s_id, base, NULL, &smt->f
    );
    destroy_string(main_name);

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ body = cpl_parse_scope(it, ctx, smt, 1); }, base->sinfo.v_id);
    if (body) AST_add_node(base, body);
    else {
        PARSE_ERROR("Error during the parsing of the 'start' body!");
        AST_unload(base);
        ANNOT_destroy_summary(&annots);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!annots.section) annots.section = create_string(CONF_get_code_section());
    SCTB_move_to_section(annots.section, annots.salign, base->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
    ANNOT_destroy_summary(&annots);
    return base;
}
