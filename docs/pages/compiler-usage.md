# Compiler usage

Build the compiler from the repository root:

```bash
make
```

The default target creates `builds/ccompiler`. The help text calls the executable `ccpl`, but in this repository the built binary is `builds/ccompiler`.

```bash
./builds/ccompiler [options] <input files>
```

## Toolchain requirements

The compiler emits NASM assembly, assembles it to object files, and links those objects into the final executable.

Default tools and target:

| Setting | Default |
|---|---|
| assembler | `nasm` |
| assembler format | `macho64` |
| linker | `clang` |
| linker mode | C driver |
| system type | `macho64` |

For Linux x86-64, pass Linux-specific options explicitly:

```bash
./builds/ccompiler \
  --arch x86_64 \
  --sys-type linux64 \
  --asm-format elf64 \
  --linker gcc \
  --linker-no-pie \
  --output hello \
  hello.cpl
```

For macOS x86-64, the defaults are already close to the expected target:

```bash
./builds/ccompiler --output hello hello.cpl
```

## Include paths

Use `-I <dir>` to add an include directory:

```bash
./builds/ccompiler -I examples --output app main.cpl
```

Quoted includes first search relative to the current file. System-style includes search the include directory passed by `-I`.

```cpl
#include "local_header.cpl"
#include <library_header.cpl>
```

## Output modes

By default the compiler links an executable. These flags also write intermediate output files in the current working directory:

| Flag | Output |
|---|---|
| `--emit-ast` | `output.ast` |
| `--emit-ir` | `output.ir` |
| `--emit-asm` | `output.s` |

In the current implementation, `--emit-asm` still continues through assembly and linking. Treat it as "also emit assembly", not as "stop after assembly".

## Optimization flags

Profiles:

| Flag | Current behavior |
|---|---|
| `-O0` | disables optimization flags |
| `-O1` | currently the same as `-O0` |
| `-O2` | enables LICM, constant propagation/folding, and peephole optimization |
| `-O3` | enables `-O2` plus copy propagation and tail recursion elimination |

Individual flags:

```bash
--tre / --no-tre
--finline / --no-finline
--licm / --no-licm
--constant / --no-constant
--peephole / --no-peephole
```

Function inlining is available through `--finline`, but it is not enabled by the `-O3` profile in the current CLI code.

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
```

Section names can also be configured:

```bash
--ro-section .rodata
--glob-section .data
--code-section .text
```

The default section names are Mach-O style: `__TEXT,__const`, `__DATA,__data`, and `__TEXT,__text`.

## Analysis flags

The compiler has optional static analysis passes:

```bash
--ast-analysis
--ir-analysis
```

AST analysis checks source-level semantic issues. IR analysis runs after HIR and CFG construction and can report lower-level problems such as suspicious control flow and invalid memory patterns.
