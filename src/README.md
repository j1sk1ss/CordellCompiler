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