# Cordell Static Analysis
This folder contains CSA (Cordell Static Analyzator) files. Main idea of the `CSA` is detecting all possible errors in a `.cpl` code with `AST` and `HIR` usage. 

# Literature
- *Semantic with Application. An appetizer.*
- *Svace static analyzator development experience.*

# Navigation
- `ast/` - AST visitors.
- `hir/` - HIR visitors.

# How to add a new checker?
It is a really simple action. You just need to follow these steps:
- Define the target AST node(s) for the checker.
- Implement the checker with the help of the template below:
```c
// xxxx - checker's name
int ASTWLKR_xxxx(AST_VISITOR_ARGS) {
    // AST_VISITOR_ARGS == ast_node_t* nd, sym_table_t* smt
    return 1; // Checker confirm the provided node.
    return 0; // Checker fires a warning.
}
``` 
- Register the new checker in the `semantic.c` file:
```c
// ast_walker_t walker;
ASTWLK_register_visitor(NODE_TYPE, ASTWLKR_xxxx, &walker);
```
