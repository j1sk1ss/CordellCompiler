# HIR
- `cfg/` - All code for Control-Flow-Graph generation and dominant, strict dominance and dominance frontier calculation.
- `constfold/` - All aligned code for constant propagation (and constant folding). The code entirely based of SSA form and the DAG.
- `dag/` - All code for Direct-Acyclic-Graph generation.
- `dump/` - Code for dumping and loading HIR form.
- `func/` - Funcs-related code (e.g. TRE, Inline, CallGraph, Overload, DeadFuncElimination).
- `hirgen/` - HIR generators from AST form.
- `loop` - Loops-related code (e.g. LICM, LoopTree).
- `ssa/` - All code for SSA-form generation.
- `hir_types.c` - Misc functions for HIR types.
- `hir.c` - Main logic of this layer.
- `hirgen.c` - Entry point in this layer.

The HIR form is a important part of the compilation process. Here we perform the major amount of avaliable optimizations such as LICM, inline, TRE, dead code and func elimination, etc. At the top we have a `HHIR` (High-High Intermediate Representation) where we're trying to save all syntax constructs such as ifs, whiles, loops and switches. At the bottom we have a `LHIR` (Low-High Intermidiate Representation) where we transfer code to SSA form, insert PHI functions, break high abstractions such as an overloading.
