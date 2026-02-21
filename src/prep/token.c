/* This is the main token generation script.
   In the nutshell - this is a DFA (Deterministic finite automaton).
   
   The first thing that we must to do - is reading. This module reads an
   input file by chunks. Chunks can be set with the BUFFER_SIZE macros in 
   the `token.h` file. */
#include <prep/token.h>

typedef enum {
    CHAR_PP,         /* Pre-processor symbol: #          */
    CHAR_ALPHA,      /* Any alphabet character: [A-Za-z] */
    CHAR_DIGIT,      /* Any digit: [0-9]                 */
    CHAR_QUOTE,      /* Quote: "                         */
    CHAR_OTHER,      /* Unknown character type           */
    CHAR_SPACE,      /* Space: ' '                       */
    CHAR_COMMA,      /* Comma: ,                         */
    CHAR_BRACKET,    /* Bracket: [ [, {, (, ), }, ] ]    */
    CHAR_NEWLINE,    /* New line character (\n)          */
    CHAR_DELIMITER,  /* Delimiter character: ;           */
    CHAR_BACKSLASH,  /* Backslash: \                     */
    CHAR_SING_QUOTE, /* Single quote: '                  */
    CHAR_SIGN,       /* Signs: -, +                      */
    CHAR_DOT,
} char_type_t;

/*
Give a character type depends on the input character.
Params:
    - `ch` - Input character.

Returns character type.
*/
static char_type_t _get_char_type(unsigned char ch) {
    if (str_isalpha(ch) || ch == '_') return CHAR_ALPHA;
    else if (str_isdigit(ch))         return CHAR_DIGIT;
    
    switch (ch) {
        case '#':  return CHAR_PP;
        case '\n': return CHAR_NEWLINE;
        case '\\': return CHAR_BACKSLASH;
        case '\'': return CHAR_SING_QUOTE;
        case ',':  return CHAR_COMMA;
        case '.':  return CHAR_DOT;
        case '"':  return CHAR_QUOTE;
        case ';':  return CHAR_DELIMITER;
        case ' ':  case '\t': return CHAR_SPACE;
        case '(':  case '[':
        case '{':  case ')':
        case ']':  case '}':  return CHAR_BRACKET;
        case '-':  case '+':  
        case '/':  case '*':
        case '%':  case '=':
        case '<':  case '>':
        case '!':
        case '&':  case '|':  return CHAR_SIGN;
        default:   return CHAR_OTHER;
    }
}

typedef struct {
    char         squt     : 1; /* Is signle quote has started?     */
    char         mqut     : 1; /* Is quote has started?            */
    char         is_spec  : 1; /* Is this is a special char?       */
    char         in_token : 1; /* Are we in a token?               */
    char         is_pp    : 1; /* Is this a pp directive?          */
    short        token_len;    /* Token len                        */
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
            int is_float = 0;
            tkn->body = input->from_number(input, &is_float);
            if (is_float) tkn->t_type = UNKNOWN_FLOAT_NUMERIC_TOKEN;
            destroy_string(input);
            break;
        }
        case CHAR_VALUE_TOKEN: {
            tkn->body = create_string_from_int(input->body[0]);
            tkn->t_type = UNKNOWN_NUMERIC_TOKEN;
            destroy_string(input);
            break;
        }
        case UNKNOWN_SIGN_TOKEN: {
            tkn->body = input;
            tkn->t_type = UNKNOWN_CHAR_TOKEN;
            break;
        }
        default: {
            tkn->body = input;
            break;
        }
    }
    
    str_memcpy(&tkn->finfo, finfo, sizeof(token_fpos_t));
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
    if (
        (b1 == 0xD0 && b2 >= 0x90 && b2 <= 0xAF) ||
        (b1 == 0xD0 && b2 >= 0xB0 && b2 <= 0xBF) ||
        (b1 == 0xD1 && b2 >= 0x80 && b2 <= 0x8F)
    ) return 1;
    return 0;
}

/*
Reset the input ctx to zero.
Params:
    - `ctx` - Token's context.
*/
static inline void _reset_tkn_ctx(tkn_ctx_t* ctx) {
    ctx->in_token  = 0;
    ctx->token_len = 0;
    ctx->ttype = LINE_BREAK_TOKEN;
}

/* Get a new type for a symbol according to the current token type and 
   a new type of a symbol. */
