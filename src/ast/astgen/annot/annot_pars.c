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

static annotation_t* _parse_annotation_content(list_iter_t* it) {
    string_t* raw_annot = CURRENT_TOKEN->body;
    annotation_t* annot = NULL;
    token_t* content    = _extract_token_from_brackets(it);

    if (raw_annot->requals(raw_annot, SECTN_ANNOTATION_COMMAND)) {
        annot = ANNOT_create_annotation(SECTION_ANNOTATION, content->body, FIELD_NO_CHANGE);
    }
    else if (raw_annot->requals(raw_annot, ALIGN_ANNOTATION_COMMAND)) {
        annot = ANNOT_create_annotation(ALIGN_ANNOTATION, NULL, content->body->to_llong(content->body));
    } 
    else if (raw_annot->requals(raw_annot, ADDRS_ANNOTATION_COMMAND)) {
        annot = ANNOT_create_annotation(ADDRESS_ANNOTATION, NULL, content->body->to_llong(content->body));
    }
    else if (raw_annot->requals(raw_annot, NAKED_ANNOTATION_COMMAND)) {
        annot = ANNOT_create_annotation(NAKED_ANNOTATION, NULL, FIELD_NO_CHANGE);
    }
    else if (raw_annot->requals(raw_annot, ENTRY_ANNOTATION_COMMAND)) {
        annot = ANNOT_create_annotation(ENTRY_ANNOTATION, NULL, FIELD_NO_CHANGE);
    }
    else {
        annot = ANNOT_create_annotation(UNKNOWN_ANNOTATION, NULL, FIELD_NO_CHANGE);
    }

    return annot;
}

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
