#include <token.h>

static char_type_t _get_char_type(unsigned char ch) {
    if (isalpha(ch) || ch == '_')          return CHAR_ALPHA;
    else if (str_isdigit(ch) || ch == '-') return CHAR_DIGIT;
    else if (ch == '"')                    return CHAR_QUOTE;
    else if (ch == '\'')                   return CHAR_SING_QUOTE;
    else if (ch == '\n')                   return CHAR_NEWLINE;
    else if (ch == ' ')                    return CHAR_SPACE;
    else if (ch == ';')                    return CHAR_DELIMITER;
    else if (ch == ',')                    return CHAR_COMMA;
    else if (ch == ':')                    return CHAR_COMMENT;
    else if (
        ch == '(' || ch == ')' || 
        ch == '[' || ch == ']' || 
        ch == '{' || ch == '}'
    )                                      return CHAR_BRACKET;
    return CHAR_OTHER;
}

static int _add_token(
    token_t** h, token_t** t, 
    token_type_t type, const unsigned char* buffer, size_t len, int line
) {
    token_t* nt = TKN_create_token(type, buffer, len, line);
    if (!nt) return 0;
    if (!*h) {
        *h = nt;
        *t = nt;
        return 1;
    }

    (*t)->next = nt;
    *t = (*t)->next;
    return 1;
}

token_t* TKN_create_token(token_type_t type, const unsigned char* value, size_t len, int line) {
    if (len > TOKEN_MAX_SIZE) return NULL;
    token_t* tkn = mm_malloc(sizeof(token_t));
    if (!tkn) return NULL;
    str_memset(tkn, 0, sizeof(token_t));

    tkn->t_type = type;
    if (value) {
        str_strncpy((char*)tkn->value, (char*)value, len);
    }
    
    tkn->line_number = line;
    if (type == UNKNOWN_NUMERIC_TOKEN) tkn->glob = 1;
    return tkn;
}

typedef struct {
    char         cmt;  // comment
    char         squt; // single quote
    char         mqut; // multiple quote
    int          line;
    char         in_token;
    short        token_len;
    token_type_t ttype;
} tkn_ctx_t;

token_t* TKN_tokenize(int fd) {
    tkn_ctx_t curr_ctx = { .ttype = UNKNOWN_STRING_TOKEN };
    token_t *head = NULL, *tail = NULL;
    unsigned char buffer[BUFFER_SIZE] = { 0 };
    unsigned char token_buf[TOKEN_MAX_SIZE] = { 0 };

    int file_offset = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = pread(fd, buffer, BUFFER_SIZE, file_offset)) > 0) {
        file_offset += bytes_read;
        for (ssize_t i = 0; i < bytes_read; ++i) {
            unsigned char ch = buffer[i];
            char_type_t ct = _get_char_type(ch);

            /* Markdown routine (quotes and comment flags handler) */
            if (ct == CHAR_SING_QUOTE || ct == CHAR_QUOTE || (ct == CHAR_COMMENT && !curr_ctx.squt && !curr_ctx.mqut)) {
                if (ct == CHAR_SING_QUOTE) curr_ctx.squt = !curr_ctx.squt;
                else if (ct == CHAR_QUOTE) curr_ctx.mqut = !curr_ctx.mqut;
                else curr_ctx.cmt = !curr_ctx.cmt;
                continue;
            }

            /* Skip character if this is comment section */
            if (curr_ctx.cmt && !curr_ctx.squt && !curr_ctx.mqut) continue;

            /* If don't reach space or new line (or we in quotes), append character and correct token type */
            if ((ct != CHAR_SPACE && ct != CHAR_NEWLINE) || curr_ctx.squt || curr_ctx.mqut) {
                token_type_t curr_type;
                if (curr_ctx.squt)                 curr_type = CHAR_VALUE_TOKEN;
                else if (curr_ctx.mqut)            curr_type = STRING_VALUE_TOKEN;
                else {
                    if (ct == CHAR_ALPHA)          curr_type = UNKNOWN_STRING_TOKEN;
                    else if (ct == CHAR_DIGIT)     curr_type = UNKNOWN_NUMERIC_TOKEN;
                    else if (ct == CHAR_DELIMITER) curr_type = DELIMITER_TOKEN;
                    else if (ct == CHAR_COMMA)     curr_type = COMMA_TOKEN;
                    else if (ct == CHAR_BRACKET)   curr_type = UNKNOWN_BRACKET_VALUE;
                    else                           curr_type = UNKNOWN_CHAR_VALUE;
                }
                
                /* If we still in existed token end formation and move to next */
                if (curr_ctx.in_token && curr_ctx.ttype != curr_type) {
                    if (!_add_token(&head, &tail, curr_ctx.ttype, token_buf, curr_ctx.token_len, curr_ctx.line)) {
                        print_error(
                            "Can't add token! type=%i token_buf=[%s], token_len=%i", 
                            curr_ctx.ttype, token_buf, curr_ctx.token_len
                        );
                        goto error;
                    }

                    curr_ctx.in_token = 0;
                }
                
                /* If we are not in token, switch type and mark state via flags */
                if (!curr_ctx.in_token) {
                    curr_ctx.ttype     = curr_type;
                    curr_ctx.token_len = 0;
                    curr_ctx.in_token  = 1;
                }
                
                if (curr_ctx.token_len + 1 > TOKEN_MAX_SIZE) {
                    print_error("Token too large!");
                    goto error;
                }

                token_buf[curr_ctx.token_len++] = ch;
            }
            else {
                if (curr_ctx.in_token) {
                    if (ct == CHAR_NEWLINE) curr_ctx.line++;
                    if (!_add_token(&head, &tail, curr_ctx.ttype, token_buf, curr_ctx.token_len, curr_ctx.line)) {
                        print_error(
                            "Can't add token! type=%i token_buf=[%s], token_len=%i", 
                            curr_ctx.ttype, token_buf, curr_ctx.token_len
                        );
                        goto error;
                    }

                    curr_ctx.in_token = 0;
                }
            }
        }
    }

    if (bytes_read < 0) {
        print_error("Invalid read size!");
        goto error;
    }

    /* Force token close and save */
    if (curr_ctx.in_token) {
        if (!_add_token(&head, &tail, curr_ctx.ttype, token_buf, curr_ctx.token_len, curr_ctx.line)) {
            print_error(
                "Can't add token! type=%i token_buf=[%s], token_len=%i", 
                curr_ctx.ttype, token_buf, curr_ctx.token_len
            );
            goto error;
        }
    }
    
    return head;

error:
    TKN_unload(head);
    return NULL;
}

int TKN_unload(token_t* head) {
    while (head) {
        token_t* next = head->next;
        mm_free(head);
        head = next;
    }
    
    return 1;
}