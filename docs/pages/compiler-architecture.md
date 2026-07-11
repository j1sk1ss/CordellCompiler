# Compiler architecture

This page connects the documentation to the repository layout. It is intended as an implementation map for readers who want to inspect the compiler after reading the language reference.

## High-level pipeline

The compiler pipeline is:

1. **Preprocessing**: resolve include and macro directives.
2. **Tokenization**: split source text into token objects.
3. **Markup**: add early semantic information to tokens.
4. **AST generation**: parse declarations, expressions, control flow, annotations, syscalls, and inline assembly.
5. **Semantic analysis**: run optional source-level and IR-level diagnostics.
6. **HIR generation**: lower AST constructs into a high-level intermediate representation.
7. **CFG and call graph construction**: build control-flow and interprocedural information.
8. **Optimization**: apply HIR and LIR transformations.
9. **LIR generation**: lower HIR toward target-sensitive operations.
10. **Instruction selection and register allocation**: select target-specific x86 operations, allocate registers, and select memory locations.
11. **Assembly generation**: emit NASM assembly, assemble, and link.

## Source layout

| Area | Repository path | Role |
|---|---|---|
| Preprocessor | `src/preproc`, `include/preproc` | Include handling and simple macro directives |
| Token and markup preparation | `src/prep`, `include/prep` | Tokenization and early token markup |
| AST | `src/ast`, `include/ast` | Parser, AST data, AST-level optimizations and dumps |
| Semantic analysis | `src/sem`, `include/sem` | AST and HIR diagnostics, warnings, Z3-related experiments |
| HIR | `src/hir`, `include/hir` | HIR generation, CFG, SSA, DAG, loop and function transformations |
| LIR | `src/lir`, `include/lir` | LIR generation, data-flow information, copy propagation, instruction planning |
| Register allocation | `src/lir/regalloc`, `include/lir/regalloc` | Register allocation infrastructure |
| Instruction selection | `src/lir/selector`, `include/lir/selector` | Target-specific lowering and memory/register selection |
| Peephole optimization | `src/lir/peephole`, `include/lir/peephole` | Low-level rewrite passes |
| Assembly generation | `src/asm`, `include/asm` | NASM backend for supported x86 and x86-64 targets |
| Symbol tables | `src/symtab`, `include/symtab` | Variables, functions, strings, arrays, sections, and type tables |
| Tests | `tests` | Module and integration testing infrastructure |

## Intermediate representations

CPL uses multiple intermediate levels because the project is about compiler architecture rather than only source-to-assembly translation.

- **AST** preserves source-level constructs and supports source diagnostics.
- **HIR** represents a higher-level lowered program suitable for CFG/SSA analysis and optimization.
- **LIR** is closer to target code and supports instruction planning, register allocation, memory selection, and peephole optimization.
- **ASM** is the final NASM text emitted for the selected target.

This division is similar to the separation used in production compilers, but with much smaller data structures and fewer target abstractions.

## Diagnostics

Diagnostics are split by abstraction level:

- AST analysis catches source-level mistakes such as wrong argument count, invalid return use, read-only updates, missing exits, and invalid array access.
- HIR/IR analysis catches lower-level issues such as null dereferences, suspicious constant branches, and target-sensitive syscall misuse.

See [Semantic static checker](#/pages/semantic-static-checker) for examples.

## Optimization areas

The documented optimization work includes:

- constant folding and propagation;
- loop-invariant code motion;
- tail-recursion elimination;
- function inlining;
- copy propagation;
- peephole optimization;
- liveness-based stack slot reuse;
- register allocation experiments.

Some optimizations are more mature than others. Treat optimization pages and changelog entries as implementation documentation, not as a formal optimization contract.

## Target model

The backend currently focuses on NASM-style assembly for x86-family targets. x86-64 Mach-O, x86-64 Linux, and i386 Linux-style paths exist, but target behavior depends on command-line configuration and external assembler/linker tools. CPL does not currently provide a target-independent runtime library comparable to mature systems languages.
