# AST parsers
- `asm/` - Inline assembly related parsers.
- `body/` - Code body generation related parsers.
- `break/` - Code breakpoint related parsers.
- `cond/` - Control Flow related parsers.
- `decl/` - Value declaration releated parsers.
- `expr/` - Expression related parsers.
- `func/` - Function related parsers.
- `import/` - Import and extern related parsers.
- `syscall/` - Sycall generation related parsers.

# How to create a new parser?
## AST
Create a new directory in the `astgens/` directory. Then choose the unique name, and implement a main parser function. Function must be called according to the next pattern: `cpl_parse_<name/token>`. Then, register it in the `body/block_pars.c` module.

## HIR
Often a new parser requiers a new HIR transformer. Check the `hir/hirgen/README.md` for the more information.
