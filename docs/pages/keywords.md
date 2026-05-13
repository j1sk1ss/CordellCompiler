# CPL keywords

This page lists the language keywords that are recognized by the current tokenizer and parser.

## Declarations and storage

| Keyword | Meaning | Example |
|---|---|---|
| `function` | function definition or prototype | `function add(i64 a, i64 b) -> i64;` |
| `extern` | external function or variable | `extern function puts(ptr i8 s) -> i32;` |
| `glob` | global symbol | `glob i32 counter = 0;` |
| `ro` | read-only data | `ro i32 answer = 42;` |
| `arr` | array declaration | `arr a[3, i8] = { 'A', 'B', 'C' };` |
| `str` | string object | `str msg = "hello";` |

## Types and pointer operators

| Keyword | Meaning | Example |
|---|---|---|
| `i0` | void-like type | `function f() -> i0;` |
| `i8`, `i16`, `i32`, `i64` | signed integers | `i64 x = 1;` |
| `u8`, `u16`, `u32`, `u64` | unsigned integers | `u8 b = 0xFF;` |
| `f32`, `f64` | floating-point values | `f64 pi = 3.14;` |
| `ptr` | pointer modifier | `ptr i8 s;` |
| `ref` | take address | `ptr i32 p = ref x;` |
| `dref` | dereference pointer | `i32 x = dref p;` |
| `as` | explicit conversion | `i8 c = x as i8;` |
| `sizeof` | object/type size query | `i64 n = sizeof(a);` |

## Control flow

| Keyword | Meaning | Example |
|---|---|---|
| `start` | program entry point | `start() { exit 0; }` |
| `exit` | process exit from entry point | `exit 0;` |
| `return` | return from function | `return x;` |
| `if`, `else` | conditional branches | `if x; a(); else b();` |
| `while` | conditional loop | `while i < n; i += 1;` |
| `loop` | unconditional loop | `loop { break; }` |
| `break` | leave loop or switch case | `break;` |
| `switch`, `case`, `default` | multi-way branch | `switch x; { case 1; {} default {} }` |

## Low-level built-ins

| Keyword | Meaning | Example |
|---|---|---|
| `syscall` | target-dependent system call | `syscall(0x2000004, 1, ref c, 1);` |
| `asm` | inline assembly block | `asm(x) { "mov rax, %0" }` |
| `not` | boolean-like negation | `not x` |