#define PROPOGATE_SYMBOL_TYPE(curr, new, res) \
    if (                                      \
        ctx->ttype == curr &&                 \
        char_type == new                      \
    ) char_type = res;

/* Get a new type for the token according to the current token type and 
   a new type of a symbol. */
#define PROPOGATE_TOKEN_TYPE(curr, new, res) \
    if (                                     \
        ctx->ttype == curr &&                \
        char_type == new                     \
    ) ctx->ttype = res;

/*
Give the next token from the provided buffer.
Params:
    - `buffer` - Source pre-processed buffer of the code.
    - `bytes_read` - Buffer's size.
    - `off` - Current offset in the buffer.
    - `finfo` - Current information about a position in a file.

Returns a new token or the 'NULL' value.
*/
static token_t* _give_next_token(char* buffer, ssize_t bytes_read, ssize_t* off, token_fpos_t* finfo, tkn_ctx_t* ctx) {
    char token_buf[BUFFER_SIZE] = { 0 };
    for (ssize_t i = *off; i < bytes_read; ++i) {
        /* Check if this a permitted character in the
           compiler. */
        if (i + 1 < bytes_read && _permitted_character(buffer + i)) {
            print_error("Permitted symbol detected! c='%.2s'", buffer + i);
            return NULL;
        }

        char ch = buffer[i];
        char_type_t ct = _get_char_type(ch);
        finfo->column++;

        if (ct == CHAR_PP) {
            ctx->is_pp = 1;
            continue;
        }

        /* Special character logic
            proceeding. Also, if we encounter backslash,
            we must skip it. */
        if (ct == CHAR_BACKSLASH) {
            ctx->is_spec = !ctx->is_spec;
            continue;
        }
        
        if (ctx->is_spec) {
            switch (ch) {
                case '0': ch = '\0'; break;
                case 'n': ch = '\n'; break;
                case 't': ch = '\t'; break;
                case 'r': ch = '\r'; break;
                default: break;
            }
            
            ctx->is_spec = !ctx->is_spec;
        }

        /* Markdown routine (quotes and comment flags handler) */
        if (ct == CHAR_SING_QUOTE || ct == CHAR_QUOTE) {
            if (ct == CHAR_SING_QUOTE) ctx->squt = !ctx->squt;
            else if (ct == CHAR_QUOTE) ctx->mqut = !ctx->mqut;
            continue;
        }
        
        /* Determine character type 
            and act according to the logic. 
            - If this is a new line, reset the column counter 
                and increment the line counter. */
        if (!ctx->in_token && ct == CHAR_NEWLINE) {
            finfo->line++;
            finfo->column = 1;
        }

        /* Read and convert the input character type to a
            defined token type. */
        token_type_t char_type;
        if (ctx->squt)               char_type = CHAR_VALUE_TOKEN;
        else if (ctx->mqut)          char_type = STRING_VALUE_TOKEN;
        else {
            switch (ct) {
                case CHAR_ALPHA:     char_type = UNKNOWN_STRING_TOKEN;  break;
                case CHAR_DIGIT:     char_type = UNKNOWN_NUMERIC_TOKEN; break;
                case CHAR_DELIMITER: char_type = DELIMITER_TOKEN;       break;
                case CHAR_COMMA:     char_type = COMMA_TOKEN;           break;
                case CHAR_BRACKET:   char_type = UNKNOWN_BRACKET_VALUE; break;
                case CHAR_SPACE:
                case CHAR_NEWLINE:   char_type = LINE_BREAK_TOKEN;      break;
                case CHAR_SIGN:      char_type = UNKNOWN_SIGN_TOKEN;    break;
                default:             char_type = UNKNOWN_CHAR_TOKEN;    break;
            }

            PROPOGATE_SYMBOL_TYPE(UNKNOWN_NUMERIC_TOKEN, UNKNOWN_CHAR_TOKEN, UNKNOWN_NUMERIC_TOKEN);
            PROPOGATE_TOKEN_TYPE(UNKNOWN_SIGN_TOKEN, UNKNOWN_NUMERIC_TOKEN, UNKNOWN_NUMERIC_TOKEN);
            PROPOGATE_SYMBOL_TYPE(UNKNOWN_STRING_TOKEN, UNKNOWN_NUMERIC_TOKEN, UNKNOWN_STRING_TOKEN);
            PROPOGATE_SYMBOL_TYPE(UNKNOWN_NUMERIC_TOKEN, UNKNOWN_STRING_TOKEN, UNKNOWN_NUMERIC_TOKEN);
        }
        
        /* Create a new token.
            Here we can proccess token's content before create it
            and push it to the list. */
        if (
            ctx->in_token &&                          /* If we're in the token */
            (
                char_type == LINE_BREAK_TOKEN ||      /* We've found a break token    */
                char_type == UNKNOWN_BRACKET_VALUE || /* or unknown token.            */
                ctx->ttype != char_type               /* Or we've found another char. */
            )
        ) {
_force_token_creation: {}
            /* Token data preparation
                We need to be sure:
                - This is a correct column is used
                - This is a correct buffer is used */
            token_buf[ctx->token_len] = 0; /* Set the end of the token */

            /* Special case. If this is a directive (any directive),
               we force type to the 'PP_TOKEN' type */
            if (ctx->is_pp) {
                ctx->ttype = PP_TOKEN;
                ctx->is_pp = 0;
            }

            token_t* nt = TKN_create_token(ctx->ttype, token_buf, finfo);
            if (!nt) {
                print_error("Can't create a token! tt=%i, tb=[%s], tl=%i", ctx->ttype, token_buf, ctx->token_len);
                TKN_unload_token(nt);
                return NULL;
            }

            _reset_tkn_ctx(ctx);
            *off = i;
            return nt;
        }

        if (char_type == LINE_BREAK_TOKEN) continue;
        if (!ctx->in_token) {
            ctx->ttype = char_type;
            ctx->in_token = 1;
        }

        if (ctx->token_len + 1 > BUFFER_SIZE) {
            print_error("Too large token is found!");
            return NULL;
        }

        token_buf[ctx->token_len++] = ch;
    }

    if (ctx->in_token) goto _force_token_creation;
    return NULL;
}

