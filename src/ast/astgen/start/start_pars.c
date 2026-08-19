#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_start, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the 'start' statement!");
    PARSER_DO_OR_THROW(!consume_token(it, OPEN_BRACKET_TOKEN), base, "Expected the '(' token during a parse of the 'start' statement!");

    annotations_summary_t annots = { .section = NULL, .salign = -1 };
    ANNOT_read_annotations(&ctx->annots, &annots); 

    forward_token(it, 1);
    PARSER_DO_OR_THROW_DO(
        !cpl_parse_funcdef_args(it, ctx, smt, (long)base), "Can't parse start's arguments!", 
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    PARSER_DO_OR_THROW_DO(
        !consume_token(it, OPEN_BLOCK_TOKEN), "Expected the '{' in a body of the 'start' statement! start( ... ) { ... }!",
        { AST_unload(base); ANNOT_destroy_summary(&annots); }
    );

    string_t* main_name = create_string(CONF_get_entry_name());
    PARSER_DO_OR_THROW_DO(
        FNTB_get_info(main_name, -1, NULL, &smt->f), "The main function already exists!", 
        { AST_unload(base); destroy_string(main_name); ANNOT_destroy_summary(&annots); }
    );

    if (!annots.fname) {
        annots.fname = main_name->copy(main_name);
    }

    string_t* virt_name = annots.fname;
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    base->sinfo.v_id = FNTB_add_info(
        main_name, virt_name, 
        (func_info_flags_t){ 
            .entry = 1, .global = 1, .naked = annots.is_naked ? 1 : 0, .onlybody = annots.is_onlybody, .weak = annots.is_weak, .abi = annots.is_abi
        }, 
        base->sinfo.s_id, base, NULL, &smt->f
    );
    destroy_string(main_name);

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ body = cpl_parse_scope(it, ctx, smt, 1); }, base->sinfo.v_id);
    PARSER_DO_OR_THROW_DO(!body, "Error during the parsing of the 'start' body!", { AST_unload(base); ANNOT_destroy_summary(&annots); });
    AST_add_node(base, body);

    if (!annots.section) annots.section = create_string(CONF_get_code_section());
    SCTB_move_to_section(annots.section, annots.salign, base->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
    ANNOT_destroy_summary(&annots);
    return base;
})
