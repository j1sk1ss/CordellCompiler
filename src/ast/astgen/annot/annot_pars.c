#include <ast/astgen/astgen.h>

static token_t* _extract_token_from_brackets(list_iter_t* it) {
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        return NULL;
    }

    forward_token(it, 1);
    token_t* content = CURRENT_TOKEN;

    if (consume_token(it, CLOSE_BRACKET_TOKEN)) forward_token(it, 1);
    else return NULL;
    return content;
}

#define ADD_ANNOTATION_HANDLER(n, t)                                           \
    if (raw_annot->requals(raw_annot, n)) {                                    \
        return ANNOT_create_annotation(                                        \
            t, content ? content->body : NULL,                                 \
            content ? content->body->to_llong(content->body) : FIELD_NO_CHANGE \
        );                                                                     \
    }
static annotation_t* _parse_annotation_content(list_iter_t* it) {
    string_t* raw_annot = CURRENT_TOKEN->body;
    token_t* content    = _extract_token_from_brackets(it);
    ADD_ANNOTATION_HANDLER(SECTN_ANNOTATION_COMMAND, SECTION_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ALIGN_ANNOTATION_COMMAND, ALIGN_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ADDRS_ANNOTATION_COMMAND, ADDRESS_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NAKED_ANNOTATION_COMMAND, NAKED_ANNOTATION);
    ADD_ANNOTATION_HANDLER(ENTRY_ANNOTATION_COMMAND, ENTRY_ANNOTATION);
    ADD_ANNOTATION_HANDLER(NOFAL_ANNOTATION_COMMAND, NOFALL_ANNOTATION);
    ADD_ANNOTATION_HANDLER(STRGH_ANNOTATION_COMMAND, STRAIGHT_ANNOTATION);
    ADD_ANNOTATION_HANDLER(COUNT_ANNOTATION_COMMAND, COUNTER_ANNOTATION);
    ADD_ANNOTATION_HANDLER(HOTSC_ANNOTATION_COMMAND, HOT_ANNOTATION);
    ADD_ANNOTATION_HANDLER(COLDS_ANNOTATION_COMMAND, COLD_ANNOTATION);
    ADD_ANNOTATION_HANDLER(REGST_ANNOTATION_COMMAND, REGISTER_ANNOTATION);
    ADD_ANNOTATION_HANDLER(SIZEV_ANNOTATION_COMMAND, SIZEOF_ANNOTATION);
    return ANNOT_create_annotation(UNKNOWN_ANNOTATION, NULL, FIELD_NO_CHANGE);
}
#undef ADD_ANNOTATION_HANDLER

ast_node_t* cpl_parse_annot(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    if (!consume_token(it, OPEN_INDEX_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_INDEX_TOKEN'!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    annotation_t* annot = _parse_annotation_content(it);
    if (annot) stack_push(&ctx->annots, annot);
    else {
        PARSE_ERROR("Annotation parse error!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return NULL;
}
