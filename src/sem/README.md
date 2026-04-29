# Cordell Static Analysis
This folder contains CSA (Cordell Static Analyzator) files. Main idea of the `CSA` is detecting all possible errors in a `.cpl` file with `AST`, `HIR` and `z3` usage. 

# Literature
- *Semantic with Application. An appetizer.*
- *Svace static analyzator development experience.*
- *Z3* [[1]](https://www.microsoft.com/en-us/research/project/z3-3/) 
- *Z3: an efficient SMT solver* [[2]](https://www.researchgate.net/publication/225142568_Z3_an_efficient_SMT_solver)

# Navigation
- `ast/` - AST visitors. These visitors based on AST tree.
- `hir/` - HIR visitors. These visitors based on CFG.

# How to add a new AST checker?
It is a really simple action. You just need to follow these steps:
- Define the target AST node(s) for the checker.
- Implement the checker with the help of the template below:
```c
// xxxx - checker's name
int ASTWLKR_xxxx(AST_VISITOR_ARGS) {
    // AST_VISITOR_ARGS == ast_node_t* nd, sym_table_t* smt
    AST_VISITOR_ARGS_USE; // (void*)nd; (void*)smt;
    return 1; // Checker confirm the provided node.
    return 0; // Checker fires a warning.
}
``` 
- Register the new checker in the `semantic.c` file in the `SEM_perform_ast_check` function:
```c
// ast_walker_t walker;
// SEM_perform_ast_check
ASTWLK_register_visitor(NODE_TYPE, ASTWLKR_xxxx, &walker, ATTENTION_UNKNOWN_LEVEL);
```

# How to add a new HIR checker?
The same as it can be performed in the AST part. You need to create a new function as it presented below:
```c
// xxxx - checker's name
int HIRWLKR_xxxx(HIR_VISITOR_ARGS) {
    // HIR_VISITOR_ARGS_USE == hir_block_t* b, cfg_block_t* bb, sym_table_t* smt, hir_visitors_ctx_t* ctx
    HIR_VISITOR_ARGS_USE;
    return 1; // Checker confirm the provided node.
    return 0; // Checker fires a warning.
}
```

Then register this in the `semantic.c` file:
```c
// SEM_perform_hir_check
HIRWLK_register_visitor(INST_TYPE, HIRWLKR_xxxx, &walker, ATTENTION_UNKNOWN_LEVEL);
```

# Z3 SMT Solver
CSA involves Z3 as SMT solver to prove the next list of questions:
- Is a program correct?
- Can program cause a Dereference of Null error?
- Is this code reachable? 

It works with the python bindings (you can find them in the `z3_wrapper` folder). Actually, such an approach is similar to how Svace static analyzer works, that's why I did the same thing. </br>
P.S.: *This is a pure experiment and can't be considered as a solid research. I'm just trying to obtain some experience with SMT solvers.*
