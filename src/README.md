# SRC
| Directory | Description | Note |
|-|-|-|
| `preproc/` | Machine independent pre-processing.                                    | Text To Text.       |
| `prep/`    | Machine independent tokenization.                                      | Text To Tokens.     |
| `ast/`     | Machine independent AST (Abstract Syntax Tree) generation.             | Tokens To AstNodes. |
| `hir/`     | Machine independent HIR (High Intermediate Representation) generation. | AstNodes to 3AC.    |
| `sem/`     | AST semantic check.                                                    | -                   |
| `lir/`     | Machine dependent LIR (Low Intermediate Representation) generation.    | 3AC To 3AC.         |
| `asm/`     | Machine dependent assembler generation.                                | 3AC to ASM.         |
| `symtab/`  | Symbol table.                                                          | -                   |
| `config.c` | Compiler shared configuration data.                                    | -                   |

P.S.: Arch dependent files are:
- src/asm/x86_64_gnu_nasm/*.c - Direct ASM translation.
- src/lir/selector/x86_64_gnu_nasm/*.c - From HLIR to LLIR (from not depent to a depent version).
- src/lir/peephole/x86_64_gnu_nasm.c - Second and third optimization passes. 

And if there is a task to add a new platform, you will need to make new directories with the same logic, but related to your platform.
