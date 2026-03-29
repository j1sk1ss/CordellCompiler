# Language Basics and Logic
The **Cordell Programming Language (CPL)** is a system-level programming language designed for learning and experimenting with modern compiler concepts. It combines low-level capabilities from `ASM` with practices inspired by modern languages like `Rust` and `C`.

## What CPL is for
CPL is intended for:
- **Radical simplicity** — the language aims to stay as small and explicit as possible.
- **No safety** — CPL does not attempt to provide memory safety. Instead, it prioritizes explicit low-level control and lightweight static diagnostics.
- **System programming** — the language is designed primarily for bootloaders (1-stage, 2-stage), edge DBMSs, edge file systems, primitive compilers, and small interpreters. It is not intended for engines, web services, operating systems, games, and similar large-scale software. You may still try unsupported use cases, but the language is not designed around them.
- **Experiments** — the compiler is a way to test compilation techniques in a real project without redundant complexity. It contains artifacts from experiments such as *symtable compression*, *perceptron inline*, *peephole DSL*, and the *AST static analyzer*. See the main README for further details.
- **Educational purpose** — the language is meant for studying compiler design, interpreters, and programming language concepts. It is not intended to be a general-purpose language.

## Key features
There is not much here that is conceptually new. CPL is mostly based on C-like ideas, with a number of deliberate restrictions. In short, the language provides:
- **Permissive static typing** — variables cannot hold values of different types. The compiler may perform implicit **widening** conversions where they are safe enough for CPL. **Narrowing is never implicit and is allowed only with the `as` keyword.**
- **No structures** — the language supports only primitive types as part of the experiment.
- **Deterministic control flow** — no hidden behavior. Execution paths stay explicit. The compiler may optimize IR, but it does not change the intended CFG except through documented optimizations.
- **Optimization** — input code can be optimized with techniques such as constant propagation, constant folding, loop-invariant code motion, peephole optimization, function inlining, tail recursion elimination, and hot/cold branch placement.
- **Function overloading** — the compiler supports function overloading with some restrictions.
- **Default arguments** — functions may define default argument values.
- **Headers** — CPL supports header-style organization similar to C/C++.
- **Strings** — the language distinguishes strings from raw pointers.
- **Local functions** — functions may define local functions, similarly to Rust.
- **Lambda functions** - functions may define lambda functions without closure support.
- **Annotations** — the language supports annotations as a way to extend the restricted grammar.
- **Assembly generation** — the compiler generates an assembly code for further compilation.
- **Separated backend** — the compiler uses a shared backend interface for multiple targets such as ASM x86 (x16, x32, x64) and RISC-V.
- **Module system** - the compiler is designed to be easy to change the existing behaviour.
- **AST and IR static analysis** - the compiler has an in-build static analyzator which works on AST and HIR structures.