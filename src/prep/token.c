/* This is the main token generation script.
   In the nutshell - this is a DFA (Deterministic finite automaton).
   
   The first thing that we must to do - is reading. This module reads an
   input file by chunks. Chunks can be set with the BUFFER_SIZE macros in 
   the `token.h` file. */
#include <prep/token.h>

typedef enum {
    CHAR_ALPHA,     /* Any alphabet character: [A-Za-z] */
    CHAR_DIGIT,     /* Any digit: [0-9]                 */
    CHAR_QUOTE,     /* Quote: "                         */
    CHAR_OTHER,     /* Unknown character type           */
    CHAR_SPACE,     /* Space: ' '                       */
    CHAR_COMMA,     /* Comma: ,                         */
    CHAR_BRACKET,   /* Bracket: [ [, {, (, ), }, ] ]    */
    CHAR_COMMENT,   /* Comment: ':'                     */
    CHAR_NEWLINE,   /* New line character (\n)          */
    CHAR_DELIMITER, /* Delimiter character: ;           */
    CHAR_BACKSLASH, /* Backslash: \                     */
    CHAR_SING_QUOTE /* Single quote: '                  */
} char_type_t;

/*
Give a character type depends on the input character.
Params:
    - `ch` - Input character.

Returns character type.
*/
static char_type_t _get_char_type(unsigned char ch) {
    if (isalpha(ch) || ch == '_') return CHAR_ALPHA;
    else if (str_isdigit(ch))     return CHAR_DIGIT;
    
    switch (ch) {
        case '\n': return CHAR_NEWLINE;
        case '\\': return CHAR_BACKSLASH;
        case '\'': return CHAR_SING_QUOTE;
        case ',':  return CHAR_COMMA;
        case '"':  return CHAR_QUOTE;
        case ':':  return CHAR_COMMENT;
        case ';':  return CHAR_DELIMITER;
        case ' ':  case '\t': return CHAR_SPACE;
        case '(':  case '[':
        case '{':  case ')':
        case ']':  case '}':  return CHAR_BRACKET;
        default:   return CHAR_OTHER;
    }
}

typedef struct {
    char         neg;       /* Is there is a minus sign before? */
    char         cmt;       /* Is comment has started?          */
    char         squt;      /* Is signle quote has started?     */
    char         mqut;      /* Is quote has started?            */
    char         is_spec;   /* Is this is a special char?       */
    char         in_token;  /* Are we in a token?               */
    short        token_len; /* Token len                        */
    token_fpos_t finfo;
    token_type_t ttype;
} tkn_ctx_t;

token_t* TKN_copy_token(token_t* src) {
    token_t* tkn = mm_malloc(sizeof(token_t));
    if (!tkn) return NULL;
    str_memcpy(tkn, src, sizeof(token_t));
    tkn->body = src->body->copy(src->body);
    if (!tkn->body) {
        mm_free(tkn);
        return NULL;
    }

    return tkn;
}

token_t* TKN_create_token(token_type_t type, const char* value, token_fpos_t* finfo) {
    token_t* tkn = mm_malloc(sizeof(token_t));
    if (!tkn) return NULL;
    str_memset(tkn, 0, sizeof(token_t));

    string_t* input = create_string((char*)value);
    if (!input) {
        mm_free(tkn);
        return NULL;
    }

    tkn->t_type = type;
    switch (type) {
        case UNKNOWN_NUMERIC_TOKEN: {
            tkn->body = input->from_number(input);
            destroy_string(input);
            break;
        }
        case CHAR_VALUE_TOKEN: {
            tkn->body = create_string_from_char(input->body[0]);
            tkn->t_type = UNKNOWN_NUMERIC_TOKEN;
            destroy_string(input);
            break;
        }
        default: {
            tkn->body = input;
            break;
        }
    }
    
    str_memcpy(&tkn->finfo, finfo, sizeof(token_fpos_t));
    if (
        type == UNKNOWN_NUMERIC_TOKEN ||
        type == STRING_VALUE_TOKEN
    ) tkn->flags.glob = 1;
    return tkn;
}

/*
Check if this is a permitted character.
Params:
    - `p` - Input character (1 byte or more than 1 byte size).

Returns 1 if this is a permitted character.
*/
static inline int _permitted_character(char* p) {
    unsigned char b1 = (unsigned char)p[0];
    unsigned char b2 = (unsigned char)p[1];
    if (b1 == 0xD0 && b2 >= 0x90 && b2 <= 0xAF) return 1;
    if (b1 == 0xD0 && b2 >= 0xB0 && b2 <= 0xBF) return 1;
    if (b1 == 0xD1 && b2 >= 0x80 && b2 <= 0x8F) return 1;
    return 0;
}

