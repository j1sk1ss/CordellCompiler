#include <token.h>

typedef enum {
    CHAR_ALPHA,
    CHAR_DIGIT,
    CHAR_QUOTE,
    CHAR_SING_QUOTE,
    CHAR_BRACKET,
    CHAR_OTHER,
    CHAR_SPACE,
    CHAR_DELIMITER,
    CHAR_COMMA,
    CHAR_COMMENT,
    CHAR_NEWLINE,
    CHAR_BACKSLASH,
} char_type_t;

static char_type_t _get_char_type(unsigned char ch) {
    if (isalpha(ch) || ch == '_')          return CHAR_ALPHA;
    else if (ch == '\\')                   return CHAR_BACKSLASH;
    else if (str_isdigit(ch) || ch == '-') return CHAR_DIGIT;
    else if (ch == '"')                    return CHAR_QUOTE;
    else if (ch == '\'')                   return CHAR_SING_QUOTE;
    else if (ch == '\n')                   return CHAR_NEWLINE;
    else if (ch == ' ' || ch == '\t')      return CHAR_SPACE;
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
    token_type_t type, const char* buffer, size_t len, int line
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

token_t* TKN_create_token(token_type_t type, const char* value, size_t len, int line) {
    if (len > TOKEN_MAX_SIZE) return NULL;
    token_t* tkn = mm_malloc(sizeof(token_t));
    if (!tkn) return NULL;
    str_memset(tkn, 0, sizeof(token_t));

    tkn->t_type = type;
    if (value) {
        str_strncpy(tkn->value, value, len);
    }
    
    tkn->lnum = line;
    if (type == UNKNOWN_NUMERIC_TOKEN) {
        tkn->vinfo.glob = 1;
    }
    
    return tkn;
}

typedef struct {
    char         cmt;  // comment
    char         squt; // single quote
    char         mqut; // multiple quote
    int          line;
    char         is_spec;
    char         in_token;
    short        token_len;
    token_type_t ttype;
} tkn_ctx_t;

token_t* TKN_tokenize(int fd) {
    tkn_ctx_t curr_ctx = { .ttype = UNKNOWN_STRING_TOKEN };
    token_t *head = NULL, *tail = NULL;
    char buffer[BUFFER_SIZE] = { 0 };
    char token_buf[TOKEN_MAX_SIZE] = { 0 };

    int file_offset = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = pread(fd, buffer, BUFFER_SIZE, file_offset)) > 0) {
        file_offset += bytes_read;
        for (ssize_t i = 0; i < bytes_read; ++i) {
            char ch = buffer[i];
            char_type_t ct = _get_char_type(ch);

            /* Special characters handler */
            if (ct == CHAR_BACKSLASH) {
                curr_ctx.is_spec = !curr_ctx.is_spec;
                continue;
            }

            if (curr_ctx.is_spec) {
                switch (ch) {
                    case '0':  ch = 0;    break;
                    case 'n':  ch = 10;   break;
                    case 't':  ch = 9;    break;
                    case 'r':  ch = 13;   break;
                    case '\\': ch = '\\'; break;
                    case '\'': ch = '\''; break;
                    case '\"': ch = '\"'; break;
                    default: break;
                }
                
                curr_ctx.is_spec = !curr_ctx.is_spec;
            }

            /* Markdown routine (quotes and comment flags handler) */
            if (ct == CHAR_SING_QUOTE || ct == CHAR_QUOTE || (ct == CHAR_COMMENT && !curr_ctx.squt && !curr_ctx.mqut)) {
                if (ct == CHAR_SING_QUOTE) curr_ctx.squt = !curr_ctx.squt;
                else if (ct == CHAR_QUOTE) curr_ctx.mqut = !curr_ctx.mqut;
                else                       curr_ctx.cmt = !curr_ctx.cmt;
                continue;
            }

            /* Skip character if this is comment section */
            if (curr_ctx.cmt && !curr_ctx.squt && !curr_ctx.mqut) continue;

            /* Determine character type */
            token_type_t char_type;
            if (curr_ctx.squt)                 char_type = CHAR_VALUE_TOKEN;
            else if (curr_ctx.mqut)            char_type = STRING_VALUE_TOKEN;
            else {
                if (ct == CHAR_ALPHA)          char_type = UNKNOWN_STRING_TOKEN;
                else if (ct == CHAR_DIGIT)     char_type = UNKNOWN_NUMERIC_TOKEN;
                else if (ct == CHAR_DELIMITER) char_type = DELIMITER_TOKEN;
                else if (ct == CHAR_COMMA)     char_type = COMMA_TOKEN;
                else if (ct == CHAR_BRACKET)   char_type = UNKNOWN_BRACKET_VALUE;
                else if (
                    ct == CHAR_SPACE || 
                    ct == CHAR_NEWLINE
                )                              char_type = LINE_BREAK_TOKEN;
                else                           char_type = UNKNOWN_CHAR_TOKEN;
                if (ct == CHAR_NEWLINE)        curr_ctx.line++;
            }
            
            if (
                curr_ctx.in_token && 
                (
                    char_type == LINE_BREAK_TOKEN ||
                    // char_type == UNKNOWN_CHAR_TOKEN ||  /* Chars can't be larger then 1 symbol */
                    char_type == UNKNOWN_BRACKET_VALUE ||  /* Same with the brackets */
                    curr_ctx.token_len + 1 > TOKEN_MAX_SIZE ||
                    curr_ctx.ttype != char_type
                )
            ) {
                if (!_add_token(&head, &tail, curr_ctx.ttype, token_buf, curr_ctx.token_len, curr_ctx.line)) {
                    print_error(
                        "Can't add token! type=%i, token_buf=[%s], token_len=%i", 
                        curr_ctx.ttype, token_buf, curr_ctx.token_len
                    );
                    TKN_unload(head);
                    return NULL;
                }

                curr_ctx.in_token  = 0;
                curr_ctx.token_len = 0;
            }

            if (char_type == LINE_BREAK_TOKEN) continue;
            if (!curr_ctx.in_token) {
                curr_ctx.ttype = char_type;
                curr_ctx.in_token = 1;
            }

            token_buf[curr_ctx.token_len++] = ch;
        }
    }
    
    return head;
}

int TKN_unload(token_t* head) {
    while (head) {
        token_t* next = head->next;
        mm_free(head);
        head = next;
    }
    
    return 1;
}