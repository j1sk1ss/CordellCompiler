#include <preproc/pp.h>

int PP_create_tmp_file() {
    char path[PP_PATH_MAX] = { 0 };
    const char* tmp_dir = getenv("TMPDIR");
    if (!tmp_dir || !tmp_dir[0]) tmp_dir = "/tmp";

    int w = snprintf(path, sizeof(path), "%s/cplc-pp-XXXXXX", tmp_dir);
    if (w < 0 || (size_t)w >= sizeof(path)) {
        fprintf(stderr, "Preprocessor temporary path is too long: %s\n", tmp_dir);
        return -1;
    }

    int tmp_fd = mkstemp(path);
    if (tmp_fd < 0) {
        fprintf(stderr, "Can't create a preprocessor temporary file in %s: %s\n", tmp_dir, strerror(errno));
        return -1;
    }

    unlink(path);
    fchmod(tmp_fd, 0644);

    int flags = fcntl(tmp_fd, F_GETFL);
    if (flags != -1) fcntl(tmp_fd, F_SETFL, flags | O_APPEND);

    fcntl(tmp_fd, F_SETFD, FD_CLOEXEC);
    return tmp_fd;
}

int PP_file_path_from_fp(FILE* fp, char* out, size_t out_sz) {
    if (!fp || !out || out_sz == 0) return 0;
    int fd = fileno(fp);
    if (fd < 0) return 0;
#if defined(__linux__)
    char link_path[64];
    snprintf(link_path, sizeof(link_path), "/proc/self/fd/%d", fd);
    ssize_t n = readlink(link_path, out, out_sz - 1);
    if (n < 0) return 0;
    out[n] = 0;
    return 1;
#elif defined(__APPLE__)
    if (fcntl(fd, F_GETPATH, out) == -1) return 0;
    return 1;
#else
    return 0;
#endif
}

char* PP_get_directive_from_line(char* line) {
    if (!line) return NULL;
    char* p = line;
    while (*p == ' ' || *p == '\t') p++;
    if (*p != '#') return NULL;
    p++;
    while (*p == ' ' || *p == '\t') p++;
    return *p ? p : NULL;
}

int PP_parse_include_arg(const char* p, char* out, size_t out_sz, int* is_system) {
    if (!p || !out || out_sz == 0 || !is_system) return 0;
    while (*p == ' ' || *p == '\t') p++;

    char end = 0;
    if (*p == '"') { *is_system = 0; end = '"'; p++; }
    else if (*p == '<') { *is_system = 1; end = '>'; p++; }
    else return 0;

    size_t i = 0;
    while (*p && *p != end) {
        if (i + 1 >= out_sz) return 0;
        out[i++] = *p++;
    }

    if (*p != end) return 0;
    out[i] = 0;
    return 1;
}

