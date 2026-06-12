#include <ast/astgen/astgen.h>

static int _extract_params_from_brackets(list_iter_t* it, token_t** first, token_t** second) {
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) return 1;
    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        if (!*first) *first = CURRENT_TOKEN;
        else if (!*second) *second = CURRENT_TOKEN;

        forward_token(it, 1);
    }

    if (!CURRENT_TOKEN) return 0;
    consume_token(it, CLOSE_BRACKET_TOKEN);
    return 1;
}

#define PACK_PARAM(t, box) \
    (box)->string = t ? t->body : NULL; \
    (box)->value  = t ? t->body->to_llong(t->body) : FIELD_NO_CHANGE;
#define ADD_ANNOTATION_HANDLER(n, t)               \
    if (raw_annot->requals(raw_annot, n)) {        \
        return ANNOT_create_annotation(t, &a, &b); \
    }
static annotation_t* _parse_annotation_content(list_iter_t* it) {
    token_t *fp = NULL, *sp = NULL;
    string_t* raw_annot = CURRENT_TOKEN->body;

    _extract_params_from_brackets(it, &fp, &sp);

    annotation_param_t a, b;
    PACK_PARAM(fp, &a);
    PACK_PARAM(sp, &b);
    
    ADD_ANNOTATION_HANDLER(SECTN_ANNOTATION_COMMAND, SECTION_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NOSEC_ANNOTATION_COMMAND, NOSECTION_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ALIGN_ANNOTATION_COMMAND, ALIGN_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ADDRS_ANNOTATION_COMMAND, ADDRESS_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NAKED_ANNOTATION_COMMAND, NAKED_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ENTRY_ANNOTATION_COMMAND, ENTRY_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NOFAL_ANNOTATION_COMMAND, NOFALL_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NTLAZ_ANNOTATION_COMMAND, NOTLAZY_ANNOTATION);
    ADD_ANNOTATION_HANDLER(STRGH_ANNOTATION_COMMAND, STRAIGHT_ANNOTATION);
    ADD_ANNOTATION_HANDLER(COUNT_ANNOTATION_COMMAND, COUNTER_ANNOTATION);
    ADD_ANNOTATION_HANDLER(HOTSC_ANNOTATION_COMMAND, HOT_ANNOTATION);
    ADD_ANNOTATION_HANDLER(COLDS_ANNOTATION_COMMAND, COLD_ANNOTATION);
    ADD_ANNOTATION_HANDLER(REGST_ANNOTATION_COMMAND, REGISTER_ANNOTATION);
    ADD_ANNOTATION_HANDLER(POPRG_ANNOTATION_COMMAND, POPARG_ANNOTATION);
    ADD_ANNOTATION_HANDLER(INLNE_ANNOTATION_COMMAND, INLINE_ANNOTATION);
    ADD_ANNOTATION_HANDLER(SSELF_ANNOTATION_COMMAND, SELF_ANNOTATION);
    ADD_ANNOTATION_HANDLER(LIKEC_ANNOTATION_COMMAND, LIKEC_ANNOTATION);
    ADD_ANNOTATION_HANDLER(UNION_ANNOTATION_COMMAND, UNION_ANNOTATION);
    ADD_ANNOTATION_HANDLER(WEAKS_ANNOTATION_COMMAND, WEAK_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ABICC_ANNOTATION_COMMAND, ABI_ANNOTATION);
    ADD_ANNOTATION_HANDLER(BODYO_ANNOTATION_COMMAND, ONLYBODY_ANNOTATION);
    return ANNOT_create_annotation(UNKNOWN_ANNOTATION, NULL, FIELD_NO_CHANGE);
}
#undef ADD_ANNOTATION_HANDLER
#undef PACK_PARAM

ast_node_t* cpl_parse_annot(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    if (!consume_token(it, OPEN_INDEX_TOKEN)) {
        PARSE_ERROR("'@' should be followed by '['!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, UNKNOWN_STRING_TOKEN)) {
        PARSE_ERROR("Expected the 'unknown string' token after the annotation's start!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    annotation_t* annot = _parse_annotation_content(it);
    if (annot) stack_push(&ctx->annots, annot);
    else {
        PARSE_ERROR("Annotation parse error!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return NULL;
}
