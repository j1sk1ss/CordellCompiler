# Project status

CPL is an experimental language and compiler implementation. This page states the current scope so that the rest of the documentation can be read with the right expectations.

## Current status

The compiler can build and run non-trivial low-level examples, including pointer-heavy code, string traversal, direct syscall usage, inline assembly, containers, and benchmark programs. The documentation currently describes CPL v3.5-era behavior.

The implementation includes:

- preprocessing and tokenization;
- AST construction and source-level semantic checks;
- HIR generation and IR-level checks;
- CFG and call graph construction;
- SSA-related infrastructure;
- HIR and LIR optimization passes;
- LIR generation, instruction selection, register allocation, and memory selection;
- x86-64 NASM assembly generation for Mach-O and Linux-style targets;
- optional benchmark visualization and a static docsify playground frontend.

## Supported language areas

The language currently supports:

- primitive integer and floating-point types;
- pointer operations through `ptr`, `ref`, and `dref`;
- strings and fixed-size arrays;
- user-defined value-layout containers with fields, nested containers, array fields, generic functions, and explicit `@[self]` methods;
- basic control flow and switch lowering annotations;
- functions, overloads, generic functions, default arguments, local functions, lambdas, and function pointers;
- direct syscalls and inline assembly;
- low-level annotations for entry points, sections, alignment, counted loops, register placement, and varargs;
- a small C-like preprocessor.

## Important limitations

CPL currently does not support:

- classes, enums, traits, modules, inheritance, or virtual dispatch;
- memory safety guarantees;
- ownership, borrowing, or lifetime checking;
- a stable ABI or package ecosystem;
- a standard library comparable to C, Rust, Go, or Python;
- formal compiler verification;
- a complete cross-platform runtime abstraction.

These limitations are part of the current project scope. The compiler is meant to stay small enough to inspect while still exercising realistic compiler stages.

## Target and deployment notes

The compiler emits NASM assembly and then uses an external assembler and linker. The default target settings are close to Mach-O x86-64; Linux x86-64 requires explicit command-line options. See [Compiler usage](#/pages/compiler-usage) for examples.

The online documentation is a static docsify site. The editor in [Playground](#/pages/playground) works as a frontend, but live execution requires a separate backend service. On GitHub Pages, the page remains useful for reading and formatting examples but cannot compile programs by itself.

## Reading benchmarks

The benchmark page is useful as an implementation record, not as a formal performance claim. The benchmarks are small, machine-dependent, and in several cases compare low-level loop behavior rather than complete real-world workloads. See [Benchmarking](#/pages/benchmarking) for methodology notes.

## Project record

[Changelog](#/pages/changelog) is intentionally preserved as a development log. It can contain informal notes and historical implementation details. [TODO](#/pages/TODO) records planned or recently completed ideas and should not be read as a stable specification.