static inline int _is_ident_start(char c) {
    return (c == '_') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static inline int _is_ident_char(char c) {
    return _is_ident_start(c) || (c >= '0' && c <= '9');
}

static inline int _ensure_cap(char** out, size_t* cap, size_t need) {
    if (need <= *cap) return 1;
    size_t nc = (*cap ? *cap : 64);
    while (nc < need) nc *= 2;
    char* nb = (char*)realloc(*out, nc);
    if (!nb) return 0;
    *out = nb;
    *cap = nc;
    return 1;
}

static inline int _append_mem(char** out, size_t* cap, size_t* oi, const char* p, size_t n) {
    if (!_ensure_cap(out, cap, (*oi) + n + 1)) return 0;
    memcpy((*out) + (*oi), p, n);
    *oi += n;
    (*out)[*oi] = 0;
    return 1;
}

static inline int _append_ch(char** out, size_t* cap, size_t* oi, char c) {
    return _append_mem(out, cap, oi, &c, 1);
}

static size_t _literal_end(const char* p, size_t i) {
    char quote = p[i++];
    while (p[i] && p[i] != quote) {
        if (p[i] == '\\' && p[i + 1]) i++;
        i++;
    }

    return p[i] ? i + 1 : i;
}

static int _parse_macro_call(const char* in, size_t* end, int expected, list_t* args) {
    size_t start = *end + 1;
    int depth = 0;
    for (size_t i = start; in[i] && in[i] != '\n' && in[i] != '\r'; i++) {
        if (in[i] == '\'' || in[i] == '"') {
            i = _literal_end(in, i) - 1;
            continue;
        }

        if (in[i] == '(') depth++;
        if (in[i] == ')' && depth) { 
            depth--; 
            continue; 
        }

        if ((in[i] == ',' && !depth) || (in[i] == ')' && !depth)) {
            size_t a = start, b = i;
            while (a < b && isspace((unsigned char)in[a])) a++;
            while (b > a && isspace((unsigned char)in[b - 1])) b--;
            if (a != b || expected || args->s || in[i] == ',') {
                string_t* arg = create_string_from_part(in, a, b - a);
                if (!arg) return 0;
                if (!list_add(args, arg)) { destroy_string(arg); return 0; }
            }

            if (in[i] == ')') {
                *end = i + 1;
                return args->s == expected;
            }
            
            start = i + 1;
        }
    }

    return 0;
}

static const char* _body_marker(const char* p) {
    while (*p) {
        if (*p == '\'' || *p == '"') {
            p += _literal_end(p, 0);
            continue;
        }

        if (!_is_ident_start(*p)) p++;
        else {
            const char* start = p++;
            while (_is_ident_char(*p)) p++;
            if (p - start == 8 && !memcmp(start, "__BODY__", 8)) return start;
        }
    }

    return NULL;
}

static int _parse_macro_body(const char* in, size_t* start, size_t* end) {
    size_t i = *end;
    while (isspace((unsigned char)in[i])) i++;
    if (!in[i]) return 2;
    if (in[i] != '{') return 0;
    *start = i;
    int depth = 0;
    for (; in[i]; i++) {
        if (in[i] == '\'' || in[i] == '"') {
            i = _literal_end(in, i) - 1;
            continue;
        }
        if (in[i] == '{') depth++;
        if (in[i] == '}' && --depth == 0) {
            *end = i + 1;
            return 1;
        }
    }
    return 2;
}

static int _expand_macro(
    define_t* def, list_t* args, const char* block, size_t block_len, char** out, size_t* cap, size_t* oi
) {
    map_t replacements;
    if (!map_init(&replacements, MAP_NO_CMP)) return 0;
    list_node_t* actual = args->h;
    if (def->args) {
        foreach (string_t* name, &def->args->args) {
            list_t* positions;
            if (!map_get(&def->args->positions, (long)name->hash, (void**)&positions)) goto _fail;
            foreach (void* offset, positions) {
                if (!map_put(&replacements, (long)offset, actual->data)) goto _fail;
            }

            actual = actual->n;
        }
    }

    const char* body = def->value->head;
    const char* marker = block ? _body_marker(body) : NULL;
    for (size_t i = 0; body[i];) {
        string_t* arg;
        if (body + i == marker) {
            if (!_append_mem(out, cap, oi, block, block_len)) goto _fail;
            i += 8;
            marker = _body_marker(body + i);
        }
        else if (map_get(&replacements, (long)i, (void**)&arg)) {
            if (!_append_mem(out, cap, oi, arg->head, arg->len(arg))) goto _fail;
            do { i++; } while (_is_ident_char(body[i]));
        }
        else {
            if (!_append_ch(out, cap, oi, body[i++])) goto _fail;
        }
    }

    map_free(&replacements);
    return 1;

_fail: {}
    map_free(&replacements);
    return 0;
}

int PP_register_define(const char* p, deftb_t* macros) {
    char name[PP_PATH_MAX] = { 0 }, body[PP_PATH_MAX] = { 0 };
    if (!PP_parse_define_arg(p, name, sizeof(name), body, sizeof(body))) return 0;
    while (*p == ' ' || *p == '\t') p++;
    p += strlen(name);
    if (*p != '(') return MCTB_put_define(name, body, macros);

    macro_args_t* args = mm_malloc(sizeof(*args));
    if (!args) return 0;
    list_init(&args->args);
    memset(&args->positions, 0, sizeof(args->positions));
    if (!map_init(&args->positions, MAP_NO_CMP)) goto _fail;

    p++;
    while (*p == ' ' || *p == '\t') p++;
    if (*p != ')') {
        for (;;) {
            if (!_is_ident_start(*p)) goto _fail;
            const char* start = p++;
            while (_is_ident_char(*p)) p++;
            string_t* param = create_string_from_part(start, 0, p - start);
            if (!param) goto _fail;
            list_t* positions;
            if (map_get(&args->positions, (long)param->hash, (void**)&positions)) {
                destroy_string(param);
                goto _fail;
            }
            if (!list_add(&args->args, param)) { destroy_string(param); goto _fail; }
            positions = mm_malloc(sizeof(*positions));
            if (!positions) goto _fail;
            list_init(positions);
            if (!map_put(&args->positions, (long)param->hash, positions)) {
                mm_free(positions);
                goto _fail;
            }

            while (*p == ' ' || *p == '\t') p++;
            if (*p == ')') break;
            if (*p++ != ',') goto _fail;
            while (*p == ' ' || *p == '\t') p++;
        }
    }

    for (size_t i = 0; body[i];) {
        if (body[i] == '\'' || body[i] == '"') { 
            i = _literal_end(body, i); 
            continue; 
        }

        if (!_is_ident_start(body[i])) { 
            i++; 
            continue; 
        }

        size_t start = i++;
        while (_is_ident_char(body[i])) i++;
        string_t* token = create_string_from_part(body, start, i - start);
        if (!token) goto _fail;
        
        list_t* positions;
        int found = map_get(&args->positions, (long)token->hash, (void**)&positions);
        destroy_string(token);
        if (found && !list_add(positions, (void*)(long)start)) goto _fail;
    }

    if (MCTB_put_macro(name, body, args, macros)) {
        return 1;
    }

_fail: {}
    MCTB_free_macro_args(args);
    return 0;
}

static int _resolve_defines(const char* in, char** out, size_t* out_cap, deftb_t* dctx) {
    if (!in || !dctx || !out || !out_cap) return -1;

    size_t n = strlen(in);
    if (!*out || *out_cap < n + 1) {
        size_t new_cap = n + 1;
        char* nb = (char*)realloc(*out, new_cap);
        if (!nb) return -1;
        *out = nb;
        *out_cap = new_cap;
    }

    size_t oi = 0;
    (*out)[0] = 0;

    int changed = 0;
    for (size_t i = 0; in[i];) {
        unsigned char c = (unsigned char)in[i];
        if (c == '\'' || c == '"') {
            size_t end = _literal_end(in, i);
            if (!_append_mem(out, out_cap, &oi, in + i, end - i)) return -1;
            i = end;
            continue;
        }
        if (_is_ident_start(c)) {
            size_t start = i++;
            while (in[i] && ((in[i] == '_') || isalnum(in[i]))) i++;
            size_t len = i - start;

            char stack_tok[256] = { 0 };
            char* tok = stack_tok;
            if (len >= sizeof(stack_tok)) {
                tok = (char*)malloc(len + 1);
                if (!tok) return -1;
            }

            memcpy(tok, in + start, len);
            tok[len] = 0;

            define_t def;
            if (MCTB_get_define(tok, &def, dctx)) {
                size_t end = i;
                list_t args;
                list_init(&args);
                if (def.args) {
                    while (in[end] == ' ' || in[end] == '\t') end++;
                    if (in[end] == '(') {
                        int ok = _parse_macro_call(in, &end, def.args->args.s, &args);
                        if (!ok) {
                            list_free_force_op(&args, (int (*)(void*))destroy_string);
                            if (tok != stack_tok) free(tok);
                            return -1;
                        }
                    }
                    else {
                        int ok = _append_mem(out, out_cap, &oi, tok, len);
                        if (tok != stack_tok) free(tok);
                        if (!ok) return -1;
                        continue;
                    }
                }

                int has_body = _body_marker(def.value->head) != NULL;
                size_t block_start = end;
                int ok = has_body ? _parse_macro_body(in, &block_start, &end) : 1;
                if (ok != 1) {
                    list_free_force_op(&args, (int (*)(void*))destroy_string);
                    if (tok != stack_tok) free(tok);
                    return ok == 2 ? -2 : -1;
                }

                if (def.args || has_body) {
                    ok = _expand_macro(&def, &args, has_body ? in + block_start : NULL,
                                       end - block_start, out, out_cap, &oi);
                    list_free_force_op(&args, (int (*)(void*))destroy_string);
                    if (tok != stack_tok) free(tok);
                    if (!ok) return -1;
                    i = end;
                    changed = 1;
                    continue;
                }

                size_t vlen = def.value->len(def.value);
                const char* v = def.value->head;
                if (!(vlen == len && !memcmp(v, tok, len))) {
                    changed = 1;
                }

                if (!_append_mem(out, out_cap, &oi, v, vlen)) {
                    if (tok != stack_tok) free(tok);
                    return -1;
                }
            } 
            else {
                if (!_append_mem(out, out_cap, &oi, tok, len)) {
                    if (tok != stack_tok) free(tok);
                    return -1;
                }
            }

            if (tok != stack_tok) free(tok);
            continue;
        }

        if (!_append_ch(out, out_cap, &oi, (char)c)) return -1;
        i++;
    }

    (*out)[oi] = 0;
    return changed;
}

#define PP_MAX_PASSES 128
int PP_resolve_defines(char** in, size_t* in_cap, char** out, size_t* out_cap, deftb_t* dctx) {
    for (int pass = 0; pass < PP_MAX_PASSES; pass++) {
        int changed = _resolve_defines(*in, out, out_cap, dctx);
        if (changed == -2) return 2;
        if (changed < 0)   return 0;
        if (!changed)      return 1;
        
        char *ts    = *in; 
        *in         = *out; 
        *out        = ts;
        size_t tcap = *in_cap; 
        *in_cap     = *out_cap; 
        *out_cap    = tcap;
    }
    
    return 1;
}
#undef PP_MAX_PASSES

int PP_strip_colon_comments(const char* in, pp_cmt_state_t* st, char** out, size_t* out_cap) {
    if (!in || !st || !out || !out_cap) return 0;
    size_t n = strlen(in);
    if (!*out || *out_cap < n + 1) {
        size_t new_cap = n + 1;
        char* nb = (char*)realloc(*out, new_cap);
        if (!nb) return 0;
        *out = nb;
        *out_cap = new_cap;
    }

    size_t oi = 0;
    for (size_t i = 0; in[i]; i++) {
        char c = in[i];
        if (st->in_colon_slash) {
            if (c == '/' && in[i + 1] == ':') {
                st->in_colon_slash = 0;
                i++;
            }

            continue;
        }

        if (st->in_colon) {
            if (c == ':') st->in_colon = 0;
            continue;
        }
        else if (st->in_str) {
            (*out)[oi++] = c;
            if (st->esc)        st->esc    = 0;
            else if (c == '\\') st->esc    = 1;
            else if (c == '"')  st->in_str = 0;
            continue;
        }
        else if (st->in_chr) {
            (*out)[oi++] = c;
            if (st->esc)        st->esc    = 0;
            else if (c == '\\') st->esc    = 1;
            else if (c == '\'') st->in_chr = 0;
            continue;
        }

        switch (c) {
            case '"': {
                st->in_str = 1;
                (*out)[oi++] = c;
                continue;
            }
            case '\'': {
                st->in_chr = 1;
                (*out)[oi++] = c;
                continue;
            }
            case ':': {
                if (in[i + 1] == ':') {
                    (*out)[oi++] = ':';
                    (*out)[oi++] = ':';
                    i++;
                    continue;
                }

                if (in[i + 1] != '/') st->in_colon = 1;
                else {
                    st->in_colon_slash = 1;
                    i++;
                }

                continue;
            }
            default: break;
        }

        (*out)[oi++] = c;
    }

    (*out)[oi] = 0;
    return 1;
}

int PP_parse_define_arg(const char* p, char* name_out,  size_t name_sz, char* value_out, size_t value_sz) {
    if (!p || !name_out || !name_sz) return 0;
    while (*p == ' ' || *p == '\t') p++;
    if (!_is_ident_start(*p)) return 0;

    size_t i = 0;
    while (*p && _is_ident_char(*p)) {
        if (i + 1 >= name_sz) return 0;
        name_out[i++] = *p++;
    }

    name_out[i] = 0;

    if (*p == '(') {
        int depth = 0;
        while (*p && *p != '\n' && *p != '\r') {
            if (*p == '(') depth++;
            else if (*p == ')') {
                depth--;
                if (depth == 0) { 
                    p++; 
                    break; 
                }
            }

            p++;
        }

        if (depth != 0) return 0;
    }

    while (*p == ' ' || *p == '\t') p++;
    if (!value_out) return 1;

    size_t j = 0;
    while (*p && *p != '\n' && *p != '\r') {
        if (j + 1 >= value_sz) return 0;
        value_out[j++] = *p++;
    }

    value_out[j] = 0;
    while (j > 0 && (value_out[j - 1] == ' ' || value_out[j - 1] == '\t')) {
        value_out[--j] = 0;
    }

    return 1;
}