int TKN_tokenize(int fd, list_t* tkn) {
    tkn_ctx_t curr_ctx = { .ttype = LINE_BREAK_TOKEN, .finfo = { .line = 1, .column = 0 } };
    char buffer[BUFFER_SIZE]    = { 0 };
    char token_buf[BUFFER_SIZE] = { 0 };

    int file_offset = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = pread(fd, buffer, BUFFER_SIZE, file_offset)) > 0) {
        file_offset += bytes_read;
        for (ssize_t i = 0; i < bytes_read; ++i) {
            curr_ctx.finfo.column++;
            if (i + 1 < bytes_read && _permitted_character(buffer + i)) {
                print_error("Permitted symbol detected! %.2s", buffer + i);
                list_free_force_op(tkn, (int (*)(void*))TKN_unload_token);
                return 0;
            }

            char ch = buffer[i];
            if (ch == '-') curr_ctx.neg = 1;
            else curr_ctx.neg = 0;

            char_type_t ct = _get_char_type(ch);
            if (ct == CHAR_BACKSLASH) {
                curr_ctx.is_spec = !curr_ctx.is_spec;
                continue;
            }

            if (curr_ctx.is_spec) {
                switch (ch) {
                    case '0':  ch = '\0'; break;
                    case 'n':  ch = '\n'; break;
                    case 't':  ch = '\t'; break;
                    case 'r':  ch = '\r'; break;
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
            if (curr_ctx.squt)           char_type = CHAR_VALUE_TOKEN;
            else if (curr_ctx.mqut)      char_type = STRING_VALUE_TOKEN;
            else {
                switch (ct) {
                    case CHAR_ALPHA:     char_type = UNKNOWN_STRING_TOKEN;  break;
                    case CHAR_DIGIT:     char_type = UNKNOWN_NUMERIC_TOKEN; break;
                    case CHAR_DELIMITER: char_type = DELIMITER_TOKEN;       break;
                    case CHAR_COMMA:     char_type = COMMA_TOKEN;           break;
                    case CHAR_BRACKET:   char_type = UNKNOWN_BRACKET_VALUE; break;
                    case CHAR_SPACE:
                    case CHAR_NEWLINE:   char_type = LINE_BREAK_TOKEN;      break;
                    default:             char_type = UNKNOWN_CHAR_TOKEN;    break;
                }

                if (ct == CHAR_NEWLINE) {
                    curr_ctx.finfo.line++;
                    curr_ctx.finfo.column = 0;
                }

                if (
                    curr_ctx.ttype == UNKNOWN_CHAR_TOKEN && 
                    char_type == UNKNOWN_NUMERIC_TOKEN
                ) curr_ctx.ttype = UNKNOWN_NUMERIC_TOKEN;
                if (
                    curr_ctx.ttype == UNKNOWN_STRING_TOKEN && 
                    char_type == UNKNOWN_NUMERIC_TOKEN
                ) char_type = UNKNOWN_STRING_TOKEN;
                else if (
                    curr_ctx.ttype == UNKNOWN_NUMERIC_TOKEN && 
                    char_type == UNKNOWN_STRING_TOKEN
                ) char_type = UNKNOWN_NUMERIC_TOKEN;
            }
            
            if (
                curr_ctx.in_token && 
                (
                    char_type == LINE_BREAK_TOKEN ||
                    char_type == UNKNOWN_BRACKET_VALUE ||
                    curr_ctx.ttype != char_type
                )
            ) {
                /* Token data preparation
                   We need to be sure:
                   - This is a correct column is used
                   - This is a correct buffer is used */
                token_buf[curr_ctx.token_len] = 0; /* Set the end of the token */

                token_t* nt = TKN_create_token(curr_ctx.ttype, token_buf, &curr_ctx.finfo);
                if (!nt || !list_add(tkn, nt)) {
                    print_error("Can't add token! tt=%i, tb=[%s], tl=%i", curr_ctx.ttype, token_buf, curr_ctx.token_len);
                    TKN_unload_token(nt);
                    list_free_force_op(tkn, (int (*)(void*))TKN_unload_token);
                    return 0;
                }

                curr_ctx.in_token  = 0;
                curr_ctx.token_len = 0;
                curr_ctx.ttype     = LINE_BREAK_TOKEN;
                curr_ctx.neg       = 0;
            }

            if (char_type == LINE_BREAK_TOKEN) continue;
            if (!curr_ctx.in_token) {
                curr_ctx.ttype = char_type;
                curr_ctx.in_token = 1;
            }

            token_buf[curr_ctx.token_len++] = ch;
        }
    }
    
    return 1;
}

unsigned long TKN_hash_token(token_t* t) {
    token_fpos_t tmp = { .column = t->finfo.column, .line = t->finfo.line };

    t->finfo.line   = 0;
    t->finfo.column = 0;

    unsigned long hash = crc64((const unsigned char*)t, sizeof(token_t), 0);
    hash ^= crc64((const unsigned char*)t->body->body, t->body->size, 0);
    
    t->finfo.line   = tmp.line;
    t->finfo.column = tmp.column;

    return hash;
}

int TKN_unload_token(token_t* t) {
    if (!t) return 0;
    destroy_string(t->body);
    return mm_free(t);
}
