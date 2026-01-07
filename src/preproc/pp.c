#include <preproc/pp.h>

typedef struct {
    FILE* f;  /* descriptor          */
    int   l;  /* line                */
    char* n;  /* file name           */
    int   dl; /* Delete scope guards */
} source_pos_info_t;

static inline source_pos_info_t* _create_info(FILE* f, char* name, int l) {
    source_pos_info_t* inf = (source_pos_info_t*)malloc(sizeof(source_pos_info_t));
    if (!inf) return NULL;
    inf->f  = f;
    inf->l  = l;
    inf->n  = strdup(name);
    inf->dl = 0;
    return inf;
}

static int _destroy_info(source_pos_info_t* inf) {
    if (!inf) return 0;
    if (inf->f) fclose(inf->f);
    if (inf->n) free(inf->n);
    free(inf);
    return 1;
}

static int _try_push_path(sstack_t* st, char* full_path) {
    FILE* inc = fopen(full_path, "r");
    if (!inc) return 0;
    
    source_pos_info_t* inf = _create_info(inc, full_path, 0);
    inf->dl = 2; /* If we're in this function, that means, we're
                    pushing a dependency (header), which means,
                    we must delete the guard scopes. */

    if (!stack_push(st, inf)) {
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

/*
Cleanup all mess that we've produced.
Params:
    - `line` - Line variable.
    - `clean` - Clean line (without comments) variable.
    - `st` - Main PP sources stack.
    - `out` - Output tmp file.
    - `fd` - Source FD.
*/
static inline void _perform_exit_cleanup(char* line, char* clean, sstack_t* st, FILE* out, int fd, deftb_t* dctx) {
    FILE* fp;
    while (stack_pop(st, (void**)&fp)) {
        if (fp) fclose(fp);
    }

    MCTB_unload(dctx);
    stack_free_force_op(st, (int (*)(void*))_destroy_info);

    free(line);
    free(clean);
    fflush(out);
    fclose(out);
    close(fd);
}

typedef struct {
    int            skip;       /* Skip current line?      */
    char*          line;       /* Current line            */
    size_t         size;       /* Current line size       */
    pp_cmt_state_t cst;        /* Context for comment rm  */
    char*          clean;      /* Cleaned from cmt line   */
    size_t         clean_size; /* Cleaned line size       */
} pp_ctx_t;

static char* _l  = NULL;
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

int PP_perform(int fd, finder_ctx_t* fctx) {
    deftb_t dctx;
    MCTB_init(&dctx);

    pp_ctx_t ppctx = { 0 };
    int ffd = PP_create_tmp_file(fd);
    if (ffd < 0) return -1;

    FILE* out = fdopen(dup(ffd), "w");
    if (!out) { 
        close(ffd); 
        return -1; 
    }

    sstack_t source_stack;
    stack_init(&source_stack);

    FILE* src = fdopen(dup(fd), "r");
    if (!src) {
_path_calculation_error: {}
        if (src)      fclose(src);
        if (out)      fclose(out);
        if (ffd >= 0) close(ffd);
        stack_free_force_op(&source_stack, (int (*)(void*))_destroy_info);
        MCTB_unload(&dctx);
        return -1;
    }

    char src_path[PP_PATH_MAX] = { 0 };
    if (!PP_file_path_from_fp(src, src_path, sizeof(src_path))) goto _path_calculation_error;

    char* src_path_ptr = strdup(src_path);
    if (!src_path_ptr) goto _path_calculation_error;

    source_pos_info_t* info = _create_info(src, src_path_ptr, 0);
    if (!info || !stack_push(&source_stack, info)) {
        if (info) _destroy_info(info);
        else free(src_path_ptr);
        goto _path_calculation_error;
    }

    source_pos_info_t* inf;
    while (stack_top(&source_stack, (void**)&inf)) {
        ssize_t nread = getline(&ppctx.line, &ppctx.size, inf->f);

        /* Delete the guardian entry scope.
           The gramma accepts code only in scopes,
           but pre-processor must delete these scopes to
           make it works. */
        if (inf->dl == 2) { /* Init state */
            for (int i = 0; i < ppctx.size; i++) {
                if (ppctx.line[i] == '{') {
                    ppctx.line[i] = ' ';   /* Set the empty sybol */
                    inf->dl = 1;           /* Set the next state  */
                }
            }
        }

        inf->l++;
        
        /* Current source file is complete.
           Move to the next one. 
           - Also, we need to place a 'line' command
             to be sure, that tokenizer will figure out
            how to deal with a new complex file. */
        if (nread == -1) {
            source_pos_info_t* done;
            stack_pop(&source_stack, (void**)&done);
            if (done) _destroy_info(done);

            /* Delete the guardian exit scope.
            The gramma accepts code only in scopes,
            but pre-processor must delete these scopes to
            make it works. */
            if (inf->dl == 1 && _l) { /* Init state */
                for (int i = strlen(_l); i >= 0; i--) {
                    if (_l[i] == '}') {
                        _l[i] = ' ';   /* Set the empty sybol */
                        inf->dl = 0;   /* Set the next state  */
                    }
                }
            }

            if (
                stack_top(&source_stack, (void**)&done) && 
                done
            ) {
                char lder[512] = { 0 };
                snprintf(lder, sizeof(lder), "#line %i \"%s\"\n", done->l, done->n);
                _lazy_fputs(lder, out);
            }

            continue;
        }

        /* Remove all comments from the line.
           Also remember the open comment statement if it exists
           in the line.
           This info will be saved in the cst structure. */
        if (!PP_strip_colon_comments(ppctx.line, &ppctx.cst, &ppctx.clean, &ppctx.clean_size)) {
            _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
            return -1;
        }

        /* Replace all defined values by their defenitions. TODO
         */

        /* Figure out which directive is presented in the line.
           If there is no directive, just copy the line into the output. */
        char* d = PP_get_directive_from_line(ppctx.clean);
        if (!d) {
            if (!ppctx.skip) _lazy_fputs(ppctx.clean, out);
        }
        else {
            /* Inlude (#include) directive handler */
            if (IS_PP_DERICTIVE(d, PP_INCLUDE_DIRECTIVE)) {
                int is_system = 0;                      /* WIP: if this flag is 1 -> This is a system lib */
                char inc_name[PP_PATH_MAX] = { 0 };     /* Include path                                   */
                if (!PP_parse_include_arg(PP_MV_LINE_DIR(d, PP_INCLUDE_DIRECTIVE), inc_name, sizeof(inc_name), &is_system)) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                if (!_push_include(inf->f, &source_stack, fctx, inc_name, is_system)) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                /* Put a line that indicates a new start for the
                   new include file. */
                if (
                    stack_top(&source_stack, (void**)&inf) &&
                    inf
                ) {
                    char lder[512] = { 0 };
                    snprintf(lder, sizeof(lder), "#line %i \"%s\"\n", inf->l, inf->n);
                    _lazy_fputs(lder, out);
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_DEFINE_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                char defval[PP_PATH_MAX]  = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), defval, sizeof(defval))) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                if (!MCTB_put_define(defname, defval, &dctx)) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_UNDEF_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                char defval[PP_PATH_MAX]  = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), defval, sizeof(defval))) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                if (!MCTB_remove_define(defname, &dctx)) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }
            }
            else if (IS_PP_DERICTIVE(d, PP_IFDEF_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                char defval[PP_PATH_MAX]  = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), defval, sizeof(defval))) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                if (!MCTB_get_define(defname, NULL, &dctx)) ppctx.skip = 1;
            }
            else if (IS_PP_DERICTIVE(d, PP_IFNDEF_DIRECTIVE)) {
                char defname[PP_PATH_MAX] = { 0 };
                char defval[PP_PATH_MAX]  = { 0 };
                if (!PP_parse_define_arg(PP_MV_LINE_DIR(d, PP_DEFINE_DIRECTIVE), defname, sizeof(defname), defval, sizeof(defval))) {
                    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
                    return -1;
                }

                if (MCTB_get_define(defname, NULL, &dctx)) ppctx.skip = 1;
            }
            else if (IS_PP_DERICTIVE(d, PP_ENDIF_DIRECTIVE)) {
                if (ppctx.skip) ppctx.skip = 0;
            }
        }
    }

    _lazy_fputs(NULL, out);
    _perform_exit_cleanup(ppctx.line, ppctx.clean, &source_stack, out, fd, &dctx);
    lseek(ffd, 0, SEEK_SET);
    return ffd;
}