int TKN_tokenize(int fd, list_t* tkn) {
    tkn_ctx_t tkn_ctx = { 0 };
    _reset_tkn_ctx(&tkn_ctx);

    token_fpos_t finfo = { .column = 1, .line = 1, .file = NULL };
    char buffer[BUFFER_SIZE] = { 0 };

    int file_offset = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = pread(fd, buffer, BUFFER_SIZE, file_offset)) > 0) {
        ssize_t buffer_off = 0;
        token_t* token;
        while ((token = _give_next_token(buffer, bytes_read, &buffer_off, &finfo, &tkn_ctx))) {
            switch (token->t_type) {
                /* If we meet such a token type, we need to
                   take the next token and reset our file info. */
                case PP_TOKEN: {
                    if (!token->body->requals(token->body, PP_LINE_DIRECTIVE)) {
                        TKN_unload_token(token);
                        break;
                    }

                    token_t* fline = _give_next_token(buffer, bytes_read, &buffer_off, &finfo, &tkn_ctx);
                    token_t* fname = _give_next_token(buffer, bytes_read, &buffer_off, &finfo, &tkn_ctx);
                    if (!fline || !fname) {
                        print_error("Incorrect the 'PP_TOKEN' token is found! Must be 'PP_TOKEN' <int> 'file_name'!");
                        TKN_unload_token(token);
                        TKN_unload_token(fline);
                        TKN_unload_token(fname);
                        list_free_force_op(tkn, (int (*)(void*))TKN_unload_token);
                        return 0;
                    }

                    finfo.column = 1;
                    finfo.line   = fline->body->to_llong(fline->body);
                    finfo.file   = fname->body;

                    TKN_unload_token(token);
                    TKN_unload_token(fline);

                    fname->t_type = INCLUDE_FILE_TOKEN;
                    list_add(tkn, fname);
                    break;
                }

                default: list_add(tkn, token); break;
            }
        }

        file_offset += bytes_read;
    }

    return 1;
}

unsigned long TKN_hash_token(token_t* t) {
    token_fpos_t tmp = { .column = t->finfo.column, .line = t->finfo.line, .file = t->finfo.file };
    str_memset(&t->finfo, 0, sizeof(token_fpos_t));

    unsigned long hash = crc64((const unsigned char*)&t->flags, sizeof(token_flags_t), 0);
    hash ^= t->body->hash;
    hash *= t->t_type;
    
    t->finfo.line   = tmp.line;
    t->finfo.column = tmp.column;
    t->finfo.file   = tmp.file;
    return hash;
}

int TKN_unload_token(token_t* t) {
    if (!t) return 0;
    destroy_string(t->body);
    return mm_free(t);
}
