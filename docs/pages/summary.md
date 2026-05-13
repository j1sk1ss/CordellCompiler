# Summary

The **Cordell Programming Language (CPL)** is a small experimental systems language and a compiler playground. The language is intentionally close to C in shape, but it uses its own syntax for pointers, annotations, local functions, lambdas, and explicit casts.

The project is primarily about compiler construction: preprocessing, tokenization, AST generation, semantic checks, HIR/CFG/SSA transformations, LIR generation, register allocation, peephole optimization, and NASM code generation.

## What CPL is for

CPL is intended for:

- learning compiler architecture on a real codebase;
- low-level experiments where direct memory access and assembly are acceptable;
- small programs, interpreters, bootstrapping experiments, and system-oriented prototypes;
- testing optimization passes and static analysis ideas.

CPL is not designed as a safe or general-purpose application language. It has no borrow checker, no memory safety model, and no user-defined structures/classes/enums in the current implementation.

## Main language features

- Primitive integer and floating-point types: `i8`, `u8`, `i16`, `u16`, `i32`, `u32`, `i64`, `u64`, `f32`, `f64`, and `i0`.
- `ptr` pointers with `ref` and `dref`.
- `str` strings and `arr` arrays.
- `if`, `else`, `while`, `loop`, `switch`, `case`, `default`, and `break`.
- Functions, prototypes, default arguments, overloads, generic functions, local functions, lambdas, and function pointers.
- `glob`, `ro`, and `extern` declarations.
- `syscall` and inline `asm`.
- A small preprocessor with `#include`, `#define`, `#undef`, `#ifdef`, and `#ifndef`.
- Annotations such as `entry`, `naked`, `section`, `align`, `counter`, `no_fall`, `straight`, `hot`, `cold`, `not_lazy`, `register`, and `poparg`.

## Compiler pipeline

The current compiler pipeline is:

1. Preprocess input files.
2. Tokenize and mark up tokens.
3. Build AST and symbol tables.
4. Optionally run AST semantic analysis.
5. Generate HIR.
6. Build CFG and call graph.
7. Apply HIR-level transformations and optimizations.
8. Generate LIR.
9. Select instructions, allocate registers, select memory, and optionally run peephole optimization.
10. Generate NASM assembly, assemble it, and link the final executable.
