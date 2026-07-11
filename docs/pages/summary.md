# Summary

The **Cordell Programming Language (CPL)** is a small experimental systems language and compiler infrastructure. Its surface syntax is intentionally close to low-level C-like programming, but it is not a C subset. The project is primarily a compact testbed for compiler construction: parsing, semantic analysis, intermediate representations, optimization, register allocation, assembly generation, and diagnostics.

This documentation is organized in the same way as documentation for mature languages: a learning path, a language reference, compiler/toolchain notes, and project-status material are separated so that each page has a clear role.

## Intended audience

CPL documentation is written for:

- readers who want a concise description of the current language;
- compiler and programming-language instructors evaluating the design and implementation;
- students who want to follow a complete compiler pipeline in a real codebase;
- contributors who need to know which behavior is supported, experimental, or intentionally absent.

## What CPL is for

CPL is intended for:

- studying compiler architecture on a working implementation;
- low-level experiments where direct memory access, explicit entry points, syscalls, and inline assembly are acceptable;
- testing optimization passes and static-analysis ideas;
- writing small system-oriented examples, interpreters, and benchmarks.

CPL is not intended to be a safe production language. It has no borrow checker, no garbage collector, no formal memory-safety model, and no production ecosystem.

## Language core

The current language core includes:

- primitive signed and unsigned integer types, floating-point types, and `i0`;
- explicit `ptr`, `ref`, and `dref` pointer operations;
- fixed-size `arr` arrays;
- `if`, `else`, `while`, `loop`, `switch`, `case`, `default`, and `break`;
- functions, prototypes, default arguments, overloads, generic functions, local functions, lambdas, and function pointers;
- `glob`, `ro`, and `extern` declarations;
- `syscall` and inline `asm`;
- a small preprocessor with include, define, undef, ifdef, and ifndef directives;
- annotations for entry points, explicit emitted symbol names, ABI/linker metadata, sections, alignment, switch lowering, branch layout, counted loops, register placement, and varargs.

The current implementation intentionally does not include enums, modules, a package manager, exceptions, or memory-safety guarantees.

## Compiler pipeline

The compiler currently follows this pipeline:

1. Preprocess input files.
2. Tokenize and mark up tokens.
3. Build AST and symbol tables.
4. Optionally run AST semantic analysis.
5. Generate HIR.
6. Build CFG and call graph information.
7. Apply HIR-level transformations and optimizations.
8. Generate LIR.
9. Select instructions, allocate registers, select memory locations, and optionally run peephole optimization.
10. Generate NASM assembly, assemble object files, and link the final executable.

For implementation-level details, see [Compiler architecture](#/pages/compiler-architecture). For command-line usage, see [Compiler usage](#/pages/compiler-usage).
