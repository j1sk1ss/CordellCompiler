# Compiler usage

Build the compiler from the repository root:

```bash
make install && make cpllib
cplc -v
```

## Toolchain requirements

The compiler emits NASM assembly, assembles it to object files, and links those objects into the final executable. </br> 
Default tools and target are selected at compile time:

| Host build | Assembler format | Linker | Entry symbol | System type | Sections |
|---|---|---|---|---|---|
| Linux | `elf64` | `gcc` with `--linker-no-pie` defaulted on | `main` | `linux64` | `.rodata`, `.data`, `.text` |
| non-Linux default path | `macho64` | `clang` | `_main` | `macho64` | `__TEXT,__const`, `__DATA,__data`, `__TEXT,__text` |

The explcit build command is next:

```bash
cplc                 \
  --arch x86_64      \
  --sys-type linux64 \
  --asm-format elf64 \
  --linker gcc       \
  --linker-no-pie    \
  --output hello     \
  hello.cpl
./hello
```

The implicit command is:

```bash
cplc hello.cpl
./a.out
```

## Include paths

Use `-I <dir>` to add an include directory:

```bash
./builds/<platform>/cplc -I examples --output app main.cpl
```

Quoted includes first search relative to the current file. System-style includes search the include directory passed by `-I`, then the standard library found by the compiler. The standard-library lookup checks `CPL_INCLUDE_PATH`, adjacent package/install locations, the compiled-in install prefix, and finally a local `cpllib` directory.

```cpl
#include "local_header.cpl"
#include <library_header.cpl>
```

Useful preprocessor-related flags:

```bash
-E
-DNAME=value
--print-stdlib-path
```

## Output modes

By default the compiler assembles and links an executable. These flags also write intermediate output files:

| Flag | Output |
|---|---|
| `--emit-ast` | `output.ast` |
| `--emit-ir` | `output.ir` |
| `--emit-lir` | `output.lir` |
| `--emit-asm` | `output.s` |

Each default path can be replaced:

```bash
--ast-output ast.txt
--ir-output hir.txt
--lir-output lir.txt
--asm-output program.s
```

These emit flags write dumps in addition to the selected build mode.

Build modes:

| Flag | Behavior |
|---|---|
| `-E` | preprocess input and stop |
| `--CSA` | run Cordell Static Analyzer |
| `-c`, `--compile-only` | build an object file and skip linking |
| no build-mode flag | assemble and link an executable |

## Optimization flags

Profiles:

| Flag | Current behavior |
|---|---|
| `-O0` | disables optimization flags |
| `-O1` | *currently the same as `-O0`* |
| `-O2` | enables LICM, constant propagation/folding, and peephole optimization |
| `-O3` | enables `-O2` plus copy propagation, tail recursion elimination, and function inlining |

Individual flags:

```bash
--tre / --no-tre
--finline / --no-finline
--licm / --no-licm
--constant / --no-constant
--copyprop / --no-copyprop
--peephole / --no-peephole
```

## Target and section options

Common target options:

```bash
--arch x86_64
--sys-type linux64
--asm-compiler nasm
--asm-format elf64
--linker gcc
--linker-mode c
--entry-name _main
--full-bytness 8
--half-bytness 4
--quart-bytness 2
--eight-bytness 1
```

Section names can also be configured:

```bash
--ro-section .rodata
--glob-section .data
--code-section .text
```

`--sys-type macho64`, `--sys-type linux64`, and `--sys-type i386` also set section defaults for their target family. `--arch x86_64` selects 64-bit bytness and `elf64`; `--arch i386`, `--arch x86`, and `--arch ia32` select 32-bit bytness, `elf32`, and `--linker-m32`.

Linker-related switches:

```bash
--linker-pie / --linker-no-pie
--linker-m32 / --linker-no-m32
-L<dir> / -L <dir>
-l<name> / -l <name>
-Wl,<arg>
-Xlinker <arg> / --linker-arg <arg>
```

Library and raw linker arguments are appended to the final linker command after
the generated object files and `libcpl.a`.

## Analysis flags

The compiler has optional static analysis passes:

```bash
--ast-analysis
--ir-analysis
--CSA
```

AST analysis checks source-level semantic issues. IR analysis runs after HIR and CFG construction and can report lower-level problems such as suspicious control flow and invalid memory patterns. `--CSA` enables both analysis passes and stops before code generation.
