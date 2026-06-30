#include <sem/hir/hir_visitors.h>

static inline func_info_t _get_finfo_from_call(hir_block_t* b, sym_table_t* smt) {
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) {
        return (func_info_t){ .id = NO_SYMBOL_ID };
    }

    func_info_t fi;
    if (!FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)) {
        return (func_info_t){ .id = NO_SYMBOL_ID };
    }

    return fi;
}

typedef struct {
    char*  fname;
    size_t min_argc;
    size_t arg_for_check;
    int    val;
    char*  error;
} glibc_arg_check_t;

static glibc_arg_check_t _glibc_functions[] = {
    { "malloc", 1, 0, 0, "The 'malloc' function expects non-zero argument 'size', but the provided value is 0!" },
    { "calloc", 2, 0, 0, "The 'calloc' function expects non-zero argument 'nmemb', but the provided value is 0!" },
    { "calloc", 2, 1, 0, "The 'calloc' function expects non-zero argument 'size', but the provided value is 0!" },
    { "realloc", 2, 1, 0, "The 'realloc' function expects non-zero argument 'size', but the provided value is 0!" },
    { "reallocarray", 3, 1, 0, "The 'reallocarray' function expects non-zero argument 'nmemb', but the provided value is 0!" },
    { "reallocarray", 3, 2, 0, "The 'reallocarray' function expects non-zero argument 'size', but the provided value is 0!" },
    { "posix_memalign", 3, 0, 0, "The 'posix_memalign' function expects non-NULL argument 'memptr', but the provided value is NULL!" },
    { "posix_memalign", 3, 1, 0, "The 'posix_memalign' function expects non-zero argument 'alignment', but the provided value is 0!" },
    { "aligned_alloc", 2, 0, 0, "The 'aligned_alloc' function expects non-zero argument 'alignment', but the provided value is 0!" },
    { "aligned_alloc", 2, 1, 0, "The 'aligned_alloc' function expects non-zero argument 'size', but the provided value is 0!" },
    { "memcpy", 3, 0, 0, "The 'memcpy' function expects non-NULL argument 'dest', but the provided value is NULL!" },
    { "memcpy", 3, 1, 0, "The 'memcpy' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "memmove", 3, 0, 0, "The 'memmove' function expects non-NULL argument 'dest', but the provided value is NULL!" },
    { "memmove", 3, 1, 0, "The 'memmove' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "memset", 3, 0, 0, "The 'memset' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "memcmp", 3, 0, 0, "The 'memcmp' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "memcmp", 3, 1, 0, "The 'memcmp' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "memchr", 3, 0, 0, "The 'memchr' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strlen", 1, 0, 0, "The 'strlen' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strnlen", 2, 0, 0, "The 'strnlen' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strcpy", 2, 0, 0, "The 'strcpy' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "strcpy", 2, 1, 0, "The 'strcpy' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "stpcpy", 2, 0, 0, "The 'stpcpy' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "stpcpy", 2, 1, 0, "The 'stpcpy' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "strncpy", 3, 0, 0, "The 'strncpy' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "strncpy", 3, 1, 0, "The 'strncpy' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "stpncpy", 3, 0, 0, "The 'stpncpy' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "stpncpy", 3, 1, 0, "The 'stpncpy' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "strcat", 2, 0, 0, "The 'strcat' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "strcat", 2, 1, 0, "The 'strcat' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "strncat", 3, 0, 0, "The 'strncat' function expects non-NULL argument 'dst', but the provided value is NULL!" },
    { "strncat", 3, 1, 0, "The 'strncat' function expects non-NULL argument 'src', but the provided value is NULL!" },
    { "strcmp", 2, 0, 0, "The 'strcmp' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "strcmp", 2, 1, 0, "The 'strcmp' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "strncmp", 3, 0, 0, "The 'strncmp' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "strncmp", 3, 1, 0, "The 'strncmp' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "strcasecmp", 2, 0, 0, "The 'strcasecmp' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "strcasecmp", 2, 1, 0, "The 'strcasecmp' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "strncasecmp", 3, 0, 0, "The 'strncasecmp' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "strncasecmp", 3, 1, 0, "The 'strncasecmp' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "strcoll", 2, 0, 0, "The 'strcoll' function expects non-NULL argument 's1', but the provided value is NULL!" },
    { "strcoll", 2, 1, 0, "The 'strcoll' function expects non-NULL argument 's2', but the provided value is NULL!" },
    { "strchr", 2, 0, 0, "The 'strchr' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strrchr", 2, 0, 0, "The 'strrchr' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strstr", 2, 0, 0, "The 'strstr' function expects non-NULL argument 'haystack', but the provided value is NULL!" },
    { "strstr", 2, 1, 0, "The 'strstr' function expects non-NULL argument 'needle', but the provided value is NULL!" },
    { "strpbrk", 2, 0, 0, "The 'strpbrk' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strpbrk", 2, 1, 0, "The 'strpbrk' function expects non-NULL argument 'accept', but the provided value is NULL!" },
    { "strspn", 2, 0, 0, "The 'strspn' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strspn", 2, 1, 0, "The 'strspn' function expects non-NULL argument 'accept', but the provided value is NULL!" },
    { "strcspn", 2, 0, 0, "The 'strcspn' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strcspn", 2, 1, 0, "The 'strcspn' function expects non-NULL argument 'reject', but the provided value is NULL!" },
    { "strdup", 1, 0, 0, "The 'strdup' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strndup", 2, 0, 0, "The 'strndup' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strtok", 2, 1, 0, "The 'strtok' function expects non-NULL argument 'delim', but the provided value is NULL!" },
    { "strtok_r", 3, 1, 0, "The 'strtok_r' function expects non-NULL argument 'delim', but the provided value is NULL!" },
    { "strtok_r", 3, 2, 0, "The 'strtok_r' function expects non-NULL argument 'saveptr', but the provided value is NULL!" },
    { "strsep", 2, 0, 0, "The 'strsep' function expects non-NULL argument 'stringp', but the provided value is NULL!" },
    { "strsep", 2, 1, 0, "The 'strsep' function expects non-NULL argument 'delim', but the provided value is NULL!" },
    { "fopen", 2, 0, 0, "The 'fopen' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "fopen", 2, 1, 0, "The 'fopen' function expects non-NULL argument 'mode', but the provided value is NULL!" },
    { "freopen", 3, 1, 0, "The 'freopen' function expects non-NULL argument 'mode', but the provided value is NULL!" },
    { "freopen", 3, 2, 0, "The 'freopen' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fdopen", 2, 1, 0, "The 'fdopen' function expects non-NULL argument 'mode', but the provided value is NULL!" },
    { "popen", 2, 0, 0, "The 'popen' function expects non-NULL argument 'command', but the provided value is NULL!" },
    { "popen", 2, 1, 0, "The 'popen' function expects non-NULL argument 'type', but the provided value is NULL!" },
    { "fclose", 1, 0, 0, "The 'fclose' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "printf", 1, 0, 0, "The 'printf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "fprintf", 2, 0, 0, "The 'fprintf' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fprintf", 2, 1, 0, "The 'fprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "sprintf", 2, 0, 0, "The 'sprintf' function expects non-NULL argument 'str', but the provided value is NULL!" },
    { "sprintf", 2, 1, 0, "The 'sprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "snprintf", 3, 2, 0, "The 'snprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "scanf", 1, 0, 0, "The 'scanf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "fscanf", 2, 0, 0, "The 'fscanf' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fscanf", 2, 1, 0, "The 'fscanf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "sscanf", 2, 0, 0, "The 'sscanf' function expects non-NULL argument 'str', but the provided value is NULL!" },
    { "sscanf", 2, 1, 0, "The 'sscanf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "vprintf", 2, 0, 0, "The 'vprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "vprintf", 2, 1, 0, "The 'vprintf' function expects non-NULL argument 'ap', but the provided value is NULL!" },
    { "vfprintf", 3, 0, 0, "The 'vfprintf' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "vfprintf", 3, 1, 0, "The 'vfprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "vfprintf", 3, 2, 0, "The 'vfprintf' function expects non-NULL argument 'ap', but the provided value is NULL!" },
    { "vsprintf", 3, 0, 0, "The 'vsprintf' function expects non-NULL argument 'str', but the provided value is NULL!" },
    { "vsprintf", 3, 1, 0, "The 'vsprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "vsprintf", 3, 2, 0, "The 'vsprintf' function expects non-NULL argument 'ap', but the provided value is NULL!" },
    { "vsnprintf", 4, 2, 0, "The 'vsnprintf' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "vsnprintf", 4, 3, 0, "The 'vsnprintf' function expects non-NULL argument 'ap', but the provided value is NULL!" },
    { "fgets", 3, 0, 0, "The 'fgets' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "fgets", 3, 1, 0, "The 'fgets' function expects non-zero argument 'size', but the provided value is 0!" },
    { "fgets", 3, 2, 0, "The 'fgets' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fputs", 2, 0, 0, "The 'fputs' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "fputs", 2, 1, 0, "The 'fputs' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "puts", 1, 0, 0, "The 'puts' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "fread", 4, 0, 0, "The 'fread' function expects non-NULL argument 'ptr', but the provided value is NULL!" },
    { "fread", 4, 3, 0, "The 'fread' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fwrite", 4, 0, 0, "The 'fwrite' function expects non-NULL argument 'ptr', but the provided value is NULL!" },
    { "fwrite", 4, 3, 0, "The 'fwrite' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "fseek", 3, 0, 0, "The 'fseek' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "ftell", 1, 0, 0, "The 'ftell' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "rewind", 1, 0, 0, "The 'rewind' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "feof", 1, 0, 0, "The 'feof' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "ferror", 1, 0, 0, "The 'ferror' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "clearerr", 1, 0, 0, "The 'clearerr' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "getline", 3, 0, 0, "The 'getline' function expects non-NULL argument 'lineptr', but the provided value is NULL!" },
    { "getline", 3, 1, 0, "The 'getline' function expects non-NULL argument 'n', but the provided value is NULL!" },
    { "getline", 3, 2, 0, "The 'getline' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "getdelim", 4, 0, 0, "The 'getdelim' function expects non-NULL argument 'lineptr', but the provided value is NULL!" },
    { "getdelim", 4, 1, 0, "The 'getdelim' function expects non-NULL argument 'n', but the provided value is NULL!" },
    { "getdelim", 4, 3, 0, "The 'getdelim' function expects non-NULL argument 'stream', but the provided value is NULL!" },
    { "remove", 1, 0, 0, "The 'remove' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "rename", 2, 0, 0, "The 'rename' function expects non-NULL argument 'oldpath', but the provided value is NULL!" },
    { "rename", 2, 1, 0, "The 'rename' function expects non-NULL argument 'newpath', but the provided value is NULL!" },
    { "atoi", 1, 0, 0, "The 'atoi' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "atol", 1, 0, 0, "The 'atol' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "atoll", 1, 0, 0, "The 'atoll' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "atof", 1, 0, 0, "The 'atof' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtol", 3, 0, 0, "The 'strtol' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtoul", 3, 0, 0, "The 'strtoul' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtoll", 3, 0, 0, "The 'strtoll' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtoull", 3, 0, 0, "The 'strtoull' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtof", 2, 0, 0, "The 'strtof' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtod", 2, 0, 0, "The 'strtod' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "strtold", 2, 0, 0, "The 'strtold' function expects non-NULL argument 'nptr', but the provided value is NULL!" },
    { "getenv", 1, 0, 0, "The 'getenv' function expects non-NULL argument 'name', but the provided value is NULL!" },
    { "setenv", 3, 0, 0, "The 'setenv' function expects non-NULL argument 'name', but the provided value is NULL!" },
    { "setenv", 3, 1, 0, "The 'setenv' function expects non-NULL argument 'value', but the provided value is NULL!" },
    { "unsetenv", 1, 0, 0, "The 'unsetenv' function expects non-NULL argument 'name', but the provided value is NULL!" },
    { "putenv", 1, 0, 0, "The 'putenv' function expects non-NULL argument 'string', but the provided value is NULL!" },
    { "realpath", 2, 0, 0, "The 'realpath' function expects non-NULL argument 'path', but the provided value is NULL!" },
    { "mkstemp", 1, 0, 0, "The 'mkstemp' function expects non-NULL argument 'template', but the provided value is NULL!" },
    { "mkdtemp", 1, 0, 0, "The 'mkdtemp' function expects non-NULL argument 'template', but the provided value is NULL!" },
    { "qsort", 4, 3, 0, "The 'qsort' function expects non-NULL argument 'compar', but the provided value is NULL!" },
    { "bsearch", 5, 0, 0, "The 'bsearch' function expects non-NULL argument 'key', but the provided value is NULL!" },
    { "bsearch", 5, 4, 0, "The 'bsearch' function expects non-NULL argument 'compar', but the provided value is NULL!" },
    { "atexit", 1, 0, 0, "The 'atexit' function expects non-NULL argument 'function', but the provided value is NULL!" },
    { "on_exit", 2, 0, 0, "The 'on_exit' function expects non-NULL argument 'function', but the provided value is NULL!" },
    { "localtime", 1, 0, 0, "The 'localtime' function expects non-NULL argument 'timep', but the provided value is NULL!" },
    { "gmtime", 1, 0, 0, "The 'gmtime' function expects non-NULL argument 'timep', but the provided value is NULL!" },
    { "ctime", 1, 0, 0, "The 'ctime' function expects non-NULL argument 'timep', but the provided value is NULL!" },
    { "asctime", 1, 0, 0, "The 'asctime' function expects non-NULL argument 'tm', but the provided value is NULL!" },
    { "mktime", 1, 0, 0, "The 'mktime' function expects non-NULL argument 'tm', but the provided value is NULL!" },
    { "strftime", 4, 0, 0, "The 'strftime' function expects non-NULL argument 's', but the provided value is NULL!" },
    { "strftime", 4, 1, 0, "The 'strftime' function expects non-zero argument 'max', but the provided value is 0!" },
    { "strftime", 4, 2, 0, "The 'strftime' function expects non-NULL argument 'format', but the provided value is NULL!" },
    { "strftime", 4, 3, 0, "The 'strftime' function expects non-NULL argument 'tm', but the provided value is NULL!" },
    { "open", 2, 0, 0, "The 'open' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "creat", 2, 0, 0, "The 'creat' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "read", 3, 1, 0, "The 'read' function expects non-NULL argument 'buf', but the provided value is NULL!" },
    { "write", 3, 1, 0, "The 'write' function expects non-NULL argument 'buf', but the provided value is NULL!" },
    { "pread", 4, 1, 0, "The 'pread' function expects non-NULL argument 'buf', but the provided value is NULL!" },
    { "pwrite", 4, 1, 0, "The 'pwrite' function expects non-NULL argument 'buf', but the provided value is NULL!" },
    { "access", 2, 0, 0, "The 'access' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "stat", 2, 0, 0, "The 'stat' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "stat", 2, 1, 0, "The 'stat' function expects non-NULL argument 'statbuf', but the provided value is NULL!" },
    { "lstat", 2, 0, 0, "The 'lstat' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "lstat", 2, 1, 0, "The 'lstat' function expects non-NULL argument 'statbuf', but the provided value is NULL!" },
    { "fstat", 2, 1, 0, "The 'fstat' function expects non-NULL argument 'statbuf', but the provided value is NULL!" },
    { "mkdir", 2, 0, 0, "The 'mkdir' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "rmdir", 1, 0, 0, "The 'rmdir' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "unlink", 1, 0, 0, "The 'unlink' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "chdir", 1, 0, 0, "The 'chdir' function expects non-NULL argument 'path', but the provided value is NULL!" },
    { "getcwd", 2, 1, 0, "The 'getcwd' function expects non-zero argument 'size', but the provided value is 0!" },
    { "opendir", 1, 0, 0, "The 'opendir' function expects non-NULL argument 'name', but the provided value is NULL!" },
    { "readdir", 1, 0, 0, "The 'readdir' function expects non-NULL argument 'dirp', but the provided value is NULL!" },
    { "closedir", 1, 0, 0, "The 'closedir' function expects non-NULL argument 'dirp', but the provided value is NULL!" },
    { "symlink", 2, 0, 0, "The 'symlink' function expects non-NULL argument 'target', but the provided value is NULL!" },
    { "symlink", 2, 1, 0, "The 'symlink' function expects non-NULL argument 'linkpath', but the provided value is NULL!" },
    { "readlink", 3, 0, 0, "The 'readlink' function expects non-NULL argument 'pathname', but the provided value is NULL!" },
    { "readlink", 3, 1, 0, "The 'readlink' function expects non-NULL argument 'buf', but the provided value is NULL!" },
    { "readlink", 3, 2, 0, "The 'readlink' function expects non-zero argument 'bufsiz', but the provided value is 0!" },
};

static inline int _glibc_get_checked_arg(
    hir_block_t* b, const glibc_arg_check_t* check, hir_subject_t*** flattened_args, hir_subject_t** curr_arg
) {
    size_t argc = list_size(&b->targ->storage.list.h);
    if (argc < check->min_argc || argc <= check->arg_for_check) {
        *curr_arg = NULL;
        return 0;
    }

    if (*flattened_args == NULL) {
        *flattened_args = (hir_subject_t**)list_flatten(&b->targ->storage.list.h);
    }

    *curr_arg = (*flattened_args)[check->arg_for_check];
    return 1;
}

int HIRWLKR_glibc_checkers(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    func_info_t fi = _get_finfo_from_call(b, smt);
    if (fi.id == NO_SYMBOL_ID) return 1;
    
    hir_subject_t** args = NULL;
    size_t checks_count = sizeof(_glibc_functions) / sizeof(_glibc_functions[0]);

    for (size_t i = 0; i < checks_count; ++i) {
        glibc_arg_check_t* check = &_glibc_functions[i];
        if (!fi.name->requals(fi.name, check->fname)) {
            continue;
        }

        hir_subject_t* curr_arg = NULL;
        if (!_glibc_get_checked_arg(b, check, &args, &curr_arg)) {
            continue;
        }

        if (
            curr_arg &&
            !HIR_SEM_check_subject_value_and_provide_trace(b, bb, curr_arg, smt, ctx, check->val, check->error)
        ) return 0;
    }

    return 1;
}
