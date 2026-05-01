#include <sem/hir/z3.h>

static inline int _argv_add(char** argv, int* argc, const char* arg) {
    if (*argc >= Z3_WRAPPER_MAX_ARGS - 1) return 0;
    argv[*argc] = (char*)arg;
    *argc += 1;
    argv[*argc] = NULL;
    return 1;
}

typedef struct {
    const char*  python;
    const char*  script;
    const char*  input_kind;
    const char*  function;
    const char*  what;
    const char** targets;
    int          target_count;
    const char** sets;
    int          set_count;
    int          json;
    int          no_model;
    int          strict_parser;
    int          strict_z3;
    int          ptr_bits;
    int          max_depth;
    const char*  ty;
    const char*  storage;
} z3_options_t;

int _launch_z3_wrapper(FILE* f, z3_options_t* opt) {
    if (!f || !opt || !opt->what) return 1;
    if (fflush(f)) return 1;
    if (fseek(f, 0, SEEK_SET)) return 1;

    int argc = 0;
    char* argv[Z3_WRAPPER_MAX_ARGS];
    if (!_argv_add(argv, &argc, opt->python ? opt->python : "python3")) return 0;
    if (!_argv_add(argv, &argc, opt->script ? opt->script : Z3_SCRIPT)) return 0;
    if (!_argv_add(argv, &argc, "-")) return 0;
    if (!_argv_add(argv, &argc, "--input-kind")) return 0;
    if (!_argv_add(argv, &argc, opt->input_kind ? opt->input_kind : "dump")) return 0;

    if (opt->function) {
        if (!_argv_add(argv, &argc, "-f")) return 0;
        if (!_argv_add(argv, &argc, opt->function)) return 0;
    }

    for (int i = 0; i < opt->set_count; ++i) {
        if (!_argv_add(argv, &argc, "--set")) return 0;
        if (!_argv_add(argv, &argc, opt->sets[i])) return 0;
    }

    if (opt->ptr_bits > 0) {
        char ptr_bits_buf[32];
        snprintf(ptr_bits_buf, sizeof(ptr_bits_buf), "%d", opt->ptr_bits);
        if (!_argv_add(argv, &argc, "--ptr-bits")) return 0;
        if (!_argv_add(argv, &argc, ptr_bits_buf)) return 0;
    }

    if (opt->max_depth > 0) {
        char max_depth_buf[32];
        snprintf(max_depth_buf, sizeof(max_depth_buf), "%d", opt->max_depth);
        if (!_argv_add(argv, &argc, "--max-depth")) return 0;
        if (!_argv_add(argv, &argc, max_depth_buf)) return 0;
    }

    if (opt->json && !_argv_add(argv, &argc, "--json")) return 0;
    if (opt->no_model && !_argv_add(argv, &argc, "--no-model")) return 0;
    if (opt->strict_parser && !_argv_add(argv, &argc, "--strict-parser")) return 0;
    if (opt->strict_z3 && !_argv_add(argv, &argc, "--strict-z3")) return 0;
    if (opt->ty && (!_argv_add(argv, &argc, "--ty") || !_argv_add(argv, &argc, opt->ty))) return 0;
    if (opt->storage && (!_argv_add(argv, &argc, "--storage") || !_argv_add(argv, &argc, opt->storage))) return 0;
    if (!_argv_add(argv, &argc, opt->what)) return 0;

    for (int i = 0; i < opt->target_count; ++i) {
        if (!_argv_add(argv, &argc, opt->targets[i])) return 0;
    }

    pid_t pid = fork();
    if (pid == -1) return 1;
    if (!pid) {
        if (dup2(fileno(f), STDIN_FILENO) == -1) _exit(126);
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull == -1) _exit(126);
        if (dup2(devnull, STDOUT_FILENO) == -1) _exit(126);
        if (dup2(devnull, STDERR_FILENO) == -1) _exit(126);
        if (devnull > STDERR_FILENO) close(devnull);
        execvp(argv[0], argv);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) == -1) return 1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return 1;
}

int Z3_can_vid_be_equal(symbol_id_t v_id, long long value, string_t* f, FILE* dump) {
    char id_buf[32];
    char value_buf[32];
    snprintf(id_buf, sizeof(id_buf), "%%%lld", (long long)v_id);
    snprintf(value_buf, sizeof(value_buf), "%lld", value);
    const char* targets[] = {
        id_buf,
        value_buf,
    };

    z3_options_t opt = {
        .what = "var-eq",
        .targets = targets,
        .target_count = 2,
        .json = 1,
        .function = f->body
    };

    return _launch_z3_wrapper(dump, &opt);
}

int Z3_can_reach_label(long l_id, string_t* f, FILE* dump) {
    char id_buf[32];
    snprintf(id_buf, sizeof(id_buf), "%lld", (long long)l_id);
    const char* targets[] = { id_buf };
    z3_options_t opt = {
        .what = "label",
        .targets = targets,
        .target_count = 1,
        .json = 1,
        .function = f->body
    };

    return _launch_z3_wrapper(dump, &opt);
}
