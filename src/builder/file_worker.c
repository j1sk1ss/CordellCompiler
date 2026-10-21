#include <builder.h>

static inline int BUILD_readable_directory(const char* path) {
    return path && path[0] && !access(path, R_OK | X_OK);
}

static inline int BUILD_readable_file(const char* path) {
    return path && path[0] && !access(path, R_OK);
}

static int BUILD_path_from_executable(const char* argv0, const char* suffix, char* out, size_t out_size) {
    if (!argv0 || !suffix || !out || !out_size) return 0;
    char executable[PATH_MAX] = { 0 };
#ifdef __linux__
    ssize_t nread = readlink("/proc/self/exe", executable, sizeof(executable) - 1);
    if (nread > 0) executable[nread] = 0;
    else
#endif
    {
        if (!realpath(argv0, executable)) return 0;
    }

    char* slash = strrchr(executable, '/');
    if (!slash) return 0;
    *slash = 0;

    char candidate[PATH_MAX] = { 0 };
    int written = snprintf(candidate, sizeof(candidate), "%s/%s", executable, suffix);
    if (written <= 0 || (size_t)written >= sizeof(candidate)) return 0;

    char resolved[PATH_MAX] = { 0 };
    if (!realpath(candidate, resolved) || !BUILD_readable_directory(resolved)) return 0;
    if (strlen(resolved) + 1 > out_size) return 0;
    strcpy(out, resolved);
    return 1;
}

static int BUILD_file_from_executable(const char* argv0, const char* suffix, char* out, size_t out_size) {
    if (!argv0 || !suffix || !out || !out_size) return 0;
    char executable[PATH_MAX] = { 0 };
#ifdef __linux__
    ssize_t nread = readlink("/proc/self/exe", executable, sizeof(executable) - 1);
    if (nread > 0) executable[nread] = 0;
    else
#endif
    {
        if (!realpath(argv0, executable)) return 0;
    }

    char* slash = strrchr(executable, '/');
    if (!slash) return 0;
    *slash = 0;

    char candidate[PATH_MAX] = { 0 };
    int written = snprintf(candidate, sizeof(candidate), "%s/%s", executable, suffix);
    if (written <= 0 || (size_t)written >= sizeof(candidate)) return 0;

    char resolved[PATH_MAX] = { 0 };
    if (!realpath(candidate, resolved) || !BUILD_readable_file(resolved)) return 0;
    if (strlen(resolved) + 1 > out_size) return 0;
    strcpy(out, resolved);
    return 1;
}

char* BUILD_find_stdlib(const char* argv0, char* out, size_t out_size) {
    char* env = getenv("CPL_INCLUDE_PATH");
    if (BUILD_readable_directory(env))                                            return env;
    if (BUILD_path_from_executable(argv0, "../share/cpl/include", out, out_size)) return out;
    if (BUILD_path_from_executable(argv0, "../../cpllib", out, out_size))         return out;
    if (BUILD_readable_directory(CPL_DEFAULT_INCLUDE_DIR))                        return CPL_DEFAULT_INCLUDE_DIR;
    if (realpath("cpllib", out) && BUILD_readable_directory(out))                 return out;
    return NULL;
}

char* BUILD_find_runtime_library(const char* argv0, char* out, size_t out_size) {
    char* env = getenv("CPL_RUNTIME_LIB");
    if (BUILD_readable_file(env))                                                return env;
    if (BUILD_file_from_executable(argv0, "../lib/cpl/libcpl.a", out, out_size)) return out;
    if (BUILD_file_from_executable(argv0, "cpllib/libcpl.a", out, out_size))     return out;
    if (BUILD_readable_file(CPL_DEFAULT_RUNTIME_LIB))                            return CPL_DEFAULT_RUNTIME_LIB;
    return NULL;
}

char* BUILD_make_temp_path() {
    char template[] = "/tmp/builder-XXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) return NULL;
    close(fd);
    return strdup(template);
}

int BUILD_copy_fd_to_stream(int fd, FILE* stream) {
    char buffer[4096] = { 0 };
    ssize_t nread = 0;
    while ((nread = read(fd, buffer, sizeof(buffer))) > 0) {
        size_t written = fwrite(buffer, 1, (size_t)nread, stream);
        if (written != (size_t)nread) return 0;
    }

    return !nread && !ferror(stream);
}

static int BUILD_copy_file_path(const char* src, const char* dst) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) return 0;

    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        close(src_fd);
        return 0;
    }

    int ok = 1;
    char buffer[4096] = { 0 };
    ssize_t nread = 0;
    while ((nread = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t offset = 0;
        while (offset < nread) {
            ssize_t written = write(dst_fd, buffer + offset, (size_t)(nread - offset));
            if (written < 0) {
                ok = 0;
                break;
            }

            offset += written;
        }

        if (!ok) break;
    }

    if (nread < 0) ok = 0;
    if (close(dst_fd) < 0) ok = 0;
    close(src_fd);
    return ok;
}

int BUILD_move_file_path(const char* src, const char* dst) {
    if (rename(src, dst) == 0) return 1;
    if (
        errno != EXDEV ||
        !BUILD_copy_file_path(src, dst)
    ) return 0;
    return unlink(src) == 0;
}

const char* BUILD_output_path_or_default(const char* path, const char* fallback) {
    return path ? path : fallback;
}


