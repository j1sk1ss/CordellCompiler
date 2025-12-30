# HIR generator
- `asm/` - HIR generators for the asm AST nodes.
- `assign/` - HIR generators for the assign operation AST nodes.
- `body/` - Code body HIR generators.
- `break/` - Breakpoint HIR generators.
- `cond/` - Condition HIR generators.
- `conv/` - Convertion / Casting HIR generators.
- `decl/` - Declaration HIR generators.
- `expr/` - Expression / Operands HIR generators.
- `func/` - Function related HIR generators.
- `import/` - Import HIR generators.
- `storage/` - Storage (Load / Store) HIR generators.
- `syscall/` - Syscall HIR generators.

# How to add a new HIR generator?
## HIR
Create a new directory in the `hirgen/` directory. Then choose the unique name, and implement a main generator function. Function must be called according to the next pattern: `HIR_generate_<name/token>`. Then, register it in the `body/block_hirgen.c` module.

## LIR
Often a new generator requiers a new LIR transformer. Check the `lir/lirgen/README.md` for the more information.

