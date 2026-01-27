#include <preproc/pp.h>

static inline source_pos_info_t* _create_info(FILE* f, char* name, int l) {
    source_pos_info_t* inf = (source_pos_info_t*)malloc(sizeof(source_pos_info_t));
    if (!inf) return NULL;
    memset(&inf->cst, 0, sizeof(pp_cmt_state_t));
    stack_init(&inf->cst.skips);
    inf->f  = f;
    inf->l  = l;
    inf->n  = strdup(name);
    inf->dl = -1;
    return inf;
}

static int _destroy_info(source_pos_info_t* inf) {
    if (!inf) return 0;
    if (inf->f) fclose(inf->f);
    if (inf->n) free(inf->n);
    free(inf);
    stack_free(&inf->cst.skips);
    return 1;
}

/*
Init a PP ontext.
Params:
    - `ctx` - PP context.

Returns 1 if succeeds.
*/
static inline int _init_pp_ctx(pp_ctx_t* ctx) {
    memset(ctx, 0, sizeof(pp_ctx_t));
    return MCTB_init(&ctx->defines) && stack_init(&ctx->sources);
}

/*
Cleanup all mess that we've produced.
Params:
    - `ctx` - PP context.

Returns 1 if succeeds.
*/
static int _unload_pp_ctx(pp_ctx_t* ctx) {
    FILE* fp;
    while (stack_pop(&ctx->sources, (void**)&fp)) {
        if (fp) fclose(fp);
    }

    MCTB_unload(&ctx->defines);
    stack_free_force_op(&ctx->sources, (int (*)(void*))_destroy_info);

    if (ctx->line)    free(ctx->line);
    if (ctx->clean)   free(ctx->clean);
    if (ctx->defined) free(ctx->defined);
    if (ctx->out) {
        fflush(ctx->out);
        fclose(ctx->out);
    }

    if (ctx->fd >= 0) close(ctx->fd);
    return 1;
}

static int _try_push_path(sstack_t* st, char* full_path) {
    FILE* inc = fopen(full_path, "r");
    if (!inc) return 0;
    
    source_pos_info_t* inf = _create_info(inc, full_path, 0);
    if (!inf || !stack_push(st, inf)) {
        _destroy_info(inf);
        fclose(inc);
        return 0;
    }
    
    return 1;
}

static int _push_include(FILE* curr, sstack_t* st, finder_ctx_t* fctx, char* inc_name, int is_system) {
    if (!st || !inc_name || !*inc_name) return 0;
    if (inc_name[0] == '/') {
        return _try_push_path(st, inc_name) ? 1 : 0;
    }

    char full[PATH_MAX] = { 0 };
    if (!is_system) {
        char curr_path[PATH_MAX] = { 0 };
        char dir_buf[PATH_MAX]   = { 0 };
        if (PP_file_path_from_fp(curr, curr_path, sizeof(curr_path))) {
            strncpy(dir_buf, curr_path, sizeof(dir_buf));
            dir_buf[sizeof(dir_buf) - 1] = 0;

            const char* dir = dirname(dir_buf);
            int w = snprintf(full, sizeof(full), "%s/%s", dir, inc_name);
            if (w > 0 && (size_t)w < sizeof(full)) {
                if (_try_push_path(st, full)) return 1;
            }
        }
    }

    if (fctx && fctx->bpath && fctx->bpath[0]) {
        int w = snprintf(full, sizeof(full), "%s/%s", fctx->bpath, inc_name);
        if (w > 0 && (size_t)w < sizeof(full)) {
            if (_try_push_path(st, full)) return 1;
        }
    }

    return 0;
}

static char* _l = NULL;
static void _lazy_fputs(char* l, FILE* out) {
    if (_l) fputs(_l, out);
    if (_l) {
        free(_l);
        _l = NULL;
    }
    
    if (l) {
        _l  = strdup(l);
    }
}

static inline void _put_line_macro(source_pos_info_t* d, FILE* o) {
    static char lder[512] = { 0 };
    snprintf(lder, sizeof(lder), "\n#line %i \"%s\"\n", d->l, d->n);
    _lazy_fputs(lder, o);
}

