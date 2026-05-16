# Design goals and scope

CPL is designed as a deliberately restricted language plus a visible compiler pipeline. The project is more useful as a research and teaching artifact than as a production programming language.

## Goals

### Low-level explicitness

CPL exposes operations that are normally close to the target machine: pointers, explicit dereferencing, direct syscalls, inline assembly, manual entry points, section placement, alignment, and register-related annotations.

### Small language core

The language avoids a large surface syntax. It does not attempt to clone C, Rust, or Zig. Instead, it keeps the core small enough that the full compiler pipeline can be inspected and modified.

### Compiler experimentation

The compiler is a testbed for:

- AST and HIR diagnostics;
- CFG and call graph construction;
- SSA-related transformations;
- constant folding and propagation;
- loop-invariant code motion;
- inlining and tail-recursion elimination;
- LIR lowering and instruction selection;
- register allocation;
- peephole optimization;
- target-sensitive assembly generation.

### System-oriented examples

CPL programs can use explicit syscalls and inline assembly, so small operating-system-style examples and low-level runtime experiments can be represented without a large standard library.

## Non-goals

CPL does not currently aim to provide:

- memory safety;
- borrow checking or ownership analysis;
- formal verification of the compiler;
- C source compatibility;
- a production standard library;
- user-defined structures, classes, enums, traits, or modules;
- a package manager or stable public ecosystem.

## Positioning

CPL is closest in spirit to an educational systems-language testbed. Compared with a production language, the interesting part is not only the surface syntax but the way source programs move through the compiler. When reading the documentation, treat language pages as the current user-facing reference, compiler pages as implementation notes, and changelog/TODO pages as project history.
