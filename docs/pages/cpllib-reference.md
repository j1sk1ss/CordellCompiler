# CPL standard library reference

`cpllib` is the standard library shipped with the compiler. It has two layers:

- C/POSIX-style headers such as `stdio_h.cpl`, `stdlib_h.cpl`, `string_h.cpl`,
  `unistd_h.cpl`, and `math_h.cpl`.
- CPL convenience containers and wrappers implemented in `libcpl.a`, mainly
  `string`, `linked_list`, `queue`, `stack`, and the static `std` helper
  container from `io_h.cpl`.

The library is intentionally small. Most headers are thin ABI declarations for
the host runtime, while the CPL containers are simple building blocks for
examples, tests, and small programs.

## Include and link

Use system-style includes for library headers:

```cpl
#include <io_h.cpl>

start() {
    std::print(ref "hello from cpllib");
    exit 0;
}
```

Quoted includes are best for project-local files:

```cpl
#include "local_header.cpl"
#include <stdio_h.cpl>
```

The compiler searches includes in this order:

1. The directory of the current file for quoted includes.
2. The directory passed with `-I`.
3. The standard-library directory from `CPL_INCLUDE_PATH`.
4. Adjacent package/install locations.
5. The compiled-in install prefix.
6. The repository-local `cpllib` directory.

Check what the compiler sees with:

```bash
cplc --print-stdlib-path
```

The runtime implementations are compiled by the root `Makefile`:

```bash
make cpllib
make BUILD=release PRINT_PARSE=0 cpllib
```

The archive is written to `builds/<platform>/cpllib/libcpl.a`. Installed
compilers use the compiled-in runtime path, and package builds use the adjacent
runtime path inside the package tree.

## File layout

| File | Role |
|---|---|
| `cpllib/*_h.cpl` | Public headers. These define constants, type aliases, containers, extern declarations, and inline wrappers. |
| `cpllib/string.cpl` | Implementation for the CPL `string` helper container. |
| `cpllib/list.cpl` | Implementation for `linked_list` and `linked_list_block`. |
| `cpllib/queue.cpl` | Implementation for `queue`. |
| `cpllib/stack.cpl` | Implementation for `stack`. |
| `builds/<platform>/cpllib/libcpl.a` | Static archive produced by `make cpllib`. |

## The `io_h.cpl` convenience layer

For ordinary programs, start with `io_h.cpl`. It includes `stdio_h.cpl`,
`stdlib_h.cpl`, `fcntl_h.cpl`, `unistd_h.cpl`, and `string_h.cpl`, then exposes
static helpers on `std`.

```cpl
#include <io_h.cpl>

start() {
    std::print(ref "line");
    std::write(ref "no newline");
    std::putc('\n');

    ptr i32 values = std::calloc<i32>(4);
    if not values; exit 1;
    values[0] = 10;
    values[1] = 20;
    std::free<i32>(values);

    exit 0;
}
```

Main `std` helpers:

| Group | Functions |
|---|---|
| Console | `std::write(ptr i8)`, `std::write(ptr string)`, `std::print(ptr i8)`, `std::print(ptr string)`, `std::print(i32)`, `std::print(i64)`, `std::print(u64)`, `std::print(f64)`, `std::putc`, `std::getc`, `std::gets` |
| File descriptors | `std::open`, `std::create`, `std::read`, `std::read<T>`, `std::write`, `std::write<T>`, `std::fread`, `std::fwrite`, `std::close`, `std::seek`, `std::tell`, `std::flush`, `std::truncate`, `std::exists`, `std::remove`, `std::rename` |
| Buffered streams | `std::fopen`, `std::fclose`, `std::fread`, `std::fread<T>`, `std::fwrite`, `std::fwrite<T>`, `std::flush`, `std::seek`, `std::tell` |
| Memory | `std::malloc`, `std::calloc`, `std::calloc<T>`, `std::realloc`, `std::realloc<T>`, `std::free`, `std::free<T>`, `std::copy<T>`, `std::move<T>`, `std::zero<T>` |
| Strings | `std::strlen(ptr i8)`, `std::strlen(ptr string)`, `std::strcmp(ptr i8, ptr i8)`, `std::strcmp(ptr string, ptr string)`, `std::strdup(ptr i8)`, `std::strdup(ptr string)` |

Prefer the typed helpers when possible. For example, `std::calloc<i32>(16)`
returns `ptr i32`, and `std::copy<i32>(dst, src, count)` computes byte sizes
from `sizeof(i32)`.

Avoid `std::gets(buffer)` for new code because it wraps the unsafe libc
`gets`. Use `std::gets(buffer, size)` when reading a bounded line.

## `string`

`string_h.cpl` declares both libc-style byte-string functions and the CPL
`string` container. The container owns a duplicated null-terminated byte buffer:

```cpl
#include <string_h.cpl>
#include <io_h.cpl>

start() {
    ptr string name = string::new(ref "cordell");
    if not name; exit 1;

    if name.strlen() != 7; exit 2;
    std::print(name);

    name.destroy();
    exit 0;
}
```

`string` fields and methods:

| Member | Meaning |
|---|---|
| `body` | Owned `ptr i8` null-terminated byte buffer. |
| `string::new(ptr i8)` | Allocates a `string` object and duplicates the input bytes. |
| `strlen()` | Returns the byte length of `body`. |
| `equals(other)` | Compares two `string` objects with `strcmp`. |
| `destroy()` | Frees both `body` and the `string` object. |
| `print()` | Prints `body` through `printf`. Use with trusted format strings only. |

After `destroy`, the pointer must not be reused.

## `linked_list`

`list_h.cpl` provides an untyped doubly linked list. It stores payloads as
`ptr i0`; the list owns only its internal nodes.

```cpl
#include <list_h.cpl>

start() {
    i32 a = 10;
    i32 b = 20;

    ptr linked_list list = linked_list::new();
    if not list; exit 1;

    list.push_back(ref a);
    list.push_front(ref b);

    if dref (list.front() as ptr i32) != 20; exit 2;
    if dref (list.back() as ptr i32) != 10; exit 3;

    list.clear();
    list.destroy();
    exit 0;
}
```

List operations:

| Operation | Behavior |
|---|---|
| `linked_list::new()` | Allocates and initializes an empty list. |
| `init()` | Initializes an already allocated list object. Existing nodes are not freed first. |
| `add(elem)` | Alias for `push_back`. |
| `push_front(elem)`, `push_back(elem)` | Insert a caller-owned payload pointer. |
| `front()`, `back()` | Read the first or last payload without removing it. |
| `pop_front()`, `pop_back()` | Remove a node and return its payload pointer. |
| `remove(block)` | Remove a specific `linked_list_block` that belongs to this list. |
| `clear()` | Free all list nodes and reset length to zero. Payloads are not freed. |
| `destroy()` | Clear the list and free the list object. |
| `size()`, `empty()` | Inspect cached length. |

Do not pass a node from one list to another list's `remove`; that corrupts both
lists because neighboring links are updated directly.

## `queue`

`queue_h.cpl` implements a FIFO queue on top of `linked_list`. Payload ownership
is the same as for the list: the queue frees nodes, not the data pointed to by
payloads.

```cpl
#include <queue_h.cpl>

start() {
    i32 first = 1;
    i32 second = 2;

    ptr queue q = queue::new();
    if not q; exit 1;

    q.push(ref first);
    q.push(ref second);

    if dref (q.front() as ptr i32) != 1; exit 2;
    if dref (q.pop() as ptr i32) != 1; exit 3;
    if dref (q.pop() as ptr i32) != 2; exit 4;

    q.destroy();
    exit 0;
}
```

Queue operations: `new`, `init`, `push`, `pop`, `front`, `back`, `clear`,
`destroy`, `size`, and `empty`.

## `stack`

`stack_h.cpl` implements a LIFO stack on top of `linked_list`.

```cpl
#include <stack_h.cpl>

start() {
    i32 first = 1;
    i32 second = 2;

    ptr stack s = stack::new();
    if not s; exit 1;

    s.push(ref first);
    s.push(ref second);

    if dref (s.top() as ptr i32) != 2; exit 2;
    if dref (s.pop() as ptr i32) != 2; exit 3;
    if dref (s.pop() as ptr i32) != 1; exit 4;

    s.destroy();
    exit 0;
}
```

Stack operations: `new`, `init`, `push`, `pop`, `top`, `clear`, `destroy`,
`size`, and `empty`.

## Header function map

Use this table as a quick map. The detailed signatures are in the corresponding
`cpllib/*_h.cpl` header comments.