int PP_perform(int fd, finder_ctx_t* fctx) {
    pp_ctx_t ppctx;
    _init_pp_ctx(&ppctx);

    ppctx.fd = fd;
    int ffd = PP_create_tmp_file(ppctx.fd);
    if (ffd < 0) return -1;

    ppctx.out = fdopen(dup(ffd), "w");
    if (!ppctx.out) { 
        _unload_pp_ctx(&ppctx);
        return -1; 
    }

    FILE* src = fdopen(dup(ppctx.fd), "r");
    if (!src) {
        _unload_pp_ctx(&ppctx);
        return -1;
    }

    char src_path[PP_PATH_MAX] = { 0 };
    if (!PP_file_path_from_fp(src, src_path, sizeof(src_path))) {
        _unload_pp_ctx(&ppctx);
        return -1;
    }

    char* src_path_ptr = strdup(src_path);
    if (!src_path_ptr) {
        _unload_pp_ctx(&ppctx);
        return -1;
    }

    source_pos_info_t* info = _create_info(src, src_path_ptr, 0);
    if (!info || !stack_push(&ppctx.sources, info)) {
        if (info) _destroy_info(info);
        else free(src_path_ptr);
        _unload_pp_ctx(&ppctx);
        return -1;
    }

    info->dl = SCOPE_GUARDER_INIT;

    source_pos_info_t* inf;
    while (stack_top(&ppctx.sources, (void**)&inf)) {
        ssize_t nread = getline(&ppctx.line, &ppctx.size, inf->f);

        /* Delete the guardian entry scope.
           The gramma accepts code only in scopes,
           but pre-processor must delete these scopes to
           make it works. */
        if (_l && inf->dl != SCOPE_GUARDER_INIT) for (ssize_t i = strlen(_l); i >= 0; i--) {
            switch (_l[i]) {
                case '{': inf->dl++; break;
                case '}': inf->dl--; break;
                default: continue;
            }

            if (inf->dl <= 0) {
                _l[i] = ' ';
            }
        }
        
        inf->l++;
        
        /* Current source file is complete.
           Move to the next one. 
           - Also, we need to place a 'line' command
             to be sure, that tokenizer will figure out
            how to deal with a new complex file. */
        if (nread < 0) {
            source_pos_info_t* done;
            stack_pop(&ppctx.sources, (void**)&done);
            if (done) _destroy_info(done);

            /* Just to be sure, that we have another one file to continue.
               If we have, we must mark the return with an information line. */
            if (stack_top(&ppctx.sources, (void**)&done) && done) {
                _put_line_macro(done, ppctx.out);
            }

            continue;
        }

        /* Remove all comments from the line.
           Also remember the open comment statement if it exists
           in the line.
           This info will be saved in the cst structure. */
        if (!PP_strip_colon_comments(ppctx.line, &inf->cst, &ppctx.clean, &ppctx.clean_size)) {
            _unload_pp_ctx(&ppctx);
            return -1;
        }

        /* Figure out which directive is presented in the line.
           If there is no directive, just copy the line into the output. */
        char* d = PP_get_directive_from_line(ppctx.clean);
        if (!d) {
            long stat;
            if (!stack_top(&inf->cst.skips, (void**)&stat) || !stat) {
                /* Replace all defined values by their defenitions.
                */
                if (!PP_resolve_defines(&ppctx.clean, &ppctx.clean_size, &ppctx.defined, &ppctx.defined_size, &ppctx.defines)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                _lazy_fputs(ppctx.defined, ppctx.out);
            }
        }
        else {
            long skip = 0;
            stack_top(&inf->cst.skips, (void**)&skip);
            if (IS_PP_DERICTIVE(d, PP_INCLUDE_DIRECTIVE) && !skip) {
                int is_system = 0;                      /* WIP: if this flag is 1 -> This is a system lib */
                char inc_name[PP_PATH_MAX] = { 0 };     /* Include path                                   */
                if (!PP_parse_include_arg(PP_MV_LINE_DIR(d, PP_INCLUDE_DIRECTIVE), inc_name, sizeof(inc_name), &is_system)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                if (!_push_include(inf->f, &ppctx.sources, fctx, inc_name, is_system)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                /* Put a line that indicates a new start for the
                   new include file. */
                if (stack_top(&ppctx.sources, (void**)&inf) && inf) {
                    _put_line_macro(inf, ppctx.out);
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_DEFINE_DIRECTIVE) && !skip) {
                char defname[PP_PATH_MAX] = { 0 };
                char defval[PP_PATH_MAX]  = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), defval, sizeof(defval))) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                if (!MCTB_put_define(defname, defval, &ppctx.defines)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_UNDEF_DIRECTIVE) && !skip) {
                char defname[PP_PATH_MAX] = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), NULL, 0)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                if (!MCTB_remove_define(defname, &ppctx.defines)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_IFDEF_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), NULL, 0)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                stack_push(&inf->cst.skips, (void*)((long)!MCTB_get_define(defname, NULL, &ppctx.defines)));
            }
            else if (IS_PP_DERICTIVE(d, PP_IFNDEF_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), NULL, 0)) {
                    _unload_pp_ctx(&ppctx);
                    return -1;
                }

                stack_push(&inf->cst.skips, (void*)((long)MCTB_get_define(defname, NULL, &ppctx.defines)));
            }
            else if (IS_PP_DERICTIVE(d, PP_ENDIF_DIRECTIVE)) {
                stack_pop(&inf->cst.skips, NULL);
            }
        }
    }

    _lazy_fputs(NULL, ppctx.out);
    _unload_pp_ctx(&ppctx);
    lseek(ffd, 0, SEEK_SET);
    return ffd;
}
