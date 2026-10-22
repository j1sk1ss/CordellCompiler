#include <builder.h>

static inline int _push_cmd_arg(list_t* cmd, const char* arg) {
    return list_push_back(cmd, (void*)arg);
}

static inline char** _make_argv(list_t* args) {
    if (!args) return NULL;
    if (!list_push_back(args, NULL)) return NULL;
    char** argv = (char**)list_flatten(args);
    list_remove(args, NULL);
    return argv;
}

static int _run_tool(const char* tool, list_t* args) {
    char** argv = _make_argv(args);
    if (!argv) return 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        mm_free(argv);
        return 0;
    }

    if (!pid) {
        execvp(tool, argv);
        perror(tool);
        _exit(127);
    }

    int status = 0;
    pid_t waited = 0;
    do {
        waited = waitpid(pid, &status, 0);
    } while (waited < 0 && errno == EINTR);

    if (waited < 0) {
        perror("waitpid");
        mm_free(argv);
        return 0;
    }

    mm_free(argv);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int BUILD_compile_asm_to_object(const options_t* options, const char* asm_path, const char* obj_path) {
    list_t cmd;
    list_init(&cmd);

    int ok =
        _push_cmd_arg(&cmd, options->tools.asm_compiler) &&
        _push_cmd_arg(&cmd, "-f")                        &&
        _push_cmd_arg(&cmd, options->tools.asm_format)   &&
        _push_cmd_arg(&cmd, asm_path)                    &&
        _push_cmd_arg(&cmd, "-o")                        &&
        _push_cmd_arg(&cmd, obj_path);

    if (ok) ok = _run_tool(options->tools.asm_compiler, &cmd);
    list_free(&cmd);
    return ok;
}

int BUILD_link_objects(const options_t* options, list_t* objects) {
    list_t cmd;
    list_init(&cmd);

    if (!_push_cmd_arg(&cmd, options->tools.linker)) goto _fail;
    if (options->tools.linker_use_c_driver) {
        if (options->tools.linker_no_pie && !_push_cmd_arg(&cmd, "-no-pie")) goto _fail;
        if (options->tools.linker_m32    && !_push_cmd_arg(&cmd, "-m32"))    goto _fail;
    }

    if (
        !_push_cmd_arg(&cmd, "-o") ||
        !_push_cmd_arg(&cmd, options->locations.output ? options->locations.output : "a.out")
    ) goto _fail;

    foreach (char* object, objects) {
        if (!_push_cmd_arg(&cmd, object)) goto _fail;
    }

    if (
        options->locations.runtime && 
        !_push_cmd_arg(&cmd, options->locations.runtime)
    ) goto _fail;

    foreach (char* linker_arg, (list_t*)&options->tools.linker_args) {
        if (!_push_cmd_arg(&cmd, linker_arg)) goto _fail;
    }

    int ok = _run_tool(options->tools.linker, &cmd);
    list_free(&cmd);
    return ok;

_fail: {}
    list_free(&cmd);
    return 0;
}