| Header | Functions and definitions |
|---|---|
| `io_h.cpl` | `std` wrappers for printing, descriptor I/O, stream I/O, allocation, typed memory operations, string length/compare/duplication. |
| `stdio_h.cpl` | `remove`, `rename`, `tmpfile`, `tmpnam`, `fclose`, `fflush`, `fopen`, `freopen`, `setbuf`, `setvbuf`, `fprintf`, `fscanf`, `printf`, `scanf`, `snprintf`, `sprintf`, `sscanf`, `fgetc`, `fgets`, `fputc`, `fputs`, `getc`, `getchar`, `gets`, `putc`, `putchar`, `puts`, `ungetc`, `fread`, `fwrite`, `fgetpos`, `fseek`, `fsetpos`, `ftell`, `rewind`, `clearerr`, `feof`, `ferror`, `perror`, plus `stdin`, `stdout`, `stderr`, `EOF`, and seek constants. |
| `stdlib_h.cpl` | `malloc`, `calloc`, `realloc`, `aligned_alloc`, `posix_memalign`, `free`, `abort`, `_Exit`, `atexit`, `system`, `getenv`, `atoi`, `atol`, `atoll`, `strtol`, `strtoll`, `strtoul`, `strtoull`, `strtod`, `strtof`, `strtold`, `rand`, `srand`, `abs`, `labs`, `llabs`, `div`, `ldiv`, `lldiv`, `bsearch`, `qsort`, plus `EXIT_SUCCESS`, `EXIT_FAILURE`, `RAND_MAX`. |
| `string_h.cpl` | `memcpy`, `memmove`, `memset`, `memcmp`, `memchr`, `strlen`, `strnlen`, `strcpy`, `strncpy`, `strcat`, `strncat`, `strcmp`, `strncmp`, `strchr`, `strrchr`, `strstr`, `strpbrk`, `strspn`, `strcspn`, `strcoll`, `strxfrm`, `strtok`, `strtok_r`, `strerror`, `strdup`, `strndup`, plus the CPL `string` container. |
| `ctype_h.cpl` | C-style aliases such as `char`, `int`, `long`, `double`; `isalnum`, `isalpha`, `isblank`, `iscntrl`, `isdigit`, `isgraph`, `islower`, `isprint`, `ispunct`, `isspace`, `isupper`, `isxdigit`, `tolower`, `toupper`. |
| `math_h.cpl` | `f64` functions `acos`, `asin`, `atan`, `atan2`, `cos`, `sin`, `tan`, `cosh`, `sinh`, `tanh`, `exp`, `frexp`, `ldexp`, `log`, `log10`, `modf`, `pow`, `sqrt`, `ceil`, `fabs`, `floor`, `fmod`, `cbrt`, `copysign`, `exp2`, `hypot`, `log2`, `round`, `trunc`; `f32` variants ending in `f`; `HUGE_VAL`, `INFINITY`, `NAN`. |
| `fcntl_h.cpl` | `open`, `creat`, `fcntl`, open flags such as `O_RDONLY`, `O_WRONLY`, `O_RDWR`, `O_CREAT`, `O_TRUNC`, `O_APPEND`, `O_CLOEXEC`, and permission bits. |
| `unistd_h.cpl` | `read`, `write`, `close`, `lseek`, `unlink`, `rmdir`, `chdir`, `getcwd`, `access`, `dup`, `dup2`, `pipe`, `pipe2`, `fork`, `_exit`, `execve`, `execv`, `execl`, `getpid`, `getppid`, `getuid`, `geteuid`, `getgid`, `getegid`, `isatty`, `fsync`, `ftruncate`, `sleep`, `usleep`, plus standard descriptor constants. |
| `sys_stat_h.cpl` | `stat`, `fstat`, `lstat`, `chmod`, `fchmod`, `mkdir`, `mkfifo`, `umask`, file-type masks, and permission bits. |
| `sys_wait_h.cpl` | `wait`, `waitpid`, `WNOHANG`, `WUNTRACED`. |
| `poll_h.cpl` | `c_pollfd`, `poll`, and `POLL*` event flags. |
| `signal_h.cpl` | `signal`, `raise`, `SIG_DFL`, `SIG_IGN`, `SIG_ERR`. |
| `time_h.cpl` | `clock`, `time`, `difftime`, `mktime`, `asctime`, `ctime`, `gmtime`, `localtime`, `strftime`, `clock_gettime`, `clock_getres`, `nanosleep`, `c_tm`, `c_timespec`, and clock constants. |
| `locale_h.cpl` | `setlocale`, `localeconv`, locale category constants, and `c_lconv`. |
| `stdint_h.cpl`, `inttypes_h.cpl` | Fixed-width integer aliases and limits; `imaxabs`, `strtoimax`, `strtoumax`. |
| `stddef_h.cpl`, `stdbool_h.cpl`, `limits_h.cpl`, `float_h.cpl`, `types_h.cpl`, `sys_types_h.cpl` | Size, pointer, boolean, numeric limit, and shared C-compatible type definitions such as `FILE`, `c_div`, `c_ldiv`, `c_lldiv`, `clock_t`, and `time_t`. |
| `platform_h.cpl` | Target macros such as `CCPL_MACHO64`, `CCPL_GNU64`, `CCPL_GNUI386`, `CCPL_WINDOWS64`, and derived platform markers. |
| `errno_h.cpl` | Header guard placeholder for errno-related integration. |

## Ownership and ABI rules

`linked_list`, `queue`, and `stack` do not copy or free payloads. If a payload
was allocated with `malloc` or `std::calloc<T>`, the caller must free it after
removing it or before dropping all references to it.

`string::new` duplicates the input bytes and `string::destroy` frees that owned
copy. `strdup` and `strndup` return heap allocations that must be released with
`free` or `std::free`.

Most non-container functions are `extern @[abi]` declarations. They call the
host C runtime and follow the selected target ABI. On Mach-O targets the
headers use `@[vname("_name")]` for linker symbols with leading underscores; on
GNU-style targets they use the plain C names.

Variadic functions such as `printf`, `scanf`, `open`, `fcntl`, and `execl`
depend on the target calling convention. Cast arguments explicitly when a
format string or ABI expects a specific width.

## Examples

Small executable examples are kept in:

| Example | Demonstrates |
|---|---|
| `examples/small/cplib.cpl` | `io_h.cpl` and `std::print`. |
| `examples/small/list_usecase.cpl` | `linked_list` insertion, access, popping, and destruction. |
| `examples/small/queue_usecase.cpl` | FIFO `queue` behavior. |
| `examples/small/stack_usecase.cpl` | LIFO `stack` behavior. |
