import argparse
import os
import platform
import shutil
import subprocess
import sys
import tempfile
import unittest

from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]


FAULT_PRELOAD_SOURCE = r"""
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int copy_src_fd = -1;
static int copy_dst_fd = -1;
static FILE* fwrite_fail_stream = NULL;
static int flushing_exit = 0;

static int env_is(const char* name) {
    const char* value = getenv("CPLC_FAULT");
    return value && !strcmp(value, name);
}

static int contains(const char* text, const char* needle) {
    return text && needle && needle[0] && strstr(text, needle);
}

static void* sym(const char* name) {
    return dlsym(RTLD_NEXT, name);
}

static void flush_signal_handler(int signum) {
    signal(signum, SIG_DFL);
    exit(128 + signum);
}

__attribute__((constructor))
static void setup_fault_preload(void) {
    if (getenv("CPLC_FLUSH_SEGV")) {
        signal(SIGSEGV, flush_signal_handler);
    }
}

int access(const char* pathname, int mode) {
    int (*real_access)(const char*, int) = sym("access");
    if (getenv("CPLC_SKIP_DEFAULT_PATHS") &&
        (contains(pathname, "/usr/local/share/cpl/include") ||
         contains(pathname, "/usr/local/lib/cpl/libcpl.a"))) {
        errno = ENOENT;
        return -1;
    }
    return real_access(pathname, mode);
}

ssize_t readlink(const char* pathname, char* buf, size_t bufsiz) {
    ssize_t (*real_readlink)(const char*, char*, size_t) = sym("readlink");
    if (env_is("readlink") && contains(pathname, "/proc/self/exe")) {
        errno = EINVAL;
        return -1;
    }
    return real_readlink(pathname, buf, bufsiz);
}

off_t lseek(int fd, off_t offset, int whence) {
    off_t (*real_lseek)(int, off_t, int) = sym("lseek");
    if (env_is("lseek-token-size") && whence == SEEK_END) {
        return 67108865;
    }
    return real_lseek(fd, offset, whence);
}

int mkstemp(char* template) {
    int (*real_mkstemp)(char*) = sym("mkstemp");
    if (env_is("mkstemp") && contains(template, "/tmp/builder-")) {
        errno = EIO;
        return -1;
    }
    return real_mkstemp(template);
}

pid_t fork(void) {
    pid_t (*real_fork)(void) = sym("fork");
    if (env_is("fork")) {
        errno = EAGAIN;
        return -1;
    }
    return real_fork();
}

pid_t waitpid(pid_t pid, int* status, int options) {
    pid_t (*real_waitpid)(pid_t, int*, int) = sym("waitpid");
    if (env_is("waitpid")) {
        errno = ECHILD;
        return -1;
    }
    return real_waitpid(pid, status, options);
}

FILE* fopen(const char* pathname, const char* mode) {
    FILE* (*real_fopen)(const char*, const char*) = sym("fopen");
    const char* needle = getenv("CPLC_FAULT_FOPEN_CONTAINS");
    if (contains(pathname, needle) ||
        (env_is("fopen-builder-temp") && contains(pathname, "/tmp/builder-") && mode && mode[0] == 'w')) {
        errno = EACCES;
        return NULL;
    }
    FILE* stream = real_fopen(pathname, mode);
    if (stream && contains(pathname, getenv("CPLC_FAULT_FWRITE_CONTAINS"))) {
        fwrite_fail_stream = stream;
    }
    return stream;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t (*real_fwrite)(const void*, size_t, size_t, FILE*) = sym("fwrite");
    if (env_is("fwrite") && (!getenv("CPLC_FAULT_FWRITE_CONTAINS") || stream == fwrite_fail_stream)) return 0;
    return real_fwrite(ptr, size, nmemb, stream);
}

int fclose(FILE* stream) {
    int (*real_fclose)(FILE*) = sym("fclose");
    if (stream == fwrite_fail_stream) fwrite_fail_stream = NULL;
    return real_fclose(stream);
}

void _exit(int status) {
    void (*real__exit)(int) = sym("_exit");
    if (getenv("CPLC_FLUSH_EXIT") && !flushing_exit) {
        flushing_exit = 1;
        exit(status);
    }
    real__exit(status);
    __builtin_unreachable();
}

int rename(const char* oldpath, const char* newpath) {
    int (*real_rename)(const char*, const char*) = sym("rename");
    if (env_is("rename-eacces")) {
        errno = EACCES;
        return -1;
    }
    if (getenv("CPLC_FORCE_RENAME_EXDEV") || env_is("rename-exdev")) {
        errno = EXDEV;
        return -1;
    }
    return real_rename(oldpath, newpath);
}

int open(const char* pathname, int flags, ...) {
    int (*real_open)(const char*, int, ...) = sym("open");
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = (mode_t)va_arg(args, int);
        va_end(args);
    }

    if (env_is("copy-open-src") && contains(pathname, "/tmp/builder-") && ((flags & O_ACCMODE) == O_RDONLY)) {
        errno = EACCES;
        return -1;
    }

    const char* dst_needle = getenv("CPLC_FAULT_COPY_DST_CONTAINS");
    if (env_is("copy-open-dst") && contains(pathname, dst_needle)) {
        errno = EACCES;
        return -1;
    }

    int fd = (flags & O_CREAT) ? real_open(pathname, flags, mode) : real_open(pathname, flags);
    if (fd >= 0 && contains(pathname, "/tmp/builder-") && ((flags & O_ACCMODE) == O_RDONLY)) copy_src_fd = fd;
    if (fd >= 0 && contains(pathname, dst_needle) && ((flags & O_ACCMODE) == O_WRONLY)) copy_dst_fd = fd;
    return fd;
}

ssize_t read(int fd, void* buf, size_t count) {
    ssize_t (*real_read)(int, void*, size_t) = sym("read");
    if (env_is("copy-read") && fd == copy_src_fd) {
        errno = EIO;
        return -1;
    }
    return real_read(fd, buf, count);
}

ssize_t write(int fd, const void* buf, size_t count) {
    ssize_t (*real_write)(int, const void*, size_t) = sym("write");
    if (env_is("copy-write") && fd == copy_dst_fd) {
        errno = ENOSPC;
        return -1;
    }
    return real_write(fd, buf, count);
}

int close(int fd) {
    int (*real_close)(int) = sym("close");
    if (env_is("copy-close") && fd == copy_dst_fd) {
        copy_dst_fd = -1;
        errno = EIO;
        return -1;
    }
    if (fd == copy_src_fd) copy_src_fd = -1;
    if (fd == copy_dst_fd) copy_dst_fd = -1;
    return real_close(fd);
}

int unlink(const char* pathname) {
    int (*real_unlink)(const char*) = sym("unlink");
    if (env_is("unlink-builder-temp") && contains(pathname, "/tmp/builder-")) {
        errno = EIO;
        return -1;
    }
    return real_unlink(pathname);
}
"""


def _default_platform_name() -> str:
    return f"{platform.system().lower()}-{platform.machine().lower()}"


def _default_cplc_path() -> Path:
    env_binary = os.environ.get("CPLC_BINARY")
    if env_binary:
        return Path(env_binary)

    return REPO_ROOT / "builds" / _default_platform_name() / "cplc"


def _tmpdir() -> tempfile.TemporaryDirectory:
    base = Path(os.environ.get("CPLC_TEST_TMPDIR", "/tmp"))
    return tempfile.TemporaryDirectory(prefix="cplc-cli-", dir=base)


def _minimal_source(path: Path) -> None:
    path.write_text("start() {\n}\n", encoding="utf-8")


def _copy_binary(src: Path, dst: Path) -> Path:
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst)
    dst.chmod(0o755)
    return dst


def _linker_passthrough_arg() -> str:
    if platform.system() == "Darwin":
        return "-Wl,-dead_strip"
    return "-Wl,-z,noexecstack"


def _build_fault_preload(output_dir: Path) -> Path:
    compiler = shutil.which("gcc") or shutil.which("cc")
    if not compiler:
        raise unittest.SkipTest("C compiler is required to build the fault preload helper")

    source = output_dir / "fault_preload.c"
    is_darwin = platform.system() == "Darwin"
    library = output_dir / ("fault_preload.dylib" if is_darwin else "fault_preload.so")
    source.write_text(FAULT_PRELOAD_SOURCE, encoding="utf-8")
    if is_darwin:
        command = [compiler, "-dynamiclib", "-fPIC", source, "-o", library]
    else:
        command = [compiler, "-shared", "-fPIC", "-D_GNU_SOURCE", source, "-o", library, "-ldl"]
    result = subprocess.run(
        command,
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        raise unittest.SkipTest(f"Can't build fault preload helper: {result.stderr}")
    return library


class BuilderCLITests(unittest.TestCase):
    cplc: Path = _default_cplc_path()
    _preload_tmp: tempfile.TemporaryDirectory | None = None
    fault_preload: Path | None = None

    @classmethod
    def setUpClass(cls) -> None:
        if not cls.cplc.exists():
            raise unittest.SkipTest(
                f"cplc binary is not built: {cls.cplc}. "
                "Run `make all` or pass --cplc /path/to/cplc."
            )
        cls._preload_tmp = tempfile.TemporaryDirectory(prefix="cplc-fault-", dir=os.environ.get("CPLC_TEST_TMPDIR", "/tmp"))
        cls.fault_preload = _build_fault_preload(Path(cls._preload_tmp.name))

    @classmethod
    def tearDownClass(cls) -> None:
        if cls._preload_tmp:
            cls._preload_tmp.cleanup()

    def run_cplc(
        self,
        *args: object,
        env: dict[str, str] | None = None,
        cwd: Path | None = None,
        cplc: Path | None = None,
        timeout: int = 10,
    ) -> subprocess.CompletedProcess[str]:
        test_env = os.environ.copy()
        test_env.setdefault("GCOV_ERROR_FILE", os.devnull)
        if env:
            test_env.update(env)

        return subprocess.run(
            [str(cplc or self.cplc), *[str(arg) for arg in args]],
            cwd=cwd or REPO_ROOT,
            env=test_env,
            capture_output=True,
            text=True,
            timeout=timeout,
        )

    def assert_no_stderr(self, result: subprocess.CompletedProcess[str]) -> None:
        self.assertEqual(result.stderr, "")

    def assert_failed_with(self, result: subprocess.CompletedProcess[str], message: str) -> None:
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn(message, result.stderr)

    def fault_env(self, fault: str | None = None, **extra: str) -> dict[str, str]:
        assert self.fault_preload is not None
        if platform.system() == "Darwin":
            env = {
                "DYLD_INSERT_LIBRARIES": str(self.fault_preload),
                "DYLD_FORCE_FLAT_NAMESPACE": "1",
            }
        else:
            env = {"LD_PRELOAD": str(self.fault_preload)}
        if fault:
            env["CPLC_FAULT"] = fault
        env.update(extra)
        return env

    def test_help_lists_builder_options(self) -> None:
        result = self.run_cplc("--help")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("Usage: cplc [options] <input files>", result.stdout)
        self.assertIn("General options:", result.stdout)
        self.assertIn("Linker options:", result.stdout)
        self.assertIn("-c, --compile-only", result.stdout)
        self.assertIn("--emit-asm", result.stdout)
        self.assert_no_stderr(result)

    def test_version_prints_single_line(self) -> None:
        result = self.run_cplc("--version")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertRegex(result.stdout, r"^cplc \S+\n")
        self.assert_no_stderr(result)

    def test_something_prints_embedded_payload(self) -> None:
        result = self.run_cplc("--smth")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertGreater(len(result.stdout), 0)
        self.assert_no_stderr(result)

    def test_no_args_prints_help_but_exits_with_failure(self) -> None:
        result = self.run_cplc()

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Usage: cplc [options] <input files>", result.stdout)
        self.assert_no_stderr(result)

    def test_rejects_missing_option_value(self) -> None:
        result = self.run_cplc("--output")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)

    def test_rejects_invalid_define_name(self) -> None:
        result = self.run_cplc("-D", "1BAD=1")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)

    def test_requires_input_files_for_build_actions(self) -> None:
        result = self.run_cplc("-O2")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("No input files", result.stderr)

    def test_parser_accepts_target_tool_emit_and_optimization_options(self) -> None:
        with _tmpdir() as tmp:
            stdlib = Path(tmp) / "stdlib"
            libdir = Path(tmp) / "libs"
            stdlib.mkdir()
            libdir.mkdir()

            result = self.run_cplc(
                "--arch", "x86",
                "--arch", "amd64",
                "--arch", "unknown-cpu",
                "--asm-compiler", "nasm",
                "--asm-format", "elf64",
                "--linker", "gcc",
                "--linker-mode", "raw",
                "--linker-mode", "driver",
                "-Xlinker", "-z",
                "--linker-arg", "now",
                "-L", libdir,
                "-l", "c",
                "-Wl,--as-needed",
                "--linker-no-pie",
                "--linker-pie",
                "--linker-m32",
                "--linker-no-m32",
                "--entry-name", "custom_entry",
                "--ro-section", ".custom_ro",
                "--glob-section", ".custom_data",
                "--code-section", ".custom_text",
                "--full-bytness", "8",
                "--half-bytness", "4",
                "--quart-bytness", "2",
                "--eight-bytness", "1",
                "--sys-type", "unknown",
                "--sys-type", "macho64",
                "--sys-type", "linux64",
                "--sys-type", "i386",
                "--sys-type", "windows64",
                "--tre",
                "--no-tre",
                "--finline",
                "--no-finline",
                "--licm",
                "--no-licm",
                "--constant",
                "--no-constant",
                "--copyprop",
                "--no-copyprop",
                "--peephole",
                "--no-peephole",
                "-O0",
                "-O1",
                "-O2",
                "-O3",
                "--debug",
                "--no-debug",
                "--i-dont-know-what-i-am-doing",
                "--ast-analysis",
                "--ir-analysis",
                "--emit-ast",
                "--ast-output", "ast.out",
                "--emit-ir",
                "--ir-output", "ir.out",
                "--emit-hir-cfg", "main",
                "--emit-lir",
                "--emit-lir-cfg", "main",
                "--lir-output", "lir.out",
                "--emit-asm",
                "--asm-output", "asm.out",
                "--print-stdlib-path",
                env={"CPL_INCLUDE_PATH": str(stdlib)},
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{stdlib}\n")
        self.assert_no_stderr(result)

    def test_rejects_invalid_parser_values_and_mode_conflicts(self) -> None:
        cases: list[tuple[list[object], str]] = [
            (["--linker-mode", "sideways"], "Can't parse input arguments"),
            (["--full-bytness", "wide"], "Can't parse input arguments"),
            (["--half-bytness", "wide"], "Can't parse input arguments"),
            (["--quart-bytness", "wide"], "Can't parse input arguments"),
            (["--eight-bytness", "wide"], "Can't parse input arguments"),
            (["--sys-type", "plan9"], "Can't parse input arguments"),
            (["--CSA", "--emit-lir"], "Can't parse input arguments"),
            (["--CSA", "-c"], "Can't parse input arguments"),
            (["--strict"], "Can't parse input arguments"),
            (["--no-strict"], "Can't parse input arguments"),
        ]

        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            _minimal_source(source)
            for args, message in cases:
                with self.subTest(args=args):
                    tail = [source] if "--CSA" in args else []
                    result = self.run_cplc(*args, *tail)
                    self.assert_failed_with(result, message)

    def test_rejects_empty_define_and_linker_arg_values(self) -> None:
        cases: list[list[object]] = [
            ["-D", ""],
            ["-D="],
            ["--linker-arg", ""],
        ]

        for args in cases:
            with self.subTest(args=args):
                result = self.run_cplc(*args, "--print-stdlib-path")
                self.assert_failed_with(result, "Can't parse input arguments")

    def test_print_stdlib_path_prefers_environment(self) -> None:
        with _tmpdir() as tmp:
            stdlib = Path(tmp) / "stdlib"
            stdlib.mkdir()

            result = self.run_cplc(
                "--print-stdlib-path",
                env={"CPL_INCLUDE_PATH": str(stdlib)},
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{stdlib}\n")
        self.assert_no_stderr(result)

    def test_print_stdlib_path_finds_packaged_layout_from_executable(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp) / "package"
            cplc = _copy_binary(self.cplc, root / "bin" / "cplc")
            include_dir = root / "share" / "cpl" / "include"
            runtime_dir = root / "lib" / "cpl"
            include_dir.mkdir(parents=True)
            runtime_dir.mkdir(parents=True)
            (runtime_dir / "libcpl.a").write_bytes(b"!<arch>\n")

            result = self.run_cplc(
                "--print-stdlib-path",
                cplc=cplc,
                cwd=Path(tmp),
                env=self.fault_env("readlink"),
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{include_dir.resolve()}\n")
        self.assert_no_stderr(result)

    def test_print_stdlib_path_finds_cwd_fallback_and_sibling_runtime(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            cplc = _copy_binary(self.cplc, root / "cplc")
            stdlib = root / "cpllib"
            stdlib.mkdir()
            (stdlib / "libcpl.a").write_bytes(b"!<arch>\n")

            result = self.run_cplc(
                "--print-stdlib-path",
                cplc=cplc,
                cwd=root,
                env=self.fault_env(None, CPLC_SKIP_DEFAULT_PATHS="1"),
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{stdlib.resolve()}\n")
        self.assert_no_stderr(result)

    def test_print_stdlib_path_reports_missing_when_no_locations_match(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            cplc = _copy_binary(self.cplc, root / "standalone" / "cplc")

            result = self.run_cplc(
                "--print-stdlib-path",
                cplc=cplc,
                cwd=root,
                env=self.fault_env(None, CPLC_SKIP_DEFAULT_PATHS="1"),
            )

        self.assert_failed_with(result, "CPL standard library isn't found")

    def test_runtime_library_prefers_environment(self) -> None:
        with _tmpdir() as tmp:
            stdlib = Path(tmp) / "stdlib"
            runtime = Path(tmp) / "libcpl.a"
            stdlib.mkdir()
            runtime.write_bytes(b"!<arch>\n")

            result = self.run_cplc(
                "--print-stdlib-path",
                env={
                    "CPL_INCLUDE_PATH": str(stdlib),
                    "CPL_RUNTIME_LIB": str(runtime),
                },
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{stdlib}\n")
        self.assert_no_stderr(result)


    def test_preprocess_only_applies_cli_defines(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "defines.cpl"
            source.write_text(
                "\n".join(
                    [
                        "#ifdef ENABLED",
                        "function picked(i32 value = ANSWER + GOOD_1);",
                        "#endif",
                        "#ifndef ENABLED",
                        "function skipped();",
                        "#endif",
                        "",
                    ]
                ),
                encoding="utf-8",
            )

            result = self.run_cplc("-E", "-DENABLED", "-DANSWER=42", "-D", "GOOD_1=7", source)

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("function picked(i32 value = 42 + 7);", result.stdout)
        self.assertNotIn("function skipped();", result.stdout)
        self.assert_no_stderr(result)

    def test_preprocess_only_writes_to_output_file(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            output = Path(tmp) / "preprocessed.cpl"
            source.write_text("function from_input();\n", encoding="utf-8")

            result = self.run_cplc("-E", "--output", output, source)
            output_text = output.read_text(encoding="utf-8")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, "")
        self.assertIn("function from_input();", output_text)
        self.assert_no_stderr(result)

    def test_include_directory_is_used_by_preprocessor(self) -> None:
        with _tmpdir() as tmp:
            include_dir = Path(tmp) / "include"
            include_dir.mkdir()
            (include_dir / "cli_header.cpl").write_text(
                "function from_header();\n",
                encoding="utf-8",
            )

            source = Path(tmp) / "main.cpl"
            source.write_text(
                "#include <cli_header.cpl>\nfunction from_source();\n",
                encoding="utf-8",
            )

            result = self.run_cplc("-E", "-I", include_dir, source)

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("function from_header();", result.stdout)
        self.assertIn("function from_source();", result.stdout)
        self.assert_no_stderr(result)

    def test_conflicting_preprocess_and_compile_only_modes_are_rejected(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            source.write_text("function input();\n", encoding="utf-8")

            result = self.run_cplc("-E", "-c", source)

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)

    def test_preprocess_reports_open_write_and_include_failures(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            source.write_text("function input();\n", encoding="utf-8")

            open_result = self.run_cplc("-E", "--output", Path(tmp), source)
            write_output = Path(tmp) / "write-blocked.cpl"
            write_result = self.run_cplc(
                "-E",
                "--output", write_output,
                source,
                env=self.fault_env("fwrite", CPLC_FAULT_FWRITE_CONTAINS=write_output.name),
            )

            missing_include = Path(tmp) / "missing_include.cpl"
            missing_include.write_text("#include <not_here.cpl>\n", encoding="utf-8")
            include_result = self.run_cplc("-E", "-I", Path(tmp) / "include", missing_include)

        self.assert_failed_with(open_result, "Can't open output file")
        self.assert_failed_with(write_result, "Can't write preprocessed output")
        self.assert_failed_with(include_result, "Failed to preprocess")

    def test_input_open_and_tokenization_failures_are_reported(self) -> None:
        with _tmpdir() as tmp:
            missing = Path(tmp) / "missing.cpl"
            source = Path(tmp) / "main.cpl"
            _minimal_source(source)

            missing_result = self.run_cplc(missing)
            token_result = self.run_cplc(
                source,
                env=self.fault_env("lseek-token-size"),
            )

        self.assert_failed_with(missing_result, f"File {missing} isn't found!")
        self.assert_failed_with(token_result, "ERROR! tkn == NULL!")

    def test_compile_only_writes_explicit_and_default_objects(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            source = root / "main.cpl"
            explicit_object = root / "explicit.o"
            _minimal_source(source)

            explicit_result = self.run_cplc("-c", "--output", explicit_object, source)
            default_result = self.run_cplc("-c", source, cwd=root)
            explicit_exists = explicit_object.is_file()
            explicit_size = explicit_object.stat().st_size if explicit_exists else 0
            default_object = root / "output.o"
            default_exists = default_object.is_file()

        self.assertEqual(explicit_result.returncode, 0, explicit_result.stderr)
        self.assertTrue(explicit_exists)
        self.assertGreater(explicit_size, 0)
        self.assert_no_stderr(explicit_result)
        self.assertEqual(default_result.returncode, 0, default_result.stderr)
        self.assertTrue(default_exists)
        self.assert_no_stderr(default_result)

    def test_compile_only_uses_copy_when_rename_crosses_filesystems(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            output = Path(tmp) / "copied.o"
            _minimal_source(source)

            result = self.run_cplc(
                "-c",
                "--output", output,
                source,
                env=self.fault_env("rename-exdev"),
            )
            output_exists = output.is_file()
            output_size = output.stat().st_size if output_exists else 0

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(output_exists)
        self.assertGreater(output_size, 0)
        self.assert_no_stderr(result)

    def test_compile_only_reports_copy_failures_after_cross_device_rename(self) -> None:
        cases = ["copy-open-src", "copy-open-dst", "copy-read", "copy-write", "copy-close"]

        for fail_point in cases:
            with self.subTest(fail_point=fail_point), _tmpdir() as tmp:
                source = Path(tmp) / "main.cpl"
                output = Path(tmp) / f"{fail_point}.o"
                _minimal_source(source)

                result = self.run_cplc(
                    "-c",
                    "--output", output,
                    source,
                    env=self.fault_env(
                        fail_point,
                        CPLC_FORCE_RENAME_EXDEV="1",
                        CPLC_FAULT_COPY_DST_CONTAINS=str(output),
                    ),
                )

                self.assert_failed_with(result, "Can't write object file")

    def test_compile_only_reports_internal_pipeline_failures(self) -> None:
        cases: list[tuple[str, dict[str, str], list[object], str]] = [
            ("mkstemp", self.fault_env("mkstemp"), [], "Can't create temporary files"),
            ("asm-temp-open", self.fault_env("fopen-builder-temp"), [], "Can't open temporary asm file"),
            ("fork", self.fault_env("fork"), [], "ASM compilation failed"),
            ("waitpid", self.fault_env("waitpid"), [], "ASM compilation failed"),
            (
                "execvp",
                self.fault_env(None, CPLC_FLUSH_EXIT="1"),
                ["--asm-compiler", "definitely-not-cplc-asm"],
                "ASM compilation failed",
            ),
            ("tool-exit", {}, ["--asm-compiler", "/bin/false"], "ASM compilation failed"),
            ("rename-eacces", self.fault_env("rename-eacces"), [], "Can't write object file"),
            (
                "unlink-after-copy",
                self.fault_env(
                    "unlink-builder-temp",
                    CPLC_FORCE_RENAME_EXDEV="1",
                ),
                [],
                "Can't write object file",
            ),
        ]

        for name, env, extra_args, message in cases:
            with self.subTest(name=name), _tmpdir() as tmp:
                source = Path(tmp) / "main.cpl"
                output = Path(tmp) / f"{name}.o"
                _minimal_source(source)

                result = self.run_cplc(
                    *extra_args,
                    "-c",
                    "--output", output,
                    source,
                    env=env,
                )

                self.assert_failed_with(result, message)

    def test_compile_only_emits_default_outputs_and_runs_optimization_pipeline(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            source = root / "main.cpl"
            _minimal_source(source)

            result = self.run_cplc(
                "-O3",
                "--no-z3opt",  # Z3 is optional; exercise the built-in optimization passes.
                "--emit-ast",
                "--emit-ir",
                "--emit-hir-cfg", "main",
                "--emit-lir",
                "--emit-lir-cfg", "main",
                "--emit-asm",
                "-c",
                source,
                cwd=root,
            )
            default_outputs = [
                root / "output.ast",
                root / "output.ir",
                root / "output.lir",
                root / "output.s",
                root / "output.dot",
                root / "output.o",
            ]
            default_outputs_exist = [path.is_file() for path in default_outputs]

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(all(default_outputs_exist), default_outputs)
        self.assert_no_stderr(result)

    def test_compile_only_emits_explicit_outputs(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            source = root / "main.cpl"
            ast_output = root / "tree.ast"
            ir_output = root / "graph.ir"
            lir_output = root / "graph.lir"
            asm_output = root / "program.s"
            obj_output = root / "program.o"
            _minimal_source(source)

            result = self.run_cplc(
                "--ast-output", ast_output,
                "--ir-output", ir_output,
                "--emit-hir-cfg", "main",
                "--lir-output", lir_output,
                "--emit-lir-cfg", "main",
                "--asm-output", asm_output,
                "-c",
                "--output", obj_output,
                source,
                cwd=root,
            )
            explicit_outputs = [ast_output, ir_output, lir_output, asm_output, obj_output]
            explicit_output_stats = [(path.is_file(), path.stat().st_size if path.is_file() else 0) for path in explicit_outputs]

        self.assertEqual(result.returncode, 0, result.stderr)
        for exists, size in explicit_output_stats:
            self.assertTrue(exists)
            self.assertGreater(size, 0)
        self.assert_no_stderr(result)

    def test_emit_output_open_failures_are_reported(self) -> None:
        cases: list[tuple[str, str, str]] = [
            ("ast", "ast-output-open", "Can't open AST output file"),
            ("ir", "ir-output-open", "Can't open HIR output file"),
            ("hir-cfg", "output.dot", "Can't open HIR CFG output file"),
            ("lir", "lir-output-open", "Can't open LIR output file"),
            ("lir-cfg", "output.dot", "Can't open LIR CFG output file"),
            ("asm", "asm-output-open", "Can't open ASM output file"),
        ]

        for kind, marker, message in cases:
            with self.subTest(kind=kind), _tmpdir() as tmp:
                source = Path(tmp) / "main.cpl"
                output = Path(tmp) / "main.o"
                _minimal_source(source)
                marker_path = Path(tmp) / marker
                args_by_kind: dict[str, list[object]] = {
                    "ast": ["--ast-output", marker_path],
                    "ir": ["--ir-output", marker_path],
                    "hir-cfg": ["--emit-hir-cfg", "main"],
                    "lir": ["--lir-output", marker_path],
                    "lir-cfg": ["--emit-lir-cfg", "main"],
                    "asm": ["--asm-output", marker_path],
                }

                result = self.run_cplc(
                    *args_by_kind[kind],
                    "-c",
                    "--output", output,
                    source,
                    cwd=Path(tmp),
                    env=self.fault_env(None, CPLC_FAULT_FOPEN_CONTAINS=marker),
                )

                self.assert_failed_with(result, message)

    def test_analysis_only_runs_ast_and_hir_cleanup_path(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            ast_output = Path(tmp) / "analysis.ast"
            ir_output = Path(tmp) / "analysis.ir"
            source.write_text("start() {\n    exit 0 as u8;\n}\n", encoding="utf-8")

            result = self.run_cplc(
                "--CSA",
                "--ast-output", ast_output,
                "--ir-output", ir_output,
                "--emit-hir-cfg", "main",
                source,
                cwd=Path(tmp),
            )
            ast_exists = ast_output.is_file()
            ir_exists = ir_output.is_file()

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(ast_exists)
        self.assertTrue(ir_exists)
        self.assert_no_stderr(result)

    def test_strict_ast_analysis_stops_on_blocking_issue(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            output = Path(tmp) / "main.o"
            _minimal_source(source)

            result = self.run_cplc(
                "--ast-analysis",
                "--i-dont-know-what-i-am-doing",
                "-c",
                "--output", output,
                source,
            )

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("[WARNING]", result.stdout)
        self.assertIn("AST semantic analysis failed", result.stderr)

    def test_strict_hir_analysis_stops_on_blocking_issue(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            output = Path(tmp) / "main.o"
            source.write_text(
                "start() {\n"
                "    i32 a = 0;\n"
                "    exit (10 / a) as u8;\n"
                "}\n",
                encoding="utf-8",
            )

            result = self.run_cplc(
                "--ir-analysis",
                "--constant",
                "--i-dont-know-what-i-am-doing",
                "-c",
                "--output", output,
                source,
            )

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("[WARNING]", result.stdout)
        self.assertIn("HIR semantic analysis failed", result.stderr)

    def test_i_know_what_i_am_doing_disables_strict_analysis_failure(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            output = Path(tmp) / "main.o"
            _minimal_source(source)

            result = self.run_cplc(
                "--i-know-what-i-am-doing",
                "--ast-analysis",
                "-c",
                "--output", output,
                source,
            )
            output_exists = output.is_file()

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(output_exists)

    def test_multiple_input_files_are_parsed_before_finalization(self) -> None:
        with _tmpdir() as tmp:
            first = Path(tmp) / "decl.cpl"
            second = Path(tmp) / "main.cpl"
            output = Path(tmp) / "multi.o"
            first.write_text("function helper();\n", encoding="utf-8")
            _minimal_source(second)

            result = self.run_cplc("-c", "--output", output, first, second)
            output_exists = output.is_file()

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue(output_exists)
        self.assert_no_stderr(result)

    def test_executable_build_links_default_and_explicit_outputs(self) -> None:
        with _tmpdir() as tmp:
            root = Path(tmp)
            source = root / "main.cpl"
            explicit_output = root / "program"
            _minimal_source(source)

            default_result = self.run_cplc(
                "--linker-arg", _linker_passthrough_arg(),
                source,
                cwd=root,
            )
            explicit_result = self.run_cplc(
                "--output", explicit_output,
                "--linker-arg", _linker_passthrough_arg(),
                source,
            )
            default_exists = (root / "a.out").is_file()
            explicit_exists = explicit_output.is_file()

        self.assertEqual(default_result.returncode, 0, default_result.stderr)
        self.assertTrue(default_exists)
        self.assert_no_stderr(default_result)
        self.assertEqual(explicit_result.returncode, 0, explicit_result.stderr)
        self.assertTrue(explicit_exists)
        self.assert_no_stderr(explicit_result)

    def test_linking_failures_are_reported(self) -> None:
        cases: list[tuple[str, dict[str, str], list[object], str]] = [
            ("linker-exit", {}, ["--linker", "/bin/false"], "Linking failed"),
            ("linker-m32", {}, ["--linker", "/bin/false", "--linker-m32"], "Linking failed"),
            ("linker-raw", {}, ["--linker", "/bin/false", "--linker-mode", "raw"], "Linking failed"),
        ]

        for name, env, extra_args, message in cases:
            with self.subTest(name=name), _tmpdir() as tmp:
                source = Path(tmp) / "main.cpl"
                output = Path(tmp) / name
                _minimal_source(source)

                result = self.run_cplc(
                    *extra_args,
                    "--output", output,
                    source,
                    env=env,
                )

                self.assert_failed_with(result, message)

    def test_target_selection_paths_are_reached(self) -> None:
        cases: list[tuple[str, list[object], str]] = [
            (
                "macho64",
                ["--sys-type", "macho64", "--asm-format", "macho64", "--entry-name", "_main", "--asm-compiler", "/bin/false"],
                "ASM compilation failed",
            ),
            (
                "i386",
                ["--arch", "i386", "--sys-type", "i386", "--asm-format", "elf32", "--asm-compiler", "/bin/false"],
                "ASM compilation failed",
            ),
        ]

        for name, args, message in cases:
            with self.subTest(name=name), _tmpdir() as tmp:
                source = Path(tmp) / "main.cpl"
                output = Path(tmp) / f"{name}.o"
                _minimal_source(source)

                result = self.run_cplc(
                    *args,
                    "-c",
                    "--output", output,
                    source,
                )

                self.assert_failed_with(result, message)

    def test_unknown_codegen_target_reaches_default_generator_path(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            output = Path(tmp) / "unknown.o"
            _minimal_source(source)

            result = self.run_cplc(
                "--sys-type", "unknown",
                "-c",
                "--output", output,
                source,
                env=self.fault_env(None, CPLC_FLUSH_SEGV="1"),
            )

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")

    def test_preprocess_only_reaches_windows_and_unknown_targets(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "main.cpl"
            _minimal_source(source)

            windows_result = self.run_cplc("--sys-type", "windows64", "-E", source)
            unknown_result = self.run_cplc("--sys-type", "unknown", "-E", source)

        self.assertEqual(windows_result.returncode, 0, windows_result.stderr)
        self.assertEqual(unknown_result.returncode, 0, unknown_result.stderr)
        self.assertIn("start()", windows_result.stdout)
        self.assertIn("start()", unknown_result.stdout)
        self.assert_no_stderr(windows_result)
        self.assert_no_stderr(unknown_result)


def _entry() -> None:
    parser = argparse.ArgumentParser(description="Builder CLI tests")
    parser.add_argument(
        "--cplc",
        default=str(_default_cplc_path()),
        help="Path to the cplc binary under test",
    )
    args, unittest_args = parser.parse_known_args()

    BuilderCLITests.cplc = Path(args.cplc).resolve()
    unittest.main(argv=[sys.argv[0], *unittest_args], verbosity=2)


if __name__ == "__main__":
    _entry()
